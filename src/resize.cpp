#include "resize.hpp"
#include "types.hpp"
#include <cmath>
#include <cstddef>
#include <vector>
#include "image.hpp"

NAMESPACE_BEGIN

constexpr uint8_t k_shift = 7;

#define ST3_LAN1_OFS(REG, D0, D1, D2)                     \
    "st3 {"#D0".b, "#D1".b, "#D2".b}[0], ["#REG"], #3 \n"

#define ST3_LAN2_OFS(REG, D0, D1, D2)                     \
    "st3 {"#D0".b, "#D1".b, "#D2".b}[0], ["#REG"], #3 \n" \
    "st3 {"#D0".b, "#D1".b, "#D2".b}[1], ["#REG"], #3 \n"

#define ST3_LAN4_OFS(REG, D0, D1, D2)                     \
    ST3_LAN2_OFS(REG, D0, D1, D2)                         \
    "st3 {"#D0".b, "#D1".b, "#D2".b}[2], ["#REG"], #3 \n" \
    "st3 {"#D0".b, "#D1".b, "#D2".b}[3], ["#REG"], #3 \n"

#define ST3_LAN1(REG, D0, D1, D2)                         \
    "st3 {"#D0".b, "#D1".b, "#D2".b}[0], ["#REG"]     \n"

#define ST3_LAN2(REG, D0, D1, D2)                         \
    "st3 {"#D0".b, "#D1".b, "#D2".b}[0], ["#REG"]     \n" \
    "st3 {"#D0".b, "#D1".b, "#D2".b}[1], ["#REG"]     \n"

#define ST3_LAN4(REG, D0, D1, D2)                         \
    ST3_LAN2(REG, D0, D1, D2)                             \
    "st3 {"#D0".b, "#D1".b, "#D2".b}[2], ["#REG"]     \n" \
    "st3 {"#D0".b, "#D1".b, "#D2".b}[3], ["#REG"]     \n"

#define LD3_LAN1_OFS(REG, D0, D1, D2)                     \
    "ld3 {"#D0".b, "#D1".b, "#D2".b}[0], ["#REG"], #3 \n"

#define LD3_LAN2_OFS(REG, D0, D1, D2)                     \
    "ld3 {"#D0".b, "#D1".b, "#D2".b}[0], ["#REG"], #3 \n" \
    "ld3 {"#D0".b, "#D1".b, "#D2".b}[1], ["#REG"], #3 \n"

#define LD3_LAN4_OFS(REG, D0, D1, D2)                     \
    LD3_LAN2_OFS(REG, D0, D1, D2)                         \
    "ld3 {"#D0".b, "#D1".b, "#D2".b}[2], ["#REG"], #3 \n" \
    "ld3 {"#D0".b, "#D1".b, "#D2".b}[3], ["#REG"], #3 \n"

#define LD3_LAN1(REG, D0, D1, D2)                         \
    "ld3 {"#D0".b, "#D1".b, "#D2".b}[0], ["#REG"]     \n"

#define LD3_LAN2(REG, D0, D1, D2)                         \
    "ld3 {"#D0".b, "#D1".b, "#D2".b}[0], ["#REG"]     \n" \
    "ld3 {"#D0".b, "#D1".b, "#D2".b}[1], ["#REG"]     \n"

#define LD3_LAN4(REG, D0, D1, D2)                         \
    LD3_LAN2(REG, D0, D1, D2)                             \
    "ld3 {"#D0".b, "#D1".b, "#D2".b}[2], ["#REG"]     \n" \
    "ld3 {"#D0".b, "#D1".b, "#D2".b}[3], ["#REG"]     \n"

#define MUL(S0, S1, A0, A1)                               \
    "umull "#S0".8h, "#S0".8b, "#A0".8b               \n" \
    "umlal "#S0".8h, "#S1".8b, "#A1".8b               \n" \
    "uqrshrn "#S0".8b, "#S0".8h, %[shift]             \n"

#define ADDLP(V)                                          \
    "uaddlp "#V".8h, "#V".16b                         \n" \
    "uqrshrn "#V".8b, "#V".8h, #1                     \n"

class ResizeBuffer {
 public:
    ResizeBuffer(ResizeParam param, int32_t src, int32_t dst, int32_t channel = 1)
        : stride_{dst * channel}
        , buff_((1L + sizeof(int32_t)) * dst, 0)
    {
        constexpr auto scale = 1U << k_shift;
        auto param_buf = getParam();
        auto start_buf = getStart();
        auto scale_d = static_cast<double>(src) / dst;
        if (param.m_half_pixel) {
            for (int32_t i = 0; i < dst; ++i) {
                auto src_f = ((static_cast<double>(i) + 0.5F) * scale_d) - 0.5F;
                auto src_floor = static_cast<int32_t>(std::floor(src_f));
                auto alpha = static_cast<decltype(scale)>(scale * (src_f - src_floor));
                if (src_floor < 0) {
                    src_floor = 0;
                    alpha = 0;
                } else if (src_floor >= src - 1) {
                    src_floor = src - 2;
                    alpha = scale;
                }
                param_buf[i] = scale - alpha;
                start_buf[i] = src_floor * channel;
            }
        } else if (param.m_align_corner) {}
    }

    uint8_t *getParam() {
        return buff_.data() + (sizeof(int32_t) * stride_);
    }

    int32_t *getStart() {
        return reinterpret_cast<int32_t*>(buff_.data());    // NOLINT
    }
 private:
    int32_t stride_{0};
    std::vector<uint8_t> buff_;
};
/*

void resize_comm_c1(const Image& src, const Image &dst) {
    auto dw = dst.cols();
    auto dh = dst.rows();
    ResizeBuffer buff(src.cols(), src.rows(), dw, dh, 1);
    auto ss = src.stride();
    auto w_remain = dw & 0x7;
    auto w8 = dw - w_remain;

    auto src_ptr = src.data();
    auto dst_ptr = dst.data();
    auto alpha = buff.getAlpha();
    auto beta = buff.getBeta();
    auto cache = buff.getCache();
    auto start_w = buff.startW();
    auto start_h = buff.startH();

    #undef LOAD_LINE0
    #define LOAD_LINE0(OFS, IDX, REG)           \
        "ldrsw "#REG", [x8, #"#OFS"]        \n" \
        "add "#REG", x0, "#REG"             \n" \
        "ld2 {v4.b, v5.b}["#IDX"], ["#REG"] \n"

    #undef LOAD_LINE1
    #define LOAD_LINE1(IDX, REG)                \
        "add "#REG", "#REG", %[ss]          \n" \
        "ld2 {v6.b, v7.b}["#IDX"], ["#REG"] \n"

    #undef LOAD_8_LINE0
    #define LOAD_8_LINE0                        \
        LOAD_LINE0(-32, 0, x9)                  \
        LOAD_LINE0(-28, 1, x10)                 \
        LOAD_LINE0(-24, 2, x11)                 \
        LOAD_LINE0(-20, 3, x12)                 \
        LOAD_LINE0(-16, 4, x13)                 \
        LOAD_LINE0(-12, 5, x14)                 \
        LOAD_LINE0(-8, 6, x15)                  \
        LOAD_LINE0(-4, 7, x16)

    #undef LOAD_8_LINE1
    #define LOAD_8_LINE1                        \
        LOAD_LINE1(0, x9)                       \
        LOAD_LINE1(1, x10)                      \
        LOAD_LINE1(2, x11)                      \
        LOAD_LINE1(3, x12)                      \
        LOAD_LINE1(4, x13)                      \
        LOAD_LINE1(5, x14)                      \
        LOAD_LINE1(6, x15)                      \
        LOAD_LINE1(7, x16)

    #undef CALC_LINE
    #define CALC_LINE(D, S0, S1)                \
        "umull "#D".8h, "#S0".8b, v2.8b     \n" \
        "umlal "#D".8h, "#S1".8b, v3.8b     \n" \
        "uqrshrn "#D".8b, "#D".8h, %[shift] \n"

    #undef CALC_2_LINE
    #define CALC_2_LINE(D, S0, S1)              \
        "umull "#D".8h, "#S0".8b, v0.8b     \n" \
        "umlal "#D".8h, "#S1".8b, v1.8b     \n" \
        "uqrshrn "#D".8b, "#D".8h, %[shift] \n"

    asm volatile(
        R"(
                            // x0 --> src0
        mov x1, xzr         // x1 --> h loop control
        mov x2, #-1         // x2 --> prev_h
                            // x3 --> current h
                            // x4 --> w loop control
                            // x5 --> w + 8
                            // x6 --> alpha addr
        mov x7, %[cache]    // x7 --> cache addr
        movi v31.8b, #1     // v31.8b
        shl v31.8b, v31.8b, %[shift]
        b RC_C1_H
    RC_C1_H_PRE:
        add x2, x3, #1
        mov x7, %[cache]
        add x1, x1, #1
    RC_C1_H:
        cmp x1, %[h]
        bge RC_C1_END
        ldrsw x3, [%[start_h], x1, lsl #2]  // h
        madd x0, x3, %[ss], %0              // src0
        ld1r {v0.8b}, [%[beta]], #1              // beta_vec
        sub v1.8b, v31.8b, v0.8b
        mov x4, xzr
        mov x6, %[alpha]                    // alpha
        cmp x2, x3
        beq RC_C1_H_CACHE_PRE

        RC_C1_H_L8:
            add x5, x4, #8
            cmp x5, %[w8]
            bgt RC_C1_H_REMAIN
            ld1 {v2.8b}, [x6], #8
            sub v3.8b, v31.8b, v2.8b
            add x8, %[start_w], x5, lsl #2
        )"
            LOAD_8_LINE0
            CALC_LINE(v4, v4, v5)
            "prfm pldl1keep, [x9, 448]  \n"
            LOAD_8_LINE1
            CALC_LINE(v5, v6, v7)
            "prfm pldl1keep, [x9, 448]  \n"
            "st1 {v5.8b}, [x7], #8      \n"
            CALC_2_LINE(v6, v4, v5)
        R"(
            st1 {v6.8b}, [%1], #8
            mov x4, x5
            b RC_C1_H_L8

        RC_C1_H_REMAIN:
            cbz %[w_remain], RC_C1_H_PRE
            sub x5, %[w_remain], #8
            add x6, x6, x5
            add x7, x7, x5
            add %1, %1, x5
            add x5, x4, x5
            ld1 {v2.8b}, [x6], #8
            sub v3.8b, v31.8b, v2.8b
            add x8, %[start_w], x5, lsl #2
        )"
            LOAD_8_LINE0
            CALC_LINE(v4, v4, v5)
            LOAD_8_LINE1
            CALC_LINE(v5, v6, v7)
            "st1 {v5.8b}, [x7], #8      \n"
            CALC_2_LINE(v6, v4, v5)
        R"(
            st1 {v6.8b}, [%1], #8
            b RC_C1_H_PRE

        RC_C1_H_CACHE_PRE:
            add x0, x0, %[ss]
        RC_C1_H_CACHE_L8:
            add x5, x4, #8
            cmp x5, %[w8]
            bgt RC_C1_H_CACHE_REMAIN
            ld1 {v4.8b}, [x7]
            ld1 {v2.8b}, [x6], #8
            sub v3.8b, v31.8b, v2.8b
            add x8, %[start_w], x5, lsl #2
        )"
            LOAD_8_LINE0
            CALC_LINE(v5, v4, v5)
            "prfm pldl1keep, [x9, 448]  \n"
            "st1 {v5.8b}, [x7], #8      \n"
            CALC_2_LINE(v6, v4, v5)
        R"(
            st1 {v6.8b}, [%1], #8
            mov x4, x5
            b RC_C1_H_CACHE_L8

        RC_C1_H_CACHE_REMAIN:
            cbz %[w_remain], RC_C1_H_PRE
            sub x5, %[w_remain], #8
            add x6, x6, x5
            add x7, x7, x5
            ld1 {v2.8b}, [x6], #8
            add %1, %1, x5
            sub v3.8b, v31.8b, v2.8b
            add x5, x4, x5
            add x8, %[start_w], x5, lsl #2
        )"
            LOAD_8_LINE0
            CALC_LINE(v5, v4, v5)
            "st1 {v5.8b}, [x7], #8      \n"
            CALC_2_LINE(v6, v4, v5)
        R"(
            st1 {v6.8b}, [%1], #8
            b RC_C1_H_PRE
    RC_C1_END:
    )"
    :
    : "r"(src_ptr), "r"(dst_ptr), [ss] "r"(ss), [w8] "r"(1L * w8), [w_remain] "r"(1L * w_remain), [h] "r"(1L * dh)
    , [alpha] "r"(alpha), [beta] "r"(beta), [start_w] "r"(start_w)
    , [start_h] "r"(start_h), [cache] "r"(cache), [shift]"i"(k_shift)
    : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8"
    , "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16"
    , "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9"
    , "v31"
    );
}
*/

static void resize_dn2_c1(const Image& src, const Image &dst, ResizeParam param) {
    // clang-format off

    asm volatile(
        // x0  --> src0
        // x1  --> h loop
        // x2  --> w loop
        // x3  --> w remain
        "mov x0, %0                                     \n" \
        "mov x1, xzr                                    \n" \
        "b RDN2_C1_H                                    \n" \
        "RDN2_C1_H_PRE:                                 \n" \
        "   add x1, x1, #1                              \n" \
        "   add x0, x0, %[ss]                           \n" \
        "RDN2_C1_H:                                     \n" \
        "   cmp x1, %[h]                                \n" \
        "   bge RDN2_C1_END                             \n" \
        "   and x3, %[w], #15                           \n" \
        "   sub x2, %[w], x3                            \n" \
        "   cbz x2, RDN2_C1_W4                          \n" \
        "   RDN2_C1_W16:                                \n" \
        "       subs x2, x2, #16                        \n" \
        "       ld2 {v0.16b, v1.16b}, [x0], #32         \n" \
        "       urhadd v0.16b, v0.16b, v1.16b           \n" \
        "       st1 {v0.16b}, [%1], #16                 \n" \
        "       bgt RDN2_C1_W16                         \n" \
        "   RDN2_C1_W8:                                 \n" \
        "       cmp x3, #8                              \n" \
        "       blt RDN2_C1_W4                          \n" \
        "       ld2 {v0.8b, v1.8b}, [x0], #16           \n" \
        "       urhadd v0.8b, v0.8b, v1.8b              \n" \
        "       st1 {v0.8b}, [%1], #8                   \n" \
        "       sub x3, x3, #8                          \n" \
        "   RDN2_C1_W4:                                 \n" \
        "       cmp x3, #4                              \n" \
        "       blt RDN2_C1_W2                          \n" \
        "       ld1 {v0.8b}, [x0], #8                   \n" \
                ADDLP(v0)                                 \
        "       st1 {v0.s}[0], [%1], #4                 \n" \
        "       sub x3, x3, #4                          \n" \
        "   RDN2_C1_W2:                                 \n" \
        "       cmp x3, #2                              \n" \
        "       blt RDN2_C1_W1                          \n" \
        "       ld1 {v0.s}[0], [x0], #4                 \n" \
                ADDLP(v0)                                 \
        "       st1 {v0.h}[0], [%1], #2                 \n" \
        "       sub x3, x3, #2                          \n" \
        "   RDN2_C1_W1:                                 \n" \
        "       cmp x3, #1                              \n" \
        "       blt RDN2_C1_H_PRE                       \n" \
        "       ld1 {v0.h}[0], [x0], #2                 \n" \
                ADDLP(v0)                                 \
        "       st1 {v0.b}[0], [%1], #1                 \n" \
        "   blt RDN2_C1_H_PRE                           \n" \
        "RDN2_C1_END:                                   \n"
        :
        : "r"(src.data()), "r"(dst.data()), [h]"r"(1UL * dst.rows()), [w]"r"(1UL * dst.cols()), [ss]"r"(src.stride())
        : "cc", "memory", "x0", "x1", "x2", "x3", "v0", "v1"
    );
}

static void resize_dn2_c3(const Image& src, const Image &dst, ResizeParam param) {
    // clang-format off

    asm volatile(
        // x0  --> src0
        // x1  --> src1
        // x2  --> h loop
        // x3  --> w loop
        // x4  --> w remain
        "mov x0, %0                                     \n" \
        "add x1, %0, %[ss]                              \n" \
        "mov x2, xzr                                    \n" \
        "b RDN2_C3_H                                       \n" \
        "RDN2_C3_H_PRE:                                    \n" \
        "   add x2, x2, #1                              \n" \
        "   mov x0, x1                                  \n" \
        "   add x1, x1, %[ss]                           \n" \
        "RDN2_C3_H:                                        \n" \
        "   cmp x2, %[h]                                \n" \
        "   bge RDN2_C3_END                                \n" \
        "   and x4, %[w], 0x7                           \n" \
        "   sub x3, %[w], x4                            \n" \
        "   cbz x3, RDN2_C3_W4                             \n" \
        "   RDN2_C3_W8:                                    \n" \
        "       subs x3, x3, #8                         \n" \
        "       ld3 {v3.16b, v4.16b, v5.16b}, [x1], #48 \n" \
                ADDLP(v3)                                 \
                ADDLP(v4)                                 \
        "       ld3 {v0.16b, v1.16b, v2.16b}, [x0], #48 \n" \
                ADDLP(v5)                                 \
                ADDLP(v0)                                 \
        "       urhadd v0.8b, v0.8b, v3.8b              \n" \
                ADDLP(v1)                                 \
        "       urhadd v1.8b, v1.8b, v4.8b              \n" \
                ADDLP(v2)                                 \
        "       urhadd v2.8b, v2.8b, v5.8b              \n" \
        "       st3 {v0.8b, v1.8b, v2.8b}, [%1], #24    \n" \
        "       bgt RDN2_C3_W8                             \n" \
        "   RDN2_C3_W4:                                    \n" \
        "       cmp x4, #4                              \n" \
        "       blt RDN2_C3_W2                             \n" \
        "       ld3 {v3.8b, v4.8b, v5.8b}, [x1], #24    \n" \
                ADDLP(v3)                                 \
                ADDLP(v4)                                 \
        "       ld3 {v0.8b, v1.8b, v2.8b}, [x0], #24    \n" \
                ADDLP(v5)                                 \
                ADDLP(v0)                                 \
        "       urhadd v0.8b, v0.8b, v3.8b              \n" \
                ADDLP(v1)                                 \
        "       urhadd v1.8b, v1.8b, v4.8b              \n" \
                ADDLP(v2)                                 \
        "       urhadd v2.8b, v2.8b, v5.8b              \n" \
                ST3_LAN4_OFS(%1, v0, v1, v2)               \
        "       sub x4, x4, #4                          \n" \
        "   RDN2_C3_W2:                                    \n" \
        "       cmp x4, #2                              \n" \
        "       blt RDN2_C3_W1                             \n" \
                LD3_LAN4_OFS(x1, v3, v4, v5)               \
                ADDLP(v3)                                 \
                ADDLP(v4)                                 \
                LD3_LAN4_OFS(x0, v0, v1, v2)               \
                ADDLP(v5)                                 \
                ADDLP(v0)                                 \
        "       urhadd v0.8b, v0.8b, v3.8b              \n" \
                ADDLP(v1)                                 \
        "       urhadd v1.8b, v1.8b, v4.8b              \n" \
                ADDLP(v2)                                 \
        "       urhadd v2.8b, v2.8b, v5.8b              \n" \
                ST3_LAN2_OFS(%1, v0, v1, v2)               \
        "       sub x4, x4, #2                          \n" \
        "   RDN2_C3_W1:                                    \n" \
        "       cmp x4, #1                              \n" \
        "       blt RDN2_C3_H_PRE                          \n" \
                LD3_LAN2_OFS(x1, v3, v4, v5)               \
                ADDLP(v3)                                 \
                ADDLP(v4)                                 \
                LD3_LAN2_OFS(x0, v0, v1, v2)               \
                ADDLP(v5)                                 \
                ADDLP(v0)                                 \
        "       urhadd v0.8b, v0.8b, v3.8b              \n" \
                ADDLP(v1)                                 \
        "       urhadd v1.8b, v1.8b, v4.8b              \n" \
                ADDLP(v2)                                 \
        "       urhadd v2.8b, v2.8b, v5.8b              \n" \
                ST3_LAN1_OFS(%1, v0, v1, v2)               \
        "   b RDN2_C3_H_PRE                                \n" \
        "RDN2_C3_END:                                      \n"
        :
        : "r"(src.data()), "r"(dst.data()), [h]"r"(1UL * dst.rows()), [w]"r"(1UL * dst.cols()), [ss]"r"(src.stride())
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4"
        , "v0", "v1", "v2", "v3", "v4", "v5"
    );
}

static void resize_dn2h_c3(const Image& src, const Image &dst, ResizeParam param) {
    ResizeBuffer buff{param, src.cols(), dst.cols(), 3};
    std::vector<uint8_t> cache(3UL * dst.rows(), 0);
    // clang-format off
    #define RDN2H_C3_LD_LINE0_LAN(REG, IDX, VEC)                        \
        "mov w"#REG", "#VEC"                                        \n" \
        "add x"#REG", x0, x"#REG"                                   \n" \
        "ld3 {v2.b, v3.b, v4.b}["#IDX"], [x"#REG"], #3              \n" \
        "ld3 {v5.b, v6.b, v7.b}["#IDX"], [x"#REG"], x7              \n"
    #define RDN2H_C3_LD_LINE1_LAN(REG, IDX)                             \
        "ld3 {v8.b, v9.b, v10.b}["#IDX"], [x"#REG"], #3             \n" \
        "ld3 {v11.b, v12.b, v13.b}["#IDX"], [x"#REG"]               \n"

    asm volatile(
        // x0 --> src0
        // x1 --> h loop
        // x2 --> w loop
        // x3 --> w remain
        // x4 --> start_w
        // x5 --> alpha
        // x6 --> cache
        // x7 --> %[ss] - 3
        "mov x0, %0                                     \n" \
        "mov x2, xzr                                    \n" \
        "mov x7, %[ss]                                  \n" \
        "sub x7, x7, #3                                 \n" \
        "movi v29.8b, #128                              \n" \
        "lsl %[ss], %[ss], #1                           \n" \
        "RDN2H_C3_H_PRE:                                \n" \
        "   add x1, x1, #1                              \n" \
        "   madd x0, %[ss], x1, %0                      \n" \
        "RDN2H_C3_H:                                    \n" \
        "   cmp x1, %[h]                                \n" \
        "   bge RDN2H_C3_END                            \n" \
        "   mov x6, %[cache]                            \n" \
        "   and x3, %[w], 0x7                           \n" \
        "   sub x2, %[w], x3                            \n" \
        "   mov x4, %[start_w]                          \n" \
        "   mov x5, %[alpha]                            \n" \
        "   cbz x2, RDN2H_C3_W4                         \n" \
        "   RDN2H_C3_W8:                                \n" \
        "       ld1 {v0.8b}, [x5], #8                   \n" \
        "       sub v1.8b, v29.8b, v0.8b                \n" \
        "       subs x2, x2, #8                         \n" \
        "       ld1 {v30.4s, v31.4s}, [x4], #32         \n" \
                RDN2H_C3_LD_LINE0_LAN(10, 0, v30.s[0])      \
                RDN2H_C3_LD_LINE0_LAN(11, 1, v30.s[1])      \
                RDN2H_C3_LD_LINE0_LAN(12, 2, v30.s[2])      \
                RDN2H_C3_LD_LINE0_LAN(13, 3, v30.s[3])      \
                RDN2H_C3_LD_LINE0_LAN(14, 4, v31.s[0])      \
                RDN2H_C3_LD_LINE0_LAN(15, 5, v31.s[1])      \
                RDN2H_C3_LD_LINE0_LAN(16, 6, v31.s[2])      \
                RDN2H_C3_LD_LINE0_LAN(17, 7, v31.s[3])      \
                MUL(v2, v5, v0, v1)                       \
                RDN2H_C3_LD_LINE1_LAN(10, 0)                \
                MUL(v3, v6, v0, v1)                       \
                RDN2H_C3_LD_LINE1_LAN(11, 1)                \
                MUL(v4, v7, v0, v1)                       \
                RDN2H_C3_LD_LINE1_LAN(12, 2)                \
                RDN2H_C3_LD_LINE1_LAN(13, 3)                \
                RDN2H_C3_LD_LINE1_LAN(14, 4)                \
                RDN2H_C3_LD_LINE1_LAN(15, 5)                \
                RDN2H_C3_LD_LINE1_LAN(16, 6)                \
                RDN2H_C3_LD_LINE1_LAN(17, 7)                \
                MUL(v8, v11, v0, v1)                      \
                MUL(v9, v12, v0, v1)                      \
                MUL(v10, v13, v0, v1)                     \
        "       st3 {v8.8b, v9.8b, v10.8b}, [x6], #24   \n" \
        "       urhadd v2.8b, v2.8b, v8.8b              \n" \
        "       urhadd v3.8b, v3.8b, v9.8b              \n" \
        "       urhadd v4.8b, v4.8b, v10.8b             \n" \
        "       st3 {v2.8b, v3.8b, v4.8b}, [%1], #24    \n" \
        "       bgt RDN2H_C3_W8                         \n" \
        "   RDN2H_C3_W4:                                \n" \
        "       cmp x3, #4                              \n" \
        "       blt RDN2H_C3_W2                         \n" \
        "       ld1 {v0.s}[0], [x5], #4                 \n" \
        "       sub v1.8b, v29.8b, v0.8b                \n" \
        "       sub x3, x3, #4                          \n" \
        "       ld1 {v30.4s}, [x4], #16                 \n" \
                RDN2H_C3_LD_LINE0_LAN(10, 0, v30.s[0])      \
                RDN2H_C3_LD_LINE0_LAN(11, 1, v30.s[1])      \
                RDN2H_C3_LD_LINE0_LAN(12, 2, v30.s[2])      \
                RDN2H_C3_LD_LINE0_LAN(13, 3, v30.s[3])      \
                MUL(v2, v5, v0, v1)                       \
                RDN2H_C3_LD_LINE1_LAN(10, 0)                \
                MUL(v3, v6, v0, v1)                       \
                RDN2H_C3_LD_LINE1_LAN(11, 1)                \
                MUL(v4, v7, v0, v1)                       \
                RDN2H_C3_LD_LINE1_LAN(12, 2)                \
                RDN2H_C3_LD_LINE1_LAN(13, 3)                \
                MUL(v8, v11, v0, v1)                      \
                MUL(v9, v12, v0, v1)                      \
                MUL(v10, v13, v0, v1)                     \
                ST3_LAN4_OFS(x6, v8, v9, v10)             \
        "       urhadd v2.8b, v2.8b, v8.8b              \n" \
        "       urhadd v3.8b, v3.8b, v9.8b              \n" \
        "       urhadd v4.8b, v4.8b, v10.8b             \n" \
                ST3_LAN4_OFS(%1, v2, v3, v4)              \
        "   RDN2H_C3_W2:                                \n" \
        "       cmp x3, #2                              \n" \
        "       blt RDN2H_C3_W1                         \n" \
        "       ld1 {v0.h}[0], [x5], #2                 \n" \
        "       sub v1.8b, v29.8b, v0.8b                \n" \
        "       sub x3, x3, #2                          \n" \
        "       ld1 {v30.1d}, [x4], #8                  \n" \
                RDN2H_C3_LD_LINE0_LAN(10, 0, v30.s[0])      \
                RDN2H_C3_LD_LINE0_LAN(11, 1, v30.s[1])      \
                MUL(v2, v5, v0, v1)                       \
                RDN2H_C3_LD_LINE1_LAN(10, 0)                \
                MUL(v3, v6, v0, v1)                       \
                RDN2H_C3_LD_LINE1_LAN(11, 1)                \
                MUL(v4, v7, v0, v1)                       \
                MUL(v8, v11, v0, v1)                      \
                MUL(v9, v12, v0, v1)                      \
                MUL(v10, v13, v0, v1)                     \
                ST3_LAN2_OFS(x6, v8, v9, v10)             \
        "       urhadd v2.8b, v2.8b, v8.8b              \n" \
        "       urhadd v3.8b, v3.8b, v9.8b              \n" \
        "       urhadd v4.8b, v4.8b, v10.8b             \n" \
                ST3_LAN2_OFS(%1, v2, v3, v4)              \
        "   RDN2H_C3_W1:                                \n" \
        "       cmp x3, #1                              \n" \
        "       blt RDN2H_C3_H_PRE                      \n" \
        "       ld1 {v0.b}[0], [x5], #1                 \n" \
        "       sub v1.8b, v29.8b, v0.8b                \n" \
        "       sub x3, x3, #2                          \n" \
        "       ld1 {v30.s}[0], [x4], #4                \n" \
                RDN2H_C3_LD_LINE0_LAN(10, 0, v30.s[0])      \
                MUL(v2, v5, v0, v1)                       \
                RDN2H_C3_LD_LINE1_LAN(10, 0)                \
                MUL(v3, v6, v0, v1)                       \
                MUL(v4, v7, v0, v1)                       \
                MUL(v8, v11, v0, v1)                      \
                MUL(v9, v12, v0, v1)                      \
                MUL(v10, v13, v0, v1)                     \
                ST3_LAN1_OFS(x6, v8, v9, v10)             \
        "       urhadd v2.8b, v2.8b, v8.8b              \n" \
        "       urhadd v3.8b, v3.8b, v9.8b              \n" \
        "       urhadd v4.8b, v4.8b, v10.8b             \n" \
                ST3_LAN1_OFS(%1, v2, v3, v4)              \
        "   b RDN2H_C3_H_PRE                            \n" \
        "RDN2H_C3_END:                                  \n"
        :
        : "r"(src.data()), "r"(dst.data()), [h]"r"(1UL * dst.rows()), [w]"r"(1UL * dst.cols())
        , [ss] "r"(src.stride()), [alpha]"r"(buff.getParam()), [start_w]"r"(buff.getStart())
        , [cache] "r"(cache.data()), [shift]"i"(k_shift)
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8"
        , "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17"
        , "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10"
        , "v11", "v12", "v13", "v29", "v30", "v31"
    );
    // clang-format on
}

static void resize_dn2w_c3(const Image& src, const Image &dst, ResizeParam param) {
    ResizeBuffer buff{param, src.rows(), dst.rows()};
    std::vector<uint8_t> cache(3UL * dst.rows(), 0);
    // clang-format off

    asm volatile(
        // x0  --> src0
        // x1  --> src1
        // x2  --> cache
        // x3  --> h loop
        // x4  --> prev line
        // x5  --> current line
        // x6  --> w loop
        // x7  --> w remain
        "mov x3, xzr                                    \n" \
        "mov x4, #-1                                    \n" \
        "movi v31.8b, #128                              \n" \
        "b RDN2W_C3_H                                   \n" \
        "RDN2W_C3_H_PRE:                                \n" \
        "   add x3, x3, #1                              \n" \
        "   add x4, x5, #1                              \n" \
        "RDN2W_C3_H:                                    \n" \
        "   cmp x3, %[h]                                \n" \
        "   bge RDN2W_C3_END                            \n" \
        "   ldrsw x5, [%[start_h], x3, lsl #2]          \n" \
        "   madd x0, x5, %[ss], %0                      \n" \
        "   add x1, x0, %[ss]                           \n" \
        "   ld1r {v0.8b}, [%[param]], #1                \n" \
        "   sub v1.8b, v31.8b, v0.8b                    \n" \
        "   mov x2, %[cache]                            \n" \
        "   and x7, %[w], 0x7                           \n" \
        "   sub x6, %[w], x7                            \n" \
        "   cmp x4, x5                                  \n" \
        "   bge RDN2W_C3_CACHE                          \n" \
        "   cbz x6, RDN2W_C3_W4                         \n" \
        "   RDN2W_C3_W8:                                \n" \
        "       subs x6, x6, #8                         \n" \
        "       ld3 {v5.16b, v6.16b, v7.16b}, [x1], #48 \n" \
                ADDLP(v5)                                 \
                ADDLP(v6)                                 \
        "       ld3 {v2.16b, v3.16b, v4.16b}, [x0], #48 \n" \
                ADDLP(v7)                                 \
                ADDLP(v2)                                 \
        "       st3 {v5.8b, v6.8b, v7.8b}, [x2], #24    \n" \
                MUL(v2, v5, v0, v1)                       \
                ADDLP(v3)                                 \
                MUL(v3, v6, v0, v1)                       \
                ADDLP(v4)                                 \
                MUL(v4, v7, v0, v1)                       \
        "       st3 {v2.8b, v3.8b, v4.8b}, [%1], #24    \n" \
        "       bgt RDN2W_C3_W8                         \n" \
        "   RDN2W_C3_W4:                                \n" \
        "       cmp x7, #4                              \n" \
        "       blt RDN2W_C3_W2                         \n" \
        "       sub x7, x7, #4                          \n" \
        "       ld3 {v5.8b, v6.8b, v7.8b}, [x1], #24    \n" \
                ADDLP(v5)                                 \
                ADDLP(v6)                                 \
        "       ld3 {v2.8b, v3.8b, v4.8b}, [x0], #24    \n" \
                ADDLP(v7)                                 \
                ADDLP(v2)                                 \
                ST3_LAN4_OFS(x2, v5, v6, v7)              \
                MUL(v2, v5, v0, v1)                       \
                ADDLP(v3)                                 \
                MUL(v3, v6, v0, v1)                       \
                ADDLP(v4)                                 \
                MUL(v4, v7, v0, v1)                       \
                ST3_LAN4_OFS(%1, v2, v3, v4)              \
        "   RDN2W_C3_W2:                                \n" \
        "       cmp x7, #2                              \n" \
        "       blt RDN2W_C3_W1                         \n" \
        "       sub x7, x7, #2                          \n" \
                LD3_LAN4_OFS(x1, v5, v6, v7)              \
                ADDLP(v5)                                 \
                ADDLP(v6)                                 \
                LD3_LAN4_OFS(x0, v2, v3, v4)              \
                ADDLP(v7)                                 \
                ADDLP(v2)                                 \
                ST3_LAN2_OFS(x2, v5, v6, v7)              \
                MUL(v2, v5, v0, v1)                       \
                ADDLP(v3)                                 \
                MUL(v3, v6, v0, v1)                       \
                ADDLP(v4)                                 \
                MUL(v4, v7, v0, v1)                       \
                ST3_LAN2_OFS(%1, v2, v3, v4)              \
        "   RDN2W_C3_W1:                                \n" \
        "       cmp x7, #1                              \n" \
        "       blt RDN2W_C3_H_PRE                      \n" \
                LD3_LAN2_OFS(x1, v5, v6, v7)              \
                ADDLP(v5)                                 \
                ADDLP(v6)                                 \
                LD3_LAN2_OFS(x0, v2, v3, v4)              \
                ADDLP(v7)                                 \
                ADDLP(v2)                                 \
                ST3_LAN1_OFS(x2, v5, v6, v7)              \
                MUL(v2, v5, v0, v1)                       \
                ADDLP(v3)                                 \
                MUL(v3, v6, v0, v1)                       \
                ADDLP(v4)                                 \
                MUL(v4, v7, v0, v1)                       \
                ST3_LAN1_OFS(%1, v2, v3, v4)              \
        "   b RDN2W_C3_H_PRE                            \n" \
        "RDN2W_C3_CACHE:                                \n" \
        "   cbz x6, RDN2W_C3_CW4                        \n" \
        "   RDN2W_C3_CW8:                               \n" \
        "       subs x6, x6, #8                         \n" \
        "       ld3 {v5.16b, v6.16b, v7.16b}, [x1], #48 \n" \
                ADDLP(v5)                                 \
        "       ld3 {v2.8b, v3.8b, v4.8b}, [x2]         \n" \
                ADDLP(v6)                                 \
                MUL(v2, v5, v0, v1)                       \
                ADDLP(v7)                                 \
                MUL(v3, v6, v0, v1)                       \
        "       st3 {v5.8b, v6.8b, v7.8b}, [x2], #24    \n" \
                MUL(v4, v7, v0, v1)                       \
        "       st3 {v2.8b, v3.8b, v4.8b}, [%1], #24    \n" \
        "       bgt RDN2W_C3_CW8                        \n" \
        "   RDN2W_C3_CW4:                               \n" \
        "       cmp x7, #4                              \n" \
        "       blt RDN2W_C3_CW2                        \n" \
        "       sub x7, x7, #4                          \n" \
        "       ld3 {v5.8b, v6.8b, v7.8b}, [x1], #24    \n" \
                ADDLP(v5)                                 \
                LD3_LAN4(x2, v2, v3, v4)                  \
                ADDLP(v6)                                 \
                MUL(v2, v5, v0, v1)                       \
                ADDLP(v7)                                 \
                MUL(v3, v6, v0, v1)                       \
                ST3_LAN4_OFS(x2, v5, v6, v7)              \
                MUL(v4, v7, v0, v1)                       \
                ST3_LAN4_OFS(%1, v2, v3, v4)              \
        "   RDN2W_C3_CW2:                               \n" \
        "       cmp x7, #2                              \n" \
        "       blt RDN2W_C3_CW1                        \n" \
        "       sub x7, x7, #2                          \n" \
                LD3_LAN4_OFS(x1, v5, v6, v7)              \
                ADDLP(v5)                                 \
                LD3_LAN2(x2, v2, v3, v4)                  \
                ADDLP(v6)                                 \
                MUL(v2, v5, v0, v1)                       \
                ADDLP(v7)                                 \
                MUL(v3, v6, v0, v1)                       \
                ST3_LAN2_OFS(x2, v5, v6, v7)              \
                MUL(v4, v7, v0, v1)                       \
                ST3_LAN2_OFS(%1, v2, v3, v4)              \
        "   RDN2W_C3_CW1:                               \n" \
        "       cmp x7, #1                              \n" \
        "       blt RDN2W_C3_H_PRE                      \n" \
        "       sub x7, x7, #1                          \n" \
                LD3_LAN2_OFS(x1, v5, v6, v7)              \
                ADDLP(v5)                                 \
                LD3_LAN1(x2, v2, v3, v4)                  \
                ADDLP(v6)                                 \
                MUL(v2, v5, v0, v1)                       \
                ADDLP(v7)                                 \
                MUL(v3, v6, v0, v1)                       \
                ST3_LAN1_OFS(x2, v5, v6, v7)              \
                MUL(v4, v7, v0, v1)                       \
                ST3_LAN1_OFS(%1, v2, v3, v4)              \
        "   b RDN2W_C3_H_PRE                            \n" \
        "RDN2W_C3_END:                                  \n"
    :
    : "r"(src.data()), "r"(dst.data()), [w]"r"(1UL * dst.cols()), [h]"r"(1UL * dst.rows())
    , [ss]"r"(src.stride()), [param]"r"(buff.getParam()), [start_h]"r"(buff.getStart())
    , [cache]"r"(cache.data()), [shift]"i"(k_shift)
    : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7"
    , "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7");
    // clang-format on
}

static void resize_comm_c3(const Image& src, const Image &dst, ResizeParam param) {
    ResizeBuffer buff_h{param, src.rows(), dst.rows()};
    ResizeBuffer buff_w{param, src.cols(), dst.cols(), 3};
    std::vector<uint8_t> cache(3UL * dst.rows(), 0);
    // clang-format off
    #define RDNC_C3_LD_LINE0_LAN(REG, IDX, VEC)                         \
        "mov w"#REG", "#VEC"                                        \n" \
        "add x"#REG", x0, x"#REG"                                   \n" \
        "ld3 {v4.b, v5.b, v6.b}["#IDX"], [x"#REG"], #3              \n" \
        "ld3 {v7.b, v8.b, v9.b}["#IDX"], [x"#REG"], x9              \n"

    #define RDNC_C3_LD_LINE1_LAN(REG, IDX)                              \
        "ld3 {v10.b, v11.b, v12.b}["#IDX"], [x"#REG"], #3           \n" \
        "ld3 {v13.b, v14.b, v15.b}["#IDX"], [x"#REG"]               \n"

    asm volatile(
        // x0  --> src0
        // x1  --> cache
        // x2  --> h loop
        // x3  --> prev line
        // x4  --> current line
        // x5  --> w loop
        // x6  --> w remain
        // x7  --> start w
        // x8  --> alpha
        // x9  --> ss -3
        "mov x2, xzr                                        \n" \
        "mov x3, #-1                                        \n" \
        "sub x9, %[ss], #-3                                 \n" \
        "movi v29.8b, #128                                  \n" \
        "b RDNC_C3_H                                        \n" \
        "RDNC_C3_H_PRE:                                     \n" \
        "   add x2, x2, #1                                  \n" \
        "   add x3, x4, #1                                  \n" \
        "RDNC_C3_H:                                         \n" \
        "   cmp x2, %[h]                                    \n" \
        "   bge RDNC_C3_END                                 \n" \
        "   ldrsw x4, [%[start_h]], #4                      \n" \
        "   madd x0, x4, %[ss], %0                          \n" \
        "   ld1r {v0.8b}, [%[beta]], #1                     \n" \
        "   sub v1.8b, v29.8b, v0.8b                        \n" \
        "   mov x1, %[cache]                                \n" \
        "   and x6, %[w], 0x7                               \n" \
        "   mov x7, %[start_w]                              \n" \
        "   sub x5, %[w], x6                                \n" \
        "   mov x8, %[alpha]                                \n" \
        "   cmp x3, x4                                      \n" \
        "   bge RDNC_C3_CACHE                               \n" \
        "   cbz x5, RDNC_C3_W4                              \n" \
        "   RDNC_C3_W8:                                     \n" \
        "       ld1 {v2.8b}, [x8], #8                       \n" \
        "       sub v3.8b, v29.8b, v2.8b                    \n" \
        "       subs x5, x5, #8                             \n" \
        "       ld1 {v30.4s, v31.4s}, [x7], #32             \n" \
                RDNC_C3_LD_LINE0_LAN(10, 0, v30.s[0])           \
                RDNC_C3_LD_LINE0_LAN(11, 1, v30.s[1])           \
                RDNC_C3_LD_LINE0_LAN(12, 2, v30.s[2])           \
                RDNC_C3_LD_LINE0_LAN(13, 3, v30.s[3])           \
                RDNC_C3_LD_LINE0_LAN(14, 4, v31.s[0])           \
                RDNC_C3_LD_LINE0_LAN(15, 5, v31.s[1])           \
                RDNC_C3_LD_LINE0_LAN(16, 6, v31.s[2])           \
                RDNC_C3_LD_LINE0_LAN(17, 7, v31.s[3])           \
                MUL(v4, v7, v2, v3)                           \
                RDNC_C3_LD_LINE1_LAN(10, 0)                     \
                MUL(v5, v8, v2, v3)                           \
                RDNC_C3_LD_LINE1_LAN(11, 1)                     \
                MUL(v6, v9, v2, v3)                           \
                RDNC_C3_LD_LINE1_LAN(12, 2)                     \
                RDNC_C3_LD_LINE1_LAN(13, 3)                     \
                RDNC_C3_LD_LINE1_LAN(14, 4)                     \
                RDNC_C3_LD_LINE1_LAN(15, 5)                     \
                RDNC_C3_LD_LINE1_LAN(16, 6)                     \
                RDNC_C3_LD_LINE1_LAN(17, 7)                     \
                MUL(v10, v13, v2, v3)                         \
                MUL(v11, v14, v2, v3)                         \
                MUL(v12, v15, v2, v3)                         \
        "       st3 {v10.8b, v11.8b, v12.8b}, [x1], #24     \n" \
                MUL(v4, v10, v0, v1)                          \
                MUL(v5, v11, v0, v1)                          \
                MUL(v6, v12, v0, v1)                          \
        "       st3 {v4.8b, v5.8b, v6.8b}, [%1], #24        \n" \
        "       bgt RDNC_C3_W8                              \n" \
        "   RDNC_C3_W4:                                     \n" \
        "       cmp x6, #4                                  \n" \
        "       blt RDNC_C3_W2                              \n" \
        "       ld1 {v2.s}[0], [x8], #4                     \n" \
        "       sub v3.8b, v29.8b, v2.8b                    \n" \
        "       ld1 {v30.4s}, [x7], #16                     \n" \
                RDNC_C3_LD_LINE0_LAN(10, 0, v30.s[0])           \
                RDNC_C3_LD_LINE0_LAN(11, 1, v30.s[1])           \
                RDNC_C3_LD_LINE0_LAN(12, 2, v30.s[2])           \
                RDNC_C3_LD_LINE0_LAN(13, 3, v30.s[3])           \
                MUL(v4, v7, v2, v3)                           \
                RDNC_C3_LD_LINE1_LAN(10, 0)                     \
                MUL(v5, v8, v2, v3)                           \
                RDNC_C3_LD_LINE1_LAN(11, 1)                     \
                MUL(v6, v9, v2, v3)                           \
                RDNC_C3_LD_LINE1_LAN(12, 2)                     \
                RDNC_C3_LD_LINE1_LAN(13, 3)                     \
                MUL(v10, v13, v2, v3)                         \
                MUL(v11, v14, v2, v3)                         \
                MUL(v12, v15, v2, v3)                         \
                ST3_LAN4_OFS(x1, v10, v11, v12)               \
                MUL(v4, v10, v0, v1)                          \
                MUL(v5, v11, v0, v1)                          \
                MUL(v6, v12, v0, v1)                          \
                ST3_LAN4_OFS(%1, v4, v5, v6)                  \
        "       sub x6, x6, #4                              \n" \
        "   RDNC_C3_W2:                                     \n" \
        "       cmp x6, #2                                  \n" \
        "       blt RDNC_C3_W1                              \n" \
        "       ld1 {v2.h}[0], [x8], #2                     \n" \
        "       sub v3.8b, v29.8b, v2.8b                    \n" \
        "       ld1 {v30.1d}, [x7], #8                      \n" \
                RDNC_C3_LD_LINE0_LAN(10, 0, v30.s[0])           \
                RDNC_C3_LD_LINE0_LAN(11, 1, v30.s[1])           \
                MUL(v4, v7, v2, v3)                           \
                RDNC_C3_LD_LINE1_LAN(10, 0)                     \
                MUL(v5, v8, v2, v3)                           \
                RDNC_C3_LD_LINE1_LAN(11, 1)                     \
                MUL(v6, v9, v2, v3)                           \
                MUL(v10, v13, v2, v3)                         \
                MUL(v11, v14, v2, v3)                         \
                MUL(v12, v15, v2, v3)                         \
                ST3_LAN2_OFS(x1, v10, v11, v12)               \
                MUL(v4, v7, v0, v1)                           \
                MUL(v5, v8, v0, v1)                           \
                MUL(v6, v9, v0, v1)                           \
                ST3_LAN2_OFS(%1, v4, v5, v6)                  \
        "       sub x6, x6, #2                              \n" \
        "   RDNC_C3_W1:                                     \n" \
        "       cmp x6, #1                                  \n" \
        "       blt RDNC_C3_H_PRE                           \n" \
        "       ld1 {v2.b}[0], [x8], #1                     \n" \
        "       sub v3.8b, v29.8b, v2.8b                    \n" \
        "       ld1 {v30.s}[0], [x7], #4                    \n" \
                RDNC_C3_LD_LINE0_LAN(10, 0, v30.s[0])           \
                MUL(v4, v7, v2, v3)                           \
                RDNC_C3_LD_LINE1_LAN(10, 0)                     \
                MUL(v5, v8, v2, v3)                           \
                MUL(v6, v9, v2, v3)                           \
                MUL(v10, v13, v2, v3)                         \
                MUL(v11, v14, v2, v3)                         \
                MUL(v12, v15, v2, v3)                         \
                ST3_LAN1_OFS(x1, v10, v11, v12)               \
                MUL(v4, v7, v0, v1)                           \
                MUL(v5, v8, v0, v1)                           \
                MUL(v6, v9, v0, v1)                           \
                ST3_LAN1_OFS(%1, v4, v5, v6)                  \
        "   b RDNC_C3_H_PRE                                 \n" \
        "RDNC_C3_CACHE:                                     \n" \
        "   cbz x5, RDNC_C3_CW4                             \n" \
        "   RDNC_C3_CW8:                                    \n" \
        "       ld1 {v2.8b}, [x8], #8                       \n" \
        "       sub v3.8b, v29.8b, v2.8b                    \n" \
        "       ld1 {v30.4s, v31.4s}, [x7], #32             \n" \
        "       subs x5, x5, #8                             \n" \
        "       ld3 {v10.8b, v11.8b, v12.8b}, [x1]          \n" \
                RDNC_C3_LD_LINE0_LAN(10, 0, v30.s[0])           \
                RDNC_C3_LD_LINE0_LAN(11, 1, v30.s[1])           \
                RDNC_C3_LD_LINE0_LAN(12, 2, v30.s[2])           \
                RDNC_C3_LD_LINE0_LAN(13, 3, v30.s[3])           \
                RDNC_C3_LD_LINE0_LAN(14, 4, v31.s[0])           \
                RDNC_C3_LD_LINE0_LAN(15, 5, v31.s[1])           \
                RDNC_C3_LD_LINE0_LAN(16, 6, v31.s[2])           \
                RDNC_C3_LD_LINE0_LAN(17, 7, v31.s[3])           \
                MUL(v4, v7, v2, v3)                           \
                MUL(v5, v8, v2, v3)                           \
                MUL(v6, v9, v2, v3)                           \
        "       st3 {v4.8b, v5.8b, v6.8b}, [x1], #24        \n" \
                MUL(v10, v4, v0, v1)                          \
                MUL(v11, v5, v0, v1)                          \
                MUL(v12, v6, v0, v1)                          \
        "       st3 {v10.8b, v11.8b, v12.8b}, [%1], #24     \n" \
        "       bgt RDNC_C3_CW8                             \n" \
        "   RDNC_C3_CW4:                                    \n" \
        "       cmp x6, #4                                  \n" \
        "       blt RDNC_C3_CW2                             \n" \
        "       ld1 {v2.s}[0], [x8], #4                     \n" \
        "       sub v3.8b, v29.8b, v2.8b                    \n" \
        "       ld1 {v30.4s}, [x7], #16                     \n" \
                LD3_LAN4(x1, v10, v11, v12)                   \
                RDNC_C3_LD_LINE0_LAN(10, 0, v30.s[0])           \
                RDNC_C3_LD_LINE0_LAN(11, 1, v30.s[1])           \
                RDNC_C3_LD_LINE0_LAN(12, 2, v30.s[2])           \
                RDNC_C3_LD_LINE0_LAN(13, 3, v30.s[3])           \
                MUL(v4, v7, v2, v3)                           \
                MUL(v5, v8, v2, v3)                           \
                MUL(v6, v9, v2, v3)                           \
                ST3_LAN4_OFS(x1, v4, v5, v6)                  \
                MUL(v10, v4, v0, v1)                          \
                MUL(v11, v5, v0, v1)                          \
                MUL(v12, v6, v0, v1)                          \
                ST3_LAN4_OFS(%1, v10, v11, v12)               \
        "       sub x6, x6, #4                              \n" \
        "   RDNC_C3_CW2:                                    \n" \
        "       cmp x6, #2                                  \n" \
        "       blt RDNC_C3_CW1                             \n" \
        "       ld1 {v2.h}[0], [x8], #2                     \n" \
        "       sub v3.8b, v29.8b, v2.8b                    \n" \
        "       ld1 {v30.1d}, [x7], #8                      \n" \
                LD3_LAN2(x1, v10, v11, v12)                   \
                RDNC_C3_LD_LINE0_LAN(10, 0, v30.s[0])           \
                RDNC_C3_LD_LINE0_LAN(11, 1, v30.s[1])           \
                MUL(v4, v7, v2, v3)                           \
                MUL(v5, v8, v2, v3)                           \
                MUL(v6, v9, v2, v3)                           \
                ST3_LAN2_OFS(x1, v4, v5, v6)                  \
                MUL(v10, v4, v0, v1)                          \
                MUL(v11, v5, v0, v1)                          \
                MUL(v12, v6, v0, v1)                          \
                ST3_LAN2_OFS(%1, v10, v11, v12)               \
        "       sub x6, x6, #2                              \n" \
        "   RDNC_C3_CW1:                                    \n" \
        "       cmp x6, #1                                  \n" \
        "       blt RDNC_C3_H_PRE                           \n" \
        "       ld1 {v2.b}[0], [x8], #1                     \n" \
        "       sub v3.8b, v29.8b, v2.8b                    \n" \
        "       ld1 {v30.s}[0], [x7], #4                    \n" \
                LD3_LAN1(x1, v10, v11, v12)                   \
                RDNC_C3_LD_LINE0_LAN(10, 0, v30.s[0])           \
                MUL(v4, v7, v2, v3)                           \
                MUL(v5, v8, v2, v3)                           \
                MUL(v6, v9, v2, v3)                           \
                ST3_LAN1_OFS(x1, v4, v5, v6)                  \
                MUL(v10, v4, v0, v1)                          \
                MUL(v11, v5, v0, v1)                          \
                MUL(v12, v6, v0, v1)                          \
                ST3_LAN1_OFS(%1, v10, v11, v12)               \
        "   b RDNC_C3_H_PRE                                 \n" \
        "RDNC_C3_END:                                       \n"
    :
    : "r"(src.data()), "r"(dst.data()), [w]"r"(1UL * dst.cols()), [h]"r"(1UL * dst.rows())
    , [ss]"r"(src.stride()), [beta]"r"(buff_h.getParam()), [start_h]"r"(buff_h.getStart())
    , [alpha]"r"(buff_w.getParam()), [start_w]"r"(buff_w.getStart())
    , [cache]"r"(cache.data()), [shift]"i"(k_shift)
    : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9"
    , "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17"
    , "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11"
    , "v12", "v13", "v14", "v15", "v29", "v30", "v31");
    // clang-format on
    #undef RDNC_C3_LD_LINE0_LAN
    #undef RDNC_C3_LD_LINE1_LAN
}

static void resize_c1(const Image& src, const Image &dst, ResizeParam param) {
    resize_dn2_c1(src, dst, param);
}

static void resize_c3(const Image& src, const Image &dst, ResizeParam param){
    auto sw = src.cols();
    auto sh = src.rows();
    auto dw = dst.cols();
    auto dh = dst.rows();

    if ((dh * 2) == sh) {
        if ((dw * 2) == sw) {
            resize_dn2_c3(src, dst, param);
            return;
        }
        resize_dn2h_c3(src, dst, param);
        return;
    }
    if ((dw * 2) == sw) {
        resize_dn2w_c3(src, dst, param);
        return;
    }
    resize_comm_c3(src, dst, param);
}

void resize(const Image& src, const Image &dst, ResizeParam param) {
    switch (src.fmt()) {
        case ImageFormat::BGR:
        case ImageFormat::RGB:
            resize_c3(src, dst, param);
            break;
        case ImageFormat::GRAY:
            resize_c1(src, dst, param);
            break;
        default:
            break;
    }
}

#undef ST3_LAN1_OFS
#undef ST3_LAN2_OFS
#undef ST3_LAN4_OFS
#undef LD3_LAN1_OFS
#undef LD3_LAN2_OFS
#undef LD3_LAN4_OFS
#undef ST3_LAN1
#undef ST3_LAN2
#undef ST3_LAN4
#undef LD3_LAN1
#undef LD3_LAN2
#undef LD3_LAN4
#undef MUL
#undef ADDLP

NAMESPACE_END
