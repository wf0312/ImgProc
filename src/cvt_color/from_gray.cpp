#include "image.hpp"
#include "types.hpp"
#include <algorithm>
#include "cvt_color.hpp"
#include "rgb.hpp"

NAMESPACE_BEGIN

constexpr int64_t k_pixel_size = 1L;
constexpr int64_t k_double_pixel_size = k_pixel_size << 1;

void gray_to_gray_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    std::copy_n(src_buf, size, dst_buf);
}

void gray_to_rgba_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    for (auto i = 0L, j = 0L; i < size; i += k_pixel_size, j += 4L) {
        dst_buf[j + 0] = src_buf[i];
        dst_buf[j + 1] = src_buf[i];
        dst_buf[j + 2] = src_buf[i];
        dst_buf[j + 3] = k_alpha;
    }
}

void gray_to_rgb_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    for (auto i = 0L, j = 0L; i < size; i += k_pixel_size, j += 3L) {
        dst_buf[j]     = src_buf[i];
        dst_buf[j + 1] = src_buf[i];
        dst_buf[j + 2] = src_buf[i];
    }
}

void gray_to_bgra_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    for (auto i = 0L, j = 0L; i < size; i += k_pixel_size, j += 4L) {
        dst_buf[j]     = src_buf[i];
        dst_buf[j + 1] = src_buf[i];
        dst_buf[j + 2] = src_buf[i];
        dst_buf[j + 3] = k_alpha;
    }
}

void gray_to_bgr_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    for (auto i = 0L, j = 0L; i < size; i += k_pixel_size, j += 3L) {
        dst_buf[j]     = src_buf[i];
        dst_buf[j + 1] = src_buf[i];
        dst_buf[j + 2] = src_buf[i];
    }
}

void gray_to_yuyv_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    constexpr auto k_offset = 1U << k_shift;
    for (auto i = 0L, j = 0L; i + k_double_pixel_size < size; i += k_double_pixel_size, j += 4) {
        auto y0 = src_buf[i];
        auto y1 = src_buf[i + 1];

        dst_buf[j + 0] = saturate_u8(y0);
        dst_buf[j + 1] = k_offset;
        dst_buf[j + 2] = saturate_u8(y1);
        dst_buf[j + 3] = k_offset;
    }
}

void gray_to_uyvy_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();
    constexpr auto k_offset = 1U << k_shift;

    for (auto i = 0L, j = 0L; i + k_double_pixel_size < size; i += k_double_pixel_size, j += 4) {
        auto y0 = src_buf[i];
        auto y1 = src_buf[i + 1];

        dst_buf[j + 0] = saturate_u8(y0);
        dst_buf[j + 1] = k_offset;
        dst_buf[j + 2] = saturate_u8(y1);
        dst_buf[j + 3] = k_offset;
    }
}

void gray_to_i420_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    auto w = src.cols();
    auto h = src.rows();
    if (0 != h % 2)
    {
        throw std::invalid_argument("Height must be even");
    }

    auto ss   = src.stride();
    auto ys   = 1L * w;

    auto src0 = src_buf;
    auto src1 = src_buf + ss;

    auto y0 = dst_buf;
    auto y1 = dst_buf + ys;

    auto u   = dst_buf + (static_cast<int64_t>(w) * h);
    auto v   = u + ((static_cast<int64_t>(w) * h) >> 2);
    auto uvs = static_cast<int64_t>(w) >> 1;

    constexpr auto k_offset        = 1U << k_shift;

    for (int32_t i = 0; i + 2 < h; i += 2) {
        for (int64_t j = 0, k = 0; j + k_double_pixel_size < ss; j += k_double_pixel_size, k += 2) {
            y0[k + 0] = src0[j];
            y0[k + 1] = src0[j + 1];
            y1[k + 0] = src1[j];
            y1[k + 1] = src1[j + 1];

            u[(k >> 1)] = k_offset;
            v[(k >> 1)] = k_offset;
        }
        src0 += (ss << 1);
        src1 += (ss << 1);
        y0 += (ys << 1);
        y1 += (ys << 1);
        u += uvs;
        v += uvs;
    }
}

void gray_to_nv12_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    auto w = src.cols();
    auto h = src.rows();
    if (0 != h % 2)
    {
        throw std::invalid_argument("Height must be even");
    }
    auto ss   = src.stride();
    auto ys   = 1L * w;

    auto src0 = src_buf;
    auto src1 = src_buf + ss;

    auto y0 = dst_buf;
    auto y1 = dst_buf + ys;

    auto uv   = dst_buf + (static_cast<int64_t>(w) * h);
    auto uvs = static_cast<int64_t>(w);

    constexpr auto k_offset        = 1U << k_shift;

    for (int32_t i = 0; i + 2 < h; i += 2) {
        for (int64_t j = 0, k = 0; j + k_double_pixel_size < ss; j += k_double_pixel_size, k += 2) {
            y0[k + 0] = src0[j];
            y0[k + 1] = src0[j + 1];
            y1[k + 0] = src1[j];
            y1[k + 1] = src1[j + 1];

            uv[k + 0] = k_offset;
            uv[k + 1] = k_offset;
        }
        src0 += (ss << 1);
        src1 += (ss << 1);
        y0 += (ys << 1);
        y1 += (ys << 1);
        uv += uvs;
    }
}

void gray_to_nv21_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    auto w = src.cols();
    auto h = src.rows();
    if (0 != h % 2)
    {
        throw std::invalid_argument("Height must be even");
    }

    auto ss   = src.stride();
    auto ys   = 1L * w;

    auto src0 = src_buf;
    auto src1 = src_buf + ss;

    auto y0 = dst_buf;
    auto y1 = dst_buf + ys;

    auto uv   = dst_buf + (static_cast<int64_t>(w) * h);
    auto uvs = static_cast<int64_t>(w) >> 1;

    constexpr auto k_offset        = 1U << k_shift;

    for (int32_t i = 0; i + 2 < h; i += 2) {
        for (int64_t j = 0, k = 0; j + k_double_pixel_size < ss; j += k_double_pixel_size, k += 2) {
            y0[k + 0] = src0[j];
            y0[k + 1] = src0[j + 1];
            y1[k + 0] = src1[j];
            y1[k + 1] = src1[j + 1];

            uv[k + 1] = k_offset;
            uv[k + 0] = k_offset;
        }
        src0 += (ss << 1);
        src1 += (ss << 1);
        y0 += (ys << 1);
        y1 += (ys << 1);
        uv += uvs;
    }
}

void gray_to_gray(const Image &src, const Image &dst)
{
    asm volatile(
        LOAD_Y_PARAM
        R"(
            and x0, %1, #15
            sub %1, %1, x0
            cbz %1, 2f
        1:
            ld1 {v0.16b}, [%0], #16
            prfm pldl1keep, [%0, 448]
            st1 {v0.16b}, [%2], #16
            subs %1, %1, #16
            bgt 1b
        2:
            cmp x0, #1
            blt 3f
            ld1 {v0.b}[0], [%0], #1
            st1 {v0.b}[1], [%2], #1
            sub x0, x0, #1
            b 2b
        3:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [param] "r"(&k_rgb_2_yuv), [shift] "i"(k_shift)
        : "memory", "cc", "x0"
        , "v0", "v1", "v2", "v3", USED_Y_REG);
}

void gray_to_rgba(const Image &src, const Image &dst)
{
    asm volatile(
        R"(
            and x0, %1, #15
            sub %1, %1, x0
            movi v3.16b, %[alpha]
            cbz %1, 2f
        1:
            ld1 {v0.16b}, [%0], #16
            mov v1.16b, v0.16b
            subs %1, %1, #16
            mov v2.16b, v0.16b
            prfm pldl1keep, [%0, 448]
            st4 {v0.16b, v1.16b, v2.16b, v3.16b}, [%2], #64
            bgt 1b
        2:
            cmp x0, #1
            blt 3f
            ld1 {v0.b}[0], [%0], #1
            mov v1.16b, v0.16b
            mov v2.16b, v0.16b
            st4 {v0.b, v1.b, v2.b, v3.b}[0], [%2], #4
            sub x0, x0, #1
            b 2b
        3:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [alpha] "I"(k_alpha)
        : "cc", "memory", "x0", "v0", "v1", "v2", "v3", "v4");
}

void gray_to_rgb(const Image &src, const Image &dst)
{
    asm volatile(
        R"(
            and x0, %1, #15
            sub %1, %1, x0
            cbz %1, 2f
        1:
            ld1 {v0.16b}, [%0], #16
            mov v1.16b, v0.16b
            subs %1, %1, #16
            mov v2.16b, v0.16b
            prfm pldl1keep, [%0, 448]
            st3 {v0.16b, v1.16b, v2.16b}, [%2], #48
            bgt 1b
        2:
            cmp x0, #1
            blt 3f
            ld1 {v0.b}[0], [%0], #1
            mov v1.16b, v0.16b
            sub x0, x0, #1
            mov v2.16b, v0.16b
            st3 {v0.b, v1.b, v2.b}[0], [%2], #3
            b 2b
        3:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        : "memory", "cc", "x0", "v0", "v1", "v2");
}

void gray_to_bgr(const Image &src, const Image &dst)
{
    asm volatile(
        R"(
            and x0, %1, #15
            sub %1, %1, x0
            cbz %1, 2f
        1:
            ld1 {v0.16b}, [%0], #16
            prfm pldl1keep, [%0, 448]
            mov v1.16b, v0.16b
            subs %1, %1, #16
            mov v2.16b, v0.16b
            st3 {v0.16b, v1.16b, v2.16b}, [%2], #48
            bgt 1b
        2:
            cmp       x0, #1
            blt       3f
            ld1       {v0.b}[0], [%0], #1
            mov v1.16b, v0.16b
            sub       x0, x0, #1
            mov v2.16b, v0.16b
            st3       {v0.b, v1.b, v2.b}[0], [%2], #3
            b 2b
        3:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        : "memory", "cc", "x0", "v0", "v1", "v2");
}

void gray_to_bgra(const Image &src, const Image &dst)
{
    asm volatile(
        R"(
            movi v3.16b, %[alpha]
            and x0, %1, #15
            sub %1, %1, x0
            cbz %1, 2f
        1:
            ld1 {v0.16b}, [%0], #16
            mov v1.16b, v0.16b
            subs %1, %1, #16
            mov v2.16b, v0.16b
            prfm pldl1keep, [%0, 448]
            st4 {v0.16b, v1.16b, v2.16b, v3.16b}, [%2], #64
            bgt 1b
        2:
            cmp x0, #1
            blt 3f
            ld1 {v0.b}[0], [%0], #1
            mov v1.16b, v0.16b
            sub x0, x0, #1
            mov v2.16b, v0.16b
            st4 {v0.b, v1.b, v2.b, v3.b}[0], [%2], #4
            b 2b
        3:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [alpha] "I"(k_alpha)
        : "memory", "cc", "x0", "v0", "v1", "v2", "v3");
}

void gray_to_yuyv(const Image &src, const Image &dst)
{
    asm volatile(
        LOAD_YUV_PARAM
        R"(
            movi v1.16b, #128
            and x0, %1, #7
            sub %1, %1, x0
            cbz %1, 2f
        1:
            ld1 {v0.8b}, [%0], #8
            prfm pldl1keep, [%0, 448]
            subs %1, %1, #8
            st2 {v0.8b, v1.8b}, [%2], #16
            bgt 1b
        2:
            cmp x0, #2
            blt 3f
            ld1 {v0.b}[0], [%0], #1
            ld1 {v0.b}[1], [%0], #1
            sub x0, x0, #2
            st2 {v0.b, v1.b}[0], [%2], #2
            st2 {v0.b, v1.b}[1], [%2], #2
            b 2b
        3:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [param] "r"(&k_rgb_2_yuv), [shift] "i"(k_shift)
        : "cc", "memory", "x0"
        , "v0", "v1", "v2", "v3", "v4", "v5", "v23", USED_YUV_REG);
}

void gray_to_uyvy(const Image &src, const Image &dst)
{
    asm volatile(
        LOAD_YUV_PARAM
        R"(
            movi v0.16b, #128
            uaddlp v23.8h, v23.16b
            shl v23.8h, v23.8h, %[shift]
            and x0, %1, #7
            sub %1, %1, x0
            cbz %1, 2f
        1:
            ld1 {v1.8b}, [%0], #8
            prfm pldl1keep, [%0, 448]
            subs %1, %1, #8
            st2 {v0.8b, v1.8b}, [%2], #16
            bgt 1b
        2:
            cmp x0, #2
            blt 3f
            ld1 {v0.b}[0], [%0], #1
            ld1 {v0.b}[1], [%0], #1
            sub x0, x0, #2
            st2 {v0.b, v1.b}[0], [%2], #2
            st2 {v0.b, v1.b}[1], [%2], #2
            b 2b
        3:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [param] "r"(&k_rgb_2_yuv), [shift] "i"(k_shift)
        : "cc", "memory", "x0"
        , "v0", "v1", "v2", "v3", "v4", "v5", "v23", USED_YUV_REG);
}

void gray_to_i420(const Image &src, const Image &dst)
{
    auto w = src.cols();
    auto h = src.rows();
    if (0 != w % 2)
    {
        throw std::invalid_argument("Width must be even");
    }
    if (0 != h % 2)
    {
        throw std::invalid_argument("Height must be even");
    }
    auto y    = dst.data();
    auto u    = y + dst.pixels();
    auto v    = u + (dst.pixels() >> 2);
    asm volatile(
        LOAD_YUV_PARAM
        R"(
            movi v2.16b, #128
            mov x1, %0
            mov x3, %1
        1:
            cmp %[h], #2
            blt 4f
            sub %[h], %[h], #2
            mov x0, x1
            add x1, x1, %[ss]
            mov x2, x3
            add x3, x3, %[w]
            and x5, %[w], #7
            sub x4, %[w], x5
            cbz x4, 3f
        2:
            ld1 {v0.8b}, [x0], #8
            ld1 {v1.8b}, [x1], #8
            prfm pldl1keep, [x0, 448]
            prfm pldl1keep, [x1, 448]
            st1 {v0.8b}, [x2], #8
            st1 {v1.8b}, [x3], #8
            st1 {v2.s}[0], [%2], #4
            subs x4, x4, #8
            st1 {v2.s}[0], [%3], #4
            bgt 2b
        3:
            cmp x5, #2
            blt 1b
            ld1 {v0.h}[0], [x0], #2
            ld1 {v1.h}[0], [x1], #2
            sub x5, x5, #2
            st1 {v0.h}[0], [x2], #2
            st1 {v1.h}[0], [x3], #2
            st1 {v2.b}[0], [%2], #1
            st1 {v2.b}[0], [%3], #1
            b 3b
        4:
        )"
        :
        : "r"(src.data()), "r"(y), "r"(u), "r"(v)
        , [ss]"r"(src.stride()), [h] "r"(1UL * h), [w] "r"(1UL * w)
        , [param] "r"(&k_rgb_2_yuv), [shift] "i"(k_shift)
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5", USED_YUV_REG, "v1", "v2", "v3", "v4", "v5", "v6", "v7");
}

void gray_to_nv12(const Image &src, const Image &dst)
{
    auto w = src.cols();
    auto h = src.rows();
    if (0 != w % 2)
    {
        throw std::invalid_argument("Width must be even");
    }
    if (0 != h % 2)
    {
        throw std::invalid_argument("Height must be even");
    }
    auto y    = dst.data();
    auto uv   = y + dst.pixels();
    asm volatile(
        LOAD_YUV_PARAM
        R"(
            movi v2.16b, #128
            mov x1, %0
            mov x3, %1
        1:
            cmp %[h], #2
            blt 4f
            sub %[h], %[h], #2
            mov x0, x1
            add x1, x1, %[ss]
            mov x2, x3
            add x3, x3, %[w]
            and x5, %[w], #7
            sub x4, %[w], x5
            cbz x4, 3f
        2:
            ld1 {v0.8b}, [x0], #8
            ld1 {v1.8b}, [x1], #8
            prfm pldl1keep, [x0, 448]
            prfm pldl1keep, [x1, 448]
            st1 {v0.8b}, [x2], #8
            st1 {v1.8b}, [x3], #8
            st1 {v2.8b}, [%2], #8
            subs x4, x4, #8
            bgt 2b
        3:
            cmp x5, #2
            blt 1b
            ld1 {v0.h}[0], [x0], #2
            ld1 {v1.h}[0], [x1], #2
            sub x5, x5, #2
            st1 {v0.h}[0], [x2], #2
            st1 {v1.h}[0], [x3], #2
            st1 {v2.h}[0], [%2], #2
            b 3b
        4:
        )"
        :
        : "r"(src.data()), "r"(y), "r"(uv)
        , [ss]"r"(src.stride()), [h] "r"(1UL * h), [w] "r"(1UL * w)
        , [param] "r"(&k_rgb_2_yuv), [shift] "i"(k_shift)
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5", USED_YUV_REG, "v1", "v2", "v3", "v4", "v5", "v6", "v7");
}

void gray_to_nv21(const Image &src, const Image &dst)
{
    auto w = src.cols();
    auto h = src.rows();
    if (0 != w % 2)
    {
        throw std::invalid_argument("Width must be even");
    }
    if (0 != h % 2)
    {
        throw std::invalid_argument("Height must be even");
    }
    auto y    = dst.data();
    auto uv   = y + dst.pixels();
    asm volatile(
        LOAD_YUV_PARAM
        R"(
            movi v2.16b, #128
            mov x1, %0
            mov x3, %1
        1:
            cmp %[h], #2
            blt 4f
            sub %[h], %[h], #2
            mov x0, x1
            add x1, x1, %[ss]
            mov x2, x3
            add x3, x3, %[w]
            and x5, %[w], #7
            sub x4, %[w], x5
            cbz x4, 3f
        2:
            ld1 {v0.8b}, [x0], #8
            ld1 {v1.8b}, [x1], #8
            prfm pldl1keep, [x0, 448]
            prfm pldl1keep, [x1, 448]
            st1 {v0.8b}, [x2], #8
            st1 {v1.8b}, [x3], #8
            st1 {v2.8b}, [%2], #8
            subs x4, x4, #8
            bgt 2b
        3:
            cmp x5, #2
            blt 1b
            ld1 {v0.h}[0], [x0], #2
            ld1 {v1.h}[0], [x1], #2
            sub x5, x5, #2
            st1 {v0.h}[0], [x2], #2
            st1 {v1.h}[0], [x3], #2
            st1 {v2.h}[0], [%2], #2
            b 3b
        4:
        )"
        :
        : "r"(src.data()), "r"(y), "r"(uv)
        , [ss]"r"(src.stride()), [h] "r"(1UL * h), [w] "r"(1UL * w)
        , [param] "r"(&k_rgb_2_yuv), [shift] "i"(k_shift)
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5", USED_YUV_REG, "v1", "v2", "v3", "v4", "v5", "v6", "v7");
}
NAMESPACE_END
