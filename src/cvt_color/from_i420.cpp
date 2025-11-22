#include "image.hpp"
#include "types.hpp"
#include <algorithm>
#include "cvt_color.hpp"
#include "rgb.hpp"

NAMESPACE_BEGIN

constexpr int64_t k_pixel_size = 3L;
void i420_to_gray_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    for (auto i = 0L, j = 0L; i < size; i += k_pixel_size, j += 1L) {
        dst_buf[j] = (src_buf[i + 2] * k_rgb_2_yuv.m_yr + src_buf[i + 1] * k_rgb_2_yuv.m_yg + src_buf[i + 0] * k_rgb_2_yuv.m_yb) >> k_shift;
    }
}

void i420_to_rgba_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    for (auto i = 0L, j = 0L; i < size; i += k_pixel_size, j += 4L) {
        dst_buf[j + 0] = src_buf[i + 2];
        dst_buf[j + 1] = src_buf[i + 1];
        dst_buf[j + 2] = src_buf[i + 0];
        dst_buf[j + 3] = k_alpha;
    }
}

void i420_to_rgb_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    for (auto i = 0L, j = 0L; i < size; i += k_pixel_size, j += 3L) {
        dst_buf[j]     = src_buf[i + 2];
        dst_buf[j + 1] = src_buf[i + 1];
        dst_buf[j + 2] = src_buf[i];
    }
}

void i420_to_bgra_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    for (auto i = 0L, j = 0L; i < size; i += k_pixel_size, j += 4L) {
        dst_buf[j]     = src_buf[i + 0];
        dst_buf[j + 1] = src_buf[i + 1];
        dst_buf[j + 2] = src_buf[i + 2];
        dst_buf[j + 3] = k_alpha;
    }
}

void i420_to_bgr_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    std::copy_n(src_buf, size, dst_buf);
}

void i420_to_yuyv_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    constexpr auto k_offset = 1U << k_shift;
    for (auto i = 0L, j = 0L; i + 6 < size; i += 6, j += 4) {
        auto y0 = (src_buf[i + 2] * k_rgb_2_yuv.m_yr + src_buf[i + 1] * k_rgb_2_yuv.m_yg + src_buf[i + 0] * k_rgb_2_yuv.m_yb) >> k_shift;
        auto y1 = (src_buf[i + 5] * k_rgb_2_yuv.m_yr + src_buf[i + 4] * k_rgb_2_yuv.m_yg + src_buf[i + 3] * k_rgb_2_yuv.m_yb) >> k_shift;
        auto u0 = (src_buf[i + 0] * k_rgb_2_yuv.m_ub - src_buf[i + 2] * k_rgb_2_yuv.m_ur - src_buf[i + 1] * k_rgb_2_yuv.m_ug) >> k_shift;
        auto u1 = (src_buf[i + 3] * k_rgb_2_yuv.m_ub - src_buf[i + 5] * k_rgb_2_yuv.m_ur - src_buf[i + 4] * k_rgb_2_yuv.m_ug) >> k_shift;
        auto v0 = (src_buf[i + 2] * k_rgb_2_yuv.m_vr - src_buf[i + 1] * k_rgb_2_yuv.m_vg - src_buf[i + 0] * k_rgb_2_yuv.m_vb) >> k_shift;
        auto v1 = (src_buf[i + 5] * k_rgb_2_yuv.m_vr - src_buf[i + 4] * k_rgb_2_yuv.m_vg - src_buf[i + 3] * k_rgb_2_yuv.m_vb) >> k_shift;

        dst_buf[j + 0] = saturate_u8(y0);
        dst_buf[j + 1] = saturate_u8(((u0 + u1) >> 1) + k_offset);
        dst_buf[j + 2] = saturate_u8(y1);
        dst_buf[j + 3] = saturate_u8(((v0 + v1) >> 1) + k_offset);
    }
}

void i420_to_uyvy_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();
    constexpr auto k_offset = 1U << k_shift;

    for (auto i = 0L, j = 0L; i + 6 < size; i += 6, j += 4) {
        auto y0 = (src_buf[i + 2] * k_rgb_2_yuv.m_yr + src_buf[i + 1] * k_rgb_2_yuv.m_yg + src_buf[i + 0] * k_rgb_2_yuv.m_yb) >> k_shift;
        auto y1 = (src_buf[i + 5] * k_rgb_2_yuv.m_yr + src_buf[i + 4] * k_rgb_2_yuv.m_yg + src_buf[i + 3] * k_rgb_2_yuv.m_yb) >> k_shift;
        auto u0 = (src_buf[i + 0] * k_rgb_2_yuv.m_ub - src_buf[i + 2] * k_rgb_2_yuv.m_ur - src_buf[i + 1] * k_rgb_2_yuv.m_ug) >> k_shift;
        auto u1 = (src_buf[i + 3] * k_rgb_2_yuv.m_ub - src_buf[i + 5] * k_rgb_2_yuv.m_ur - src_buf[i + 4] * k_rgb_2_yuv.m_ug) >> k_shift;
        auto v0 = (src_buf[i + 2] * k_rgb_2_yuv.m_vr - src_buf[i + 1] * k_rgb_2_yuv.m_vg - src_buf[i + 0] * k_rgb_2_yuv.m_vb) >> k_shift;
        auto v1 = (src_buf[i + 5] * k_rgb_2_yuv.m_vr - src_buf[i + 4] * k_rgb_2_yuv.m_vg - src_buf[i + 3] * k_rgb_2_yuv.m_vb) >> k_shift;

        dst_buf[j + 0] = saturate_u8(((u0 + u1) >> 1) + k_offset);
        dst_buf[j + 1] = saturate_u8(y0);
        dst_buf[j + 2] = saturate_u8(((v0 + v1) >> 1) + k_offset);
        dst_buf[j + 3] = saturate_u8(y1);
    }
}

void i420_to_i420_c(const Image &src, const Image &dst)
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
        for (int64_t j = 0, k = 0; j + 6 < ss; j += 6, k += 2) {
            auto y00 = (src0[j + 2] * k_rgb_2_yuv.m_yr + src0[j + 1] * k_rgb_2_yuv.m_yg + src0[j + 0] * k_rgb_2_yuv.m_yb) >> k_shift;
            auto y01 = (src0[j + 5] * k_rgb_2_yuv.m_yr + src0[j + 4] * k_rgb_2_yuv.m_yg + src0[j + 3] * k_rgb_2_yuv.m_yb) >> k_shift;
            auto y10 = (src1[j + 2] * k_rgb_2_yuv.m_yr + src1[j + 1] * k_rgb_2_yuv.m_yg + src1[j + 0] * k_rgb_2_yuv.m_yb) >> k_shift;
            auto y11 = (src1[j + 5] * k_rgb_2_yuv.m_yr + src1[j + 4] * k_rgb_2_yuv.m_yg + src1[j + 3] * k_rgb_2_yuv.m_yb) >> k_shift;
            auto u00 = (src0[j + 0] * k_rgb_2_yuv.m_ub - src0[j + 2] * k_rgb_2_yuv.m_ur - src0[j + 1] * k_rgb_2_yuv.m_ug) >> k_shift;
            auto u01 = (src0[j + 3] * k_rgb_2_yuv.m_ub - src0[j + 5] * k_rgb_2_yuv.m_ur - src0[j + 4] * k_rgb_2_yuv.m_ug) >> k_shift;
            auto u10 = (src1[j + 0] * k_rgb_2_yuv.m_ub - src1[j + 2] * k_rgb_2_yuv.m_ur - src1[j + 1] * k_rgb_2_yuv.m_ug) >> k_shift;
            auto u11 = (src1[j + 3] * k_rgb_2_yuv.m_ub - src1[j + 5] * k_rgb_2_yuv.m_ur - src1[j + 4] * k_rgb_2_yuv.m_ug) >> k_shift;
            auto v00 = (src0[j + 2] * k_rgb_2_yuv.m_vr - src0[j + 1] * k_rgb_2_yuv.m_vg - src0[j + 0] * k_rgb_2_yuv.m_vb) >> k_shift;
            auto v01 = (src0[j + 5] * k_rgb_2_yuv.m_vr - src0[j + 4] * k_rgb_2_yuv.m_vg - src0[j + 3] * k_rgb_2_yuv.m_vb) >> k_shift;
            auto v10 = (src1[j + 2] * k_rgb_2_yuv.m_vr - src1[j + 1] * k_rgb_2_yuv.m_vg - src1[j + 0] * k_rgb_2_yuv.m_vb) >> k_shift;
            auto v11 = (src1[j + 5] * k_rgb_2_yuv.m_vr - src1[j + 4] * k_rgb_2_yuv.m_vg - src1[j + 3] * k_rgb_2_yuv.m_vb) >> k_shift;
            y0[k + 0] = y00;
            y0[k + 1] = y01;
            y1[k + 0] = y10;
            y1[k + 1] = y11;

            u[(k >> 1)] = saturate_u8(((u00 + u01 + u10 + u11) >> 2) + k_offset);
            v[(k >> 1)] = saturate_u8(((v00 + v01 + v10 + v11) >> 2) + k_offset);
        }
        src0 += (ss << 1);
        src1 += (ss << 1);
        y0 += (ys << 1);
        y1 += (ys << 1);
        u += uvs;
        v += uvs;
    }
}

void i420_to_nv12_c(const Image &src, const Image &dst)
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
        for (int64_t j = 0, k = 0; j + 6 < ss; j += 6, k += 2) {
            auto y00 = (src0[j + 2] * k_rgb_2_yuv.m_yr + src0[j + 1] * k_rgb_2_yuv.m_yg + src0[j + 0] * k_rgb_2_yuv.m_yb) >> k_shift;
            auto y01 = (src0[j + 5] * k_rgb_2_yuv.m_yr + src0[j + 4] * k_rgb_2_yuv.m_yg + src0[j + 3] * k_rgb_2_yuv.m_yb) >> k_shift;
            auto y10 = (src1[j + 2] * k_rgb_2_yuv.m_yr + src1[j + 1] * k_rgb_2_yuv.m_yg + src1[j + 0] * k_rgb_2_yuv.m_yb) >> k_shift;
            auto y11 = (src1[j + 5] * k_rgb_2_yuv.m_yr + src1[j + 4] * k_rgb_2_yuv.m_yg + src1[j + 3] * k_rgb_2_yuv.m_yb) >> k_shift;
            auto u00 = (src0[j + 0] * k_rgb_2_yuv.m_ub - src0[j + 2] * k_rgb_2_yuv.m_ur - src0[j + 1] * k_rgb_2_yuv.m_ug) >> k_shift;
            auto u01 = (src0[j + 3] * k_rgb_2_yuv.m_ub - src0[j + 5] * k_rgb_2_yuv.m_ur - src0[j + 4] * k_rgb_2_yuv.m_ug) >> k_shift;
            auto u10 = (src1[j + 0] * k_rgb_2_yuv.m_ub - src1[j + 2] * k_rgb_2_yuv.m_ur - src1[j + 1] * k_rgb_2_yuv.m_ug) >> k_shift;
            auto u11 = (src1[j + 3] * k_rgb_2_yuv.m_ub - src1[j + 5] * k_rgb_2_yuv.m_ur - src1[j + 4] * k_rgb_2_yuv.m_ug) >> k_shift;
            auto v00 = (src0[j + 2] * k_rgb_2_yuv.m_vr - src0[j + 1] * k_rgb_2_yuv.m_vg - src0[j + 0] * k_rgb_2_yuv.m_vb) >> k_shift;
            auto v01 = (src0[j + 5] * k_rgb_2_yuv.m_vr - src0[j + 4] * k_rgb_2_yuv.m_vg - src0[j + 3] * k_rgb_2_yuv.m_vb) >> k_shift;
            auto v10 = (src1[j + 2] * k_rgb_2_yuv.m_vr - src1[j + 1] * k_rgb_2_yuv.m_vg - src1[j + 0] * k_rgb_2_yuv.m_vb) >> k_shift;
            auto v11 = (src1[j + 5] * k_rgb_2_yuv.m_vr - src1[j + 4] * k_rgb_2_yuv.m_vg - src1[j + 3] * k_rgb_2_yuv.m_vb) >> k_shift;

            uv[k + 0] = saturate_u8(((u00 + u01 + u10 + u11) >> 2) + k_offset);
            uv[k + 1] = saturate_u8(((v00 + v01 + v10 + v11) >> 2) + k_offset);
        }
        src0 += (ss << 1);
        src1 += (ss << 1);
        y0 += (ys << 1);
        y1 += (ys << 1);
        uv += uvs;
    }
}

void i420_to_nv21_c(const Image &src, const Image &dst)
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
        for (int64_t j = 0, k = 0; j + 6 < ss; j += 6, k += 2) {
            auto y00 = (src0[j + 2] * k_rgb_2_yuv.m_yr + src0[j + 1] * k_rgb_2_yuv.m_yg + src0[j + 0] * k_rgb_2_yuv.m_yb) >> k_shift;
            auto y01 = (src0[j + 5] * k_rgb_2_yuv.m_yr + src0[j + 4] * k_rgb_2_yuv.m_yg + src0[j + 3] * k_rgb_2_yuv.m_yb) >> k_shift;
            auto y10 = (src1[j + 2] * k_rgb_2_yuv.m_yr + src1[j + 1] * k_rgb_2_yuv.m_yg + src1[j + 0] * k_rgb_2_yuv.m_yb) >> k_shift;
            auto y11 = (src1[j + 5] * k_rgb_2_yuv.m_yr + src1[j + 4] * k_rgb_2_yuv.m_yg + src1[j + 3] * k_rgb_2_yuv.m_yb) >> k_shift;
            auto u00 = (src0[j + 0] * k_rgb_2_yuv.m_ub - src0[j + 2] * k_rgb_2_yuv.m_ur - src0[j + 1] * k_rgb_2_yuv.m_ug) >> k_shift;
            auto u01 = (src0[j + 3] * k_rgb_2_yuv.m_ub - src0[j + 5] * k_rgb_2_yuv.m_ur - src0[j + 4] * k_rgb_2_yuv.m_ug) >> k_shift;
            auto u10 = (src1[j + 0] * k_rgb_2_yuv.m_ub - src1[j + 2] * k_rgb_2_yuv.m_ur - src1[j + 1] * k_rgb_2_yuv.m_ug) >> k_shift;
            auto u11 = (src1[j + 3] * k_rgb_2_yuv.m_ub - src1[j + 5] * k_rgb_2_yuv.m_ur - src1[j + 4] * k_rgb_2_yuv.m_ug) >> k_shift;
            auto v00 = (src0[j + 2] * k_rgb_2_yuv.m_vr - src0[j + 1] * k_rgb_2_yuv.m_vg - src0[j + 0] * k_rgb_2_yuv.m_vb) >> k_shift;
            auto v01 = (src0[j + 5] * k_rgb_2_yuv.m_vr - src0[j + 4] * k_rgb_2_yuv.m_vg - src0[j + 3] * k_rgb_2_yuv.m_vb) >> k_shift;
            auto v10 = (src1[j + 2] * k_rgb_2_yuv.m_vr - src1[j + 1] * k_rgb_2_yuv.m_vg - src1[j + 0] * k_rgb_2_yuv.m_vb) >> k_shift;
            auto v11 = (src1[j + 5] * k_rgb_2_yuv.m_vr - src1[j + 4] * k_rgb_2_yuv.m_vg - src1[j + 3] * k_rgb_2_yuv.m_vb) >> k_shift;
            y0[k + 0] = y00;
            y0[k + 1] = y01;
            y1[k + 0] = y10;
            y1[k + 1] = y11;

            uv[k + 1] = saturate_u8(((u00 + u01 + u10 + u11) >> 2) + k_offset);
            uv[k + 0] = saturate_u8(((v00 + v01 + v10 + v11) >> 2) + k_offset);
        }
        src0 += (ss << 1);
        src1 += (ss << 1);
        y0 += (ys << 1);
        y1 += (ys << 1);
        uv += uvs;
    }
}

void i420_to_gray(const Image &src, const Image &dst)
{
    asm volatile(
        LOAD_Y_PARAM
        R"(
            and x0, %1, #15
            sub %1, %1, x0
            cbz %1, GRAY_L1

        GRAY_L16:
            ld3       {v0.16b, v1.16b, v2.16b}, [%0], #48
            umull     v3.8h, v2.8b, v29.8b
            umlal     v3.8h, v1.8b, v30.8b
            umlal     v3.8h, v0.8b, v31.8b
            prfm      pldl1keep, [%0, 448]
            uqshrn    v3.8b, v3.8h, %[shift]
            umull2    v2.8h, v2.16b, v29.16b
            umlal2    v2.8h, v1.16b, v30.16b
            umlal2    v2.8h, v0.16b, v31.16b
            uqshrn2   v3.16b, v2.8h, %[shift]
            st1       {v3.16b}, [%2], #16
            subs      %1, %1, #16
            bgt       GRAY_L16

        GRAY_L1:
            cmp       x0, #1
            blt       GRAY_END
            ld3       {v0.b, v1.b, v2.b}[0], [%0], #3
            umull     v3.8h, v2.8b, v29.8b    // * r
            umlal     v3.8h, v1.8b, v30.8b    // * g
            umlal     v3.8h, v0.8b, v31.8b    // * b
            uqshrn    v0.8b, v3.8h, %[shift]
            st1       {v0.b}[1], [%2], #1
            sub       x0, x0, #1
            b GRAY_L1

        GRAY_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [param] "r"(&k_rgb_2_yuv), [shift] "i"(k_shift)
        : "memory", "cc", "x0"
        , "v0", "v1", "v2", "v3", USED_Y_REG);
}

void i420_to_rgba(const Image &src, const Image &dst)
{
    asm volatile(
        R"(
            and x0, %1, #15
            sub %1, %1, x0
            movi v3.16b, %[alpha]
            cbz %1, RGBA_L1

        RGBA_L16:
            ld3 {v0.16b, v1.16b, v2.16b}, [%0], #48
            mov v4.16b, v0.16b
            mov v0.16b, v2.16b
            subs %1, %1, #16
            mov v2.16b, v4.16b
            prfm pldl1keep, [%0, 448]
            st4 {v0.16b, v1.16b, v2.16b, v3.16b}, [%2], #64
            bgt RGBA_L16

        RGBA_L1:
            cmp x0, #1
            blt RGBA_END
            ld3 {v0.b, v1.b, v2.b}[0], [%0], #3
            mov v4.16b, v0.16b
            mov v0.16b, v2.16b
            mov v2.16b, v4.16b
            st4 {v0.b, v1.b, v2.b, v3.b}[0], [%2], #4
            sub x0, x0, #1
            b RGBA_L1
        RGBA_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [alpha] "I"(k_alpha)
        : "cc", "memory", "x0", "v0", "v1", "v2", "v3", "v4");
}

void i420_to_rgb(const Image &src, const Image &dst)
{
    asm volatile(
        R"(
            and x0, %1, #15
            sub %1, %1, x0
            cbz %1, RGB_L1

        RGB_L16:
            ld3       {v0.16b, v1.16b, v2.16b}, [%0], #48
            mov v3.16b, v0.16b
            mov v0.16b, v2.16b
            subs      %1, %1, #16
            mov v2.16b, v3.16b
            prfm      pldl1keep, [%0, 448]
            st3       {v0.16b, v1.16b, v2.16b}, [%2], #48
            bgt       RGB_L16

        RGB_L1:
            cmp       x0, #1
            blt       RGB_END
            ld3       {v0.b, v1.b, v2.b}[0], [%0], #3
            mov v3.16b, v0.16b
            mov v0.16b, v2.16b
            sub       x0, x0, #1
            mov v2.16b, v3.16b
            st3       {v0.b, v1.b, v2.b}[0], [%2], #3

        RGB_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        : "memory", "cc", "x0", "v0", "v1", "v2");
}

void i420_to_bgr(const Image &src, const Image &dst)
{
    asm volatile(
        R"(
            and x0, %1, #15
            sub %1, %1, x0
            cbz %1, BGR_L1

        BGR_L16:
            ld3 {v0.16b, v1.16b, v2.16b}, [%0], #48
            prfm pldl1keep, [%0, #48]
            subs %1, %1, #16
            st3 {v0.16b, v1.16b, v2.16b}, [%2], #48
            bgt BGR_L16

        BGR_L1:
            cmp       x0, #1
            blt       BGR_END
            ld3       {v0.b, v1.b, v2.b}[0], [%0], #3
            sub       x0, x0, #1
            st3       {v0.b, v1.b, v2.b}[0], [%2], #3
            b RGB_L1

        BGR_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        : "memory", "cc", "x0", "v0", "v1", "v2");
}

void i420_to_bgra(const Image &src, const Image &dst)
{
    asm volatile(
        R"(
            movi v3.16b, %[alpha]
            and x0, %1, #15
            sub %1, %1, x0
            cbz %1, BGRA_L1

        BGRA_L16:
            ld3 {v0.16b, v1.16b, v2.16b}, [%0], #48
            subs %1, %1, #16
            prfm pldl1keep, [%0, #448]
            st4 {v0.16b, v1.16b, v2.16b, v3.16b}, [%2], #64
            bgt BGRA_L16

        BGRA_L1:
            cmp x0, #1
            blt BGRA_END
            ld3 {v0.b, v1.b, v2.b}[0], [%0], #3
            sub x0, x0, #1
            st4 {v0.b, v1.b, v2.b, v3.b}[0], [%2], #4
            b RGBA_L1

        BGRA_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [alpha] "I"(k_alpha)
        : "memory", "cc", "x0", "v0", "v1", "v2", "v3");
}

void i420_to_yuyv(const Image &src, const Image &dst)
{
    asm volatile(
        LOAD_YUV_PARAM
        R"(
            movi v23.16b, #128
            uaddlp v23.8h, v23.16b
            shl v23.8h, v23.8h, %[shift]

            and x0, %1, #7
            sub %1, %1, x0
            cbz %1, YUYV_L2

        YUYV_L8:
            ld3 {v0.8b, v1.8b, v2.8b}, [%0], #24
            umull v3.8h, v2.8b, v29.8b
            umlal v3.8h, v1.8b, v30.8b
            prfm pldl1keep, [%0, 448]
            umlal v3.8h, v0.8b, v31.8b
            uqshrn v3.8b, v3.8h, %[shift]

            uaddlp v0.4h, v0.8b
            uaddlp v1.4h, v1.8b
            uaddlp v2.4h, v2.8b

            mul v4.8h, v0.8h, v26.8h
            mls v4.8h, v2.8h, v24.8h
            mls v4.8h, v1.8h, v25.8h
            addhn v4.8b, v4.8h, v23.8h

            mul v5.8h, v2.8h, v26.8h
            mls v5.8h, v1.8h, v27.8h
            mls v5.8h, v0.8h, v28.8h
            addhn v5.8b, v5.8h, v23.8h

            zip1 v4.16b, v4.16b, v5.16b
            subs %1, %1, #8

            st2 {v3.8b, v4.8b}, [%2], #16
            bgt YUYV_L8

        YUYV_L2:
            cmp x0, #2
            blt YUYV_END
            ld3 {v0.b, v1.b, v2.b}[0], [%0], #3
            ld3 {v0.b, v1.b, v2.b}[1], [%0], #3

            umull v3.8h, v2.8b, v29.8b
            umlal v3.8h, v1.8b, v30.8b
            umlal v3.8h, v0.8b, v31.8b
            uqshrn v3.8b, v3.8h, %[shift]
            sub x0, x0, #2

            uaddlp v0.4h, v0.8b
            uaddlp v1.4h, v1.8b
            uaddlp v2.4h, v2.8b

            mul v4.8h, v0.8h, v26.8h
            mls v4.8h, v2.8h, v24.8h
            mls v4.8h, v1.8h, v25.8h
            addhn v4.8b, v4.8h, v23.8h

            mul v5.8h, v2.8h, v26.8h
            mls v5.8h, v1.8h, v27.8h
            mls v5.8h, v0.8h, v28.8h
            addhn v5.8b, v5.8h, v23.8h

            zip1 v4.16b, v4.16b, v5.16b

            st2 {v3.b, v4.b}[0], [%2], #2
            st2 {v3.b, v4.b}[1], [%2], #2
            b YUYV_L2

        YUYV_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [param] "r"(&k_rgb_2_yuv), [shift] "i"(k_shift)
        : "cc", "memory", "x0"
        , "v0", "v1", "v2", "v3", "v4", "v5", "v23", USED_YUV_REG);
}

void i420_to_uyvy(const Image &src, const Image &dst)
{
    asm volatile(
        LOAD_YUV_PARAM
        R"(
            movi v23.16b, #128
            uaddlp v23.8h, v23.16b
            shl v23.8h, v23.8h, %[shift]

            and x0, %1, #7
            sub %1, %1, x0
            cbz %1, UYVY_L2

        UYVY_L8:
            ld3 {v0.8b, v1.8b, v2.8b}, [%0], #24
            umull v4.8h, v0.8b, v31.8b
            umlal v4.8h, v1.8b, v30.8b
            prfm pldl1keep, [%0, 448]
            umlal v4.8h, v2.8b, v29.8b
            uqshrn v4.8b, v4.8h, %[shift]

            uaddlp v0.4h, v0.8b
            uaddlp v1.4h, v1.8b
            uaddlp v2.4h, v2.8b

            mul v3.8h, v2.8h, v26.8h
            mls v3.8h, v0.8h, v24.8h
            mls v3.8h, v1.8h, v25.8h
            addhn v3.8b, v3.8h, v23.8h

            mul v5.8h, v0.8h, v26.8h
            mls v5.8h, v1.8h, v27.8h
            mls v5.8h, v2.8h, v28.8h
            addhn v5.8b, v5.8h, v23.8h

            zip1 v3.16b, v3.16b, v5.16b
            subs %1, %1, #8

            st2 {v3.8b, v4.8b}, [%2], #16
            bgt UYVY_L8

        UYVY_L2:
            cmp x0, #2
            blt UYVY_END
            ld3 {v0.b, v1.b, v2.b}[0], [%0], #3
            ld3 {v0.b, v1.b, v2.b}[1], [%0], #3

            umull v4.8h, v0.8b, v31.8b
            umlal v4.8h, v1.8b, v30.8b
            umlal v4.8h, v2.8b, v29.8b
            uqshrn v4.8b, v4.8h, %[shift]
            sub x0, x0, #2


            uaddlp v0.4h, v0.8b
            uaddlp v1.4h, v1.8b
            uaddlp v2.4h, v2.8b

            mul v3.8h, v2.8h, v26.8h
            mls v3.8h, v0.8h, v24.8h
            mls v3.8h, v1.8h, v25.8h
            addhn v3.8b, v3.8h, v23.8h

            mul v5.8h, v0.8h, v26.8h
            mls v5.8h, v1.8h, v27.8h
            mls v5.8h, v2.8h, v28.8h
            addhn v5.8b, v5.8h, v23.8h

            zip1 v3.16b, v3.16b, v5.16b

            st2 {v3.b, v4.b}[0], [%2], #2
            st2 {v3.b, v4.b}[1], [%2], #2
            b UYVY_L2

        UYVY_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [param] "r"(&k_rgb_2_yuv), [shift] "i"(k_shift)
        : "cc", "memory", "x0"
        , "v0", "v1", "v2", "v3", "v4", "v5", "v23", USED_YUV_REG);
}

void i420_to_i420(const Image &src, const Image &dst)
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
            movi v23.16b, #128
            uaddlp v23.8h, v23.16b
            shl v23.8h, v23.8h, %[shift]
            mov x1, %0
            mov x3, %1

        I420_H2:
            cmp %[h], #2
            blt I420_END
            sub %[h], %[h], #2
            mov x0, x1
            add x1, x1, %[ss]

            mov x2, x3
            add x3, x3, %[w]

            and x5, %[w], #7
            sub x4, %[w], x5
            cbz x4, I420_H2_L2
        I420_H2_L8:
            ld3 {v0.8b, v1.8b, v2.8b}, [x0], #24
            ld3 {v3.8b, v4.8b, v5.8b}, [x1], #24

            umull v6.8h, v0.8b, v31.8b
            umlal v6.8h, v1.8b, v30.8b
            prfm pldl1keep, [x0, 448]
            umlal v6.8h, v2.8b, v29.8b

            umull v7.8h, v3.8b, v31.8b
            umlal v7.8h, v4.8b, v30.8b
            prfm pldl1keep, [x1, 448]
            umlal v7.8h, v5.8b, v29.8b

            uqshrn v6.8b, v6.8h, %[shift]
            uqshrn v7.8b, v7.8h, %[shift]
            st1 {v6.8b}, [x2], #8
            st1 {v7.8b}, [x3], #8

            uaddlp v0.4h, v0.8b
            uadalp v0.4h, v3.8b
            uaddlp v1.4h, v1.8b
            uadalp v1.4h, v4.8b
            uaddlp v2.4h, v2.8b
            uadalp v2.4h, v5.8b

            ushr v0.4h, v0.4h, #1
            ushr v1.4h, v1.4h, #1
            ushr v2.4h, v2.4h, #1

            mul v3.8h, v2.8h, v26.8h
            mls v3.8h, v0.8h, v24.8h
            mls v3.8h, v1.8h, v25.8h
            addhn v3.8b, v3.8h, v23.8h

            mul v4.8h, v0.8h, v26.8h
            mls v4.8h, v1.8h, v27.8h
            mls v4.8h, v2.8h, v28.8h
            addhn v4.8b, v4.8h, v23.8h

            st1 {v3.s}[0], [%2], #4
            subs x4, x4, #8
            st1 {v4.s}[0], [%3], #4
            bgt I420_H2_L8
        I420_H2_L2:
            cmp x5, #2
            blt I420_H2

            ld3 {v0.b, v1.b, v2.b}[0], [x0], #3
            ld3 {v0.b, v1.b, v2.b}[1], [x0], #3
            umull v6.8h, v0.8b, v31.8b
            umlal v6.8h, v1.8b, v30.8b
            umlal v6.8h, v2.8b, v29.8b

            ld3 {v3.b, v4.b, v5.b}[0], [x1], #3
            ld3 {v3.b, v4.b, v5.b}[1], [x1], #3
            umull v7.8h, v3.8b, v31.8b
            umlal v7.8h, v4.8b, v30.8b
            umlal v7.8h, v5.8b, v29.8b

            sub x5, x5, #2
            uqshrn v6.8b, v6.8h, %[shift]
            uqshrn v7.8b, v7.8h, %[shift]
            st1 {v6.h}[0], [x2], #2
            st1 {v7.h}[0], [x3], #2

            uaddlp v0.4h, v0.8b
            uadalp v0.4h, v3.8b
            uaddlp v1.4h, v1.8b
            uadalp v1.4h, v4.8b
            uaddlp v2.4h, v2.8b
            uadalp v2.4h, v5.8b

            ushr v0.4h, v0.4h, #1
            ushr v1.4h, v1.4h, #1
            ushr v2.4h, v2.4h, #1

            mul v3.8h, v2.8h, v26.8h
            mls v3.8h, v0.8h, v24.8h
            mls v3.8h, v1.8h, v25.8h
            addhn v3.8b, v3.8h, v23.8h

            mul v4.8h, v0.8h, v26.8h
            mls v4.8h, v1.8h, v27.8h
            mls v4.8h, v2.8h, v28.8h
            addhn v4.8b, v4.8h, v23.8h

            st1 {v3.b}[0], [%2], #1
            st1 {v4.b}[0], [%3], #1
            bgt I420_H2_L2

        I420_END:
        )"
        :
        : "r"(src.data()), "r"(y), "r"(u), "r"(v)
        , [ss]"r"(src.stride()), [h] "r"(1UL * h), [w] "r"(1UL * w)
        , [param] "r"(&k_rgb_2_yuv), [shift] "i"(k_shift)
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5", USED_YUV_REG, "v1", "v2", "v3", "v4", "v5", "v6", "v7");
}

void i420_to_nv12(const Image &src, const Image &dst)
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
            movi v23.16b, #128
            uaddlp v23.8h, v23.16b
            shl v23.8h, v23.8h, %[shift]
            mov x1, %0
            mov x3, %1

        NV12_H2:
            cmp %[h], #2
            blt NV12_END
            sub %[h], %[h], #2
            mov x0, x1
            add x1, x1, %[ss]

            mov x2, x3
            add x3, x3, %[w]

            and x5, %[w], #7
            sub x4, %[w], x5
            cbz x4, NV12_H2_L2
        NV12_H2_L8:
            ld3 {v0.8b, v1.8b, v2.8b}, [x0], #24
            ld3 {v3.8b, v4.8b, v5.8b}, [x1], #24

            umull v6.8h, v0.8b, v31.8b
            umlal v6.8h, v1.8b, v30.8b
            prfm pldl1keep, [x0, 448]
            umlal v6.8h, v2.8b, v29.8b

            umull v7.8h, v3.8b, v31.8b
            umlal v7.8h, v4.8b, v30.8b
            prfm pldl1keep, [x1, 448]
            umlal v7.8h, v5.8b, v29.8b

            uqshrn v6.8b, v6.8h, %[shift]
            uqshrn v7.8b, v7.8h, %[shift]
            st1 {v6.8b}, [x2], #8
            st1 {v7.8b}, [x3], #8

            uaddlp v0.4h, v0.8b
            uadalp v0.4h, v3.8b
            uaddlp v1.4h, v1.8b
            uadalp v1.4h, v4.8b
            uaddlp v2.4h, v2.8b
            uadalp v2.4h, v5.8b

            ushr v0.4h, v0.4h, #1
            ushr v1.4h, v1.4h, #1
            ushr v2.4h, v2.4h, #1

            mul v3.8h, v2.8h, v26.8h
            mls v3.8h, v0.8h, v24.8h
            mls v3.8h, v1.8h, v25.8h
            addhn v3.8b, v3.8h, v23.8h

            mul v4.8h, v0.8h, v26.8h
            mls v4.8h, v1.8h, v27.8h
            mls v4.8h, v2.8h, v28.8h
            addhn v4.8b, v4.8h, v23.8h

            zip1 v3.8b, v3.8b, v4.8b
            subs x4, x4, #8
            st1 {v3.8b}, [%2], #8
            bgt NV12_H2_L8
        NV12_H2_L2:
            cmp x5, #2
            blt NV12_H2

            ld3 {v0.b, v1.b, v2.b}[0], [x0], #3
            ld3 {v0.b, v1.b, v2.b}[1], [x0], #3
            umull v6.8h, v0.8b, v31.8b
            umlal v6.8h, v1.8b, v30.8b
            umlal v6.8h, v2.8b, v29.8b

            ld3 {v3.b, v4.b, v5.b}[0], [x1], #3
            ld3 {v3.b, v4.b, v5.b}[1], [x1], #3
            umull v7.8h, v3.8b, v31.8b
            umlal v7.8h, v4.8b, v30.8b
            umlal v7.8h, v5.8b, v29.8b

            sub x5, x5, #2
            uqshrn v6.8b, v6.8h, %[shift]
            uqshrn v7.8b, v7.8h, %[shift]
            st1 {v6.h}[0], [x2], #2
            st1 {v7.h}[0], [x3], #2

            uaddlp v0.4h, v0.8b
            uadalp v0.4h, v3.8b
            uaddlp v1.4h, v1.8b
            uadalp v1.4h, v4.8b
            uaddlp v2.4h, v2.8b
            uadalp v2.4h, v5.8b

            ushr v0.4h, v0.4h, #1
            ushr v1.4h, v1.4h, #1
            ushr v2.4h, v2.4h, #1

            mul v3.8h, v2.8h, v26.8h
            mls v3.8h, v0.8h, v24.8h
            mls v3.8h, v1.8h, v25.8h
            addhn v3.8b, v3.8h, v23.8h

            mul v4.8h, v0.8h, v26.8h
            mls v4.8h, v1.8h, v27.8h
            mls v4.8h, v2.8h, v28.8h
            addhn v4.8b, v4.8h, v23.8h

            zip1 v3.8b, v3.8b, v4.8b
            st1 {v3.h}[0], [%2], #2
            bgt NV12_H2_L2

        NV12_END:
        )"
        :
        : "r"(src.data()), "r"(y), "r"(uv)
        , [ss]"r"(src.stride()), [h] "r"(1UL * h), [w] "r"(1UL * w)
        , [param] "r"(&k_rgb_2_yuv), [shift] "i"(k_shift)
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5", USED_YUV_REG, "v1", "v2", "v3", "v4", "v5", "v6", "v7");
}

void i420_to_nv21(const Image &src, const Image &dst)
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
            movi v23.16b, #128
            uaddlp v23.8h, v23.16b
            shl v23.8h, v23.8h, %[shift]
            mov x1, %0
            mov x3, %1

        NV21_H2:
            cmp %[h], #2
            blt NV21_END
            sub %[h], %[h], #2
            mov x0, x1
            add x1, x1, %[ss]

            mov x2, x3
            add x3, x3, %[w]

            and x5, %[w], #7
            sub x4, %[w], x5
            cbz x4, NV21_H2_L2
        NV21_H2_L8:
            ld3 {v0.8b, v1.8b, v2.8b}, [x0], #24
            ld3 {v3.8b, v4.8b, v5.8b}, [x1], #24

            umull v6.8h, v0.8b, v31.8b
            umlal v6.8h, v1.8b, v30.8b
            prfm pldl1keep, [x0, 448]
            umlal v6.8h, v2.8b, v29.8b

            umull v7.8h, v3.8b, v31.8b
            umlal v7.8h, v4.8b, v30.8b
            prfm pldl1keep, [x1, 448]
            umlal v7.8h, v5.8b, v29.8b

            uqshrn v6.8b, v6.8h, %[shift]
            uqshrn v7.8b, v7.8h, %[shift]
            st1 {v6.8b}, [x2], #8
            st1 {v7.8b}, [x3], #8

            uaddlp v0.4h, v0.8b
            uadalp v0.4h, v3.8b
            uaddlp v1.4h, v1.8b
            uadalp v1.4h, v4.8b
            uaddlp v2.4h, v2.8b
            uadalp v2.4h, v5.8b

            ushr v0.4h, v0.4h, #1
            ushr v1.4h, v1.4h, #1
            ushr v2.4h, v2.4h, #1

            mul v3.8h, v2.8h, v26.8h
            mls v3.8h, v0.8h, v24.8h
            mls v3.8h, v1.8h, v25.8h
            addhn v3.8b, v3.8h, v23.8h

            mul v4.8h, v0.8h, v26.8h
            mls v4.8h, v1.8h, v27.8h
            mls v4.8h, v2.8h, v28.8h
            addhn v4.8b, v4.8h, v23.8h

            zip1 v3.8b, v4.8b, v3.8b
            subs x4, x4, #8
            st1 {v3.8b}, [%2], #8
            bgt NV21_H2_L8
        NV21_H2_L2:
            cmp x5, #2
            blt NV21_H2

            ld3 {v0.b, v1.b, v2.b}[0], [x0], #3
            ld3 {v0.b, v1.b, v2.b}[1], [x0], #3
            umull v6.8h, v0.8b, v31.8b
            umlal v6.8h, v1.8b, v30.8b
            umlal v6.8h, v2.8b, v29.8b

            ld3 {v3.b, v4.b, v5.b}[0], [x1], #3
            ld3 {v3.b, v4.b, v5.b}[1], [x1], #3
            umull v7.8h, v3.8b, v31.8b
            umlal v7.8h, v4.8b, v30.8b
            umlal v7.8h, v5.8b, v29.8b

            sub x5, x5, #2
            uqshrn v6.8b, v6.8h, %[shift]
            uqshrn v7.8b, v7.8h, %[shift]
            st1 {v6.h}[0], [x2], #2
            st1 {v7.h}[0], [x3], #2

            uaddlp v0.4h, v0.8b
            uadalp v0.4h, v3.8b
            uaddlp v1.4h, v1.8b
            uadalp v1.4h, v4.8b
            uaddlp v2.4h, v2.8b
            uadalp v2.4h, v5.8b

            ushr v0.4h, v0.4h, #1
            ushr v1.4h, v1.4h, #1
            ushr v2.4h, v2.4h, #1

            mul v3.8h, v2.8h, v26.8h
            mls v3.8h, v0.8h, v24.8h
            mls v3.8h, v1.8h, v25.8h
            addhn v3.8b, v3.8h, v23.8h

            mul v4.8h, v0.8h, v26.8h
            mls v4.8h, v1.8h, v27.8h
            mls v4.8h, v2.8h, v28.8h
            addhn v4.8b, v4.8h, v23.8h

            zip1 v3.8b, v4.8b, v3.8b
            st1 {v3.h}[0], [%2], #2
            bgt NV21_H2_L2

        NV21_END:
        )"
        :
        : "r"(src.data()), "r"(y), "r"(uv)
        , [ss]"r"(src.stride()), [h] "r"(1UL * h), [w] "r"(1UL * w)
        , [param] "r"(&k_rgb_2_yuv), [shift] "i"(k_shift)
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5", USED_YUV_REG, "v1", "v2", "v3", "v4", "v5", "v6", "v7");
}
NAMESPACE_END
