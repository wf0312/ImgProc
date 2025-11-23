#include "types.hpp"
#include <algorithm>
#include <cassert>
#include <cvt_color.hpp>

#include "yuv.hpp"
#include "image.hpp"

NAMESPACE_BEGIN

void yuyv_to_gray_c(const Image& src, const Image& dst)
{
    constexpr auto k_src_per_loop = 4L;
    constexpr auto k_dst_per_loop = 2L;

    auto size    = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    for (int64_t i = 0L, j = 0L; i < size; i += k_src_per_loop, j += k_dst_per_loop) {
        dst_buf[j + 0] = src_buf[i + 0];
        dst_buf[j + 1] = src_buf[i + 2];
    }
}

void yuyv_to_rgba_c(const Image& src, const Image& dst)
{
    constexpr auto k_src_per_loop = 4L;
    constexpr auto k_dst_per_loop = 8L;

    auto size    = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    for (int64_t i = 0L, j = 0L; i < size; i += k_src_per_loop, j += k_dst_per_loop) {
        int32_t y0 = src_buf[i] << k_shift;
        int32_t u  = src_buf[i + 1];
        int32_t y1 = src_buf[i + 2] << k_shift;
        int32_t v  = src_buf[i + 3];
        dst_buf[j + 0] = saturate_u8((y0 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
        dst_buf[j + 1] = saturate_u8((y0 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
        dst_buf[j + 2] = saturate_u8((y0 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
        dst_buf[j + 3] = k_alpha;
        dst_buf[j + 4] = saturate_u8((y1 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
        dst_buf[j + 5] = saturate_u8((y1 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
        dst_buf[j + 6] = saturate_u8((y1 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
        dst_buf[j + 7] = k_alpha;
    }
}

void yuyv_to_rgb_c(const Image& src, const Image& dst)
{
    constexpr auto k_src_per_loop = 4L;
    constexpr auto k_dst_per_loop = 6L;

    auto size    = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    for (int64_t i = 0L, j = 0L; i < size; i += k_src_per_loop, j += k_dst_per_loop) {
        int32_t y0 = src_buf[i] << k_shift;
        int32_t u  = src_buf[i + 1];
        int32_t y1 = src_buf[i + 2] << k_shift;
        int32_t v  = src_buf[i + 3];
        dst_buf[j + 0] = saturate_u8((y0 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
        dst_buf[j + 1] = saturate_u8((y0 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
        dst_buf[j + 2] = saturate_u8((y0 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
        dst_buf[j + 3] = saturate_u8((y1 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
        dst_buf[j + 4] = saturate_u8((y1 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
        dst_buf[j + 5] = saturate_u8((y1 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
    }
}

void yuyv_to_bgra_c(const Image& src, const Image& dst)
{
    constexpr auto k_src_per_loop = 4L;
    constexpr auto k_dst_per_loop = 8L;

    auto size    = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    for (int64_t i = 0L, j = 0L; i < size; i += k_src_per_loop, j += k_dst_per_loop) {
        int32_t y0 = src_buf[i] << k_shift;
        int32_t u  = src_buf[i + 1];
        int32_t y1 = src_buf[i + 2] << k_shift;
        int32_t v  = src_buf[i + 3];
        dst_buf[j + 0] = saturate_u8((y0 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
        dst_buf[j + 1] = saturate_u8((y0 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
        dst_buf[j + 2] = saturate_u8((y0 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
        dst_buf[j + 3] = k_alpha;
        dst_buf[j + 4] = saturate_u8((y1 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
        dst_buf[j + 5] = saturate_u8((y1 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
        dst_buf[j + 6] = saturate_u8((y1 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
        dst_buf[j + 7] = k_alpha;
    }
}

void yuyv_to_bgr_c(const Image& src, const Image& dst)
{
    constexpr auto k_src_per_loop = 4L;
    constexpr auto k_dst_per_loop = 6L;

    auto size    = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    for (int64_t i = 0L, j = 0L; i < size; i += k_src_per_loop, j += k_dst_per_loop) {
        int32_t y0 = src_buf[i] << k_shift;
        int32_t u  = src_buf[i + 1];
        int32_t y1 = src_buf[i + 2] << k_shift;
        int32_t v  = src_buf[i + 3];
        dst_buf[j + 0] = saturate_u8((y0 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
        dst_buf[j + 1] = saturate_u8((y0 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
        dst_buf[j + 2] = saturate_u8((y0 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
        dst_buf[j + 3] = saturate_u8((y1 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
        dst_buf[j + 4] = saturate_u8((y1 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
        dst_buf[j + 5] = saturate_u8((y1 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
    }
}

void yuyv_to_yuyv_c(const Image& src, const Image& dst)
{
    std::copy_n(src.data(), src.size(), dst.data());
}

void yuyv_to_uyvy_c(const Image& src, const Image& dst)
{
    auto size    = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();

    for (auto i = 0L; i < size; i += 4L) {
        dst_buf[i + 0] = src_buf[i + 1];
        dst_buf[i + 1] = src_buf[i + 0];
        dst_buf[i + 2] = src_buf[i + 3];
        dst_buf[i + 3] = src_buf[i + 2];
    }
}

void yuyv_to_i420_c(const Image& src, const Image& dst)
{
    auto ss  = src.stride();
    auto w = src.cols();
    auto h = src.rows();
    auto ys  = 1L * w;
    auto uvs = ys >> 1;

    auto src0 = src.data();
    auto src1 = src0 + ss;
    auto y0   = dst.data();
    auto y1   = y0 + ys;

    auto pixels = src.pixels();
    auto u = y0 + pixels;
    auto v = u + (pixels >> 2);

    for (int i = 0; i < h; i += 2) {
        for (auto j = 0L, k = 0L, x = 0L; j < ss; j += 4L, k += 2L, x += 1L) {
            y0[k + 0] = src0[j + 0];
            y0[k + 1] = src0[j + 2];
            y1[k + 0] = src1[j + 0];
            y1[k + 1] = src1[j + 2];

            u[x] = ((src0[j + 1] + src1[j + 1]) >> 1);
            v[x] = ((src0[j + 3] + src1[j + 3]) >> 1);
        }
        src0 += (ss << 1);
        src1 += (ss << 1);
        y0 += (ys << 1);
        y1 += (ys << 1);
        u += uvs;
        v += uvs;
    }
}

void yuyv_to_nv12_c(const Image& src, const Image& dst)
{
    auto ss  = src.stride();
    auto w = src.cols();
    auto h = src.rows();
    auto ys  = 1L * w;
    auto uvs = ys;

    auto src0                                    = src.data();
    auto src1                                    = src0 + ss;
    auto y0                                      = dst.data();
    auto y1                                      = y0 + ys;
    auto uv = y0 + src.pixels();

    for (int i = 0; i < h; i += 2) {
        for (auto j = 0L, k = 0L, x = 0L; j < ss; j += 4L, k += 2L, x += 2L) {
            y0[k + 0] = src0[j + 0];
            y0[k + 1] = src0[j + 2];
            y1[k + 0] = src1[j + 0];
            y1[k + 1] = src1[j + 2];

            uv[x + 0] = ((src0[j + 1] + src1[j + 1]) >> 1);
            uv[x + 1] = ((src0[j + 3] + src1[j + 3]) >> 1);
        }
        src0 += (ss << 1);
        src1 += (ss << 1);
        y0 += (ys << 1);
        y1 += (ys << 1);
        uv += uvs;
    }
}

void yuyv_to_nv21_c(const Image& src, const Image& dst)
{
    auto ss  = src.stride();
    auto w = src.cols();
    auto h = src.rows();
    auto ys  = 1L * w;
    auto uvs = ys;

    auto src0                                    = src.data();
    auto src1                                    = src0 + ss;
    auto y0                                      = dst.data();
    auto y1                                      = y0 + ys;
    auto uv = y0 + src.pixels();

    for (int i = 0; i < h; i += 2) {
        for (auto j = 0L, k = 0L, x = 0L; j < ss; j += 4L, k += 2L, x += 2L) {
            y0[k + 0] = src0[j + 0];
            y0[k + 1] = src0[j + 2];
            y1[k + 0] = src1[j + 0];
            y1[k + 1] = src1[j + 2];

            uv[x + 0] = ((src0[j + 3] + src1[j + 3]) >> 1);
            uv[x + 1] = ((src0[j + 1] + src1[j + 1]) >> 1);
        }
        src0 += (ss << 1);
        src1 += (ss << 1);
        y0 += (ys << 1);
        y1 += (ys << 1);
        uv += uvs;
    }
}

void yuyv_to_gray(const Image& src, const Image& dst)
{
    asm volatile(
        R"(
            and x0, %1, #15
            sub %1, %1, x0
            cbz %1, GRAY_L2
        GRAY_L16:
            ld2 {v0.16b, v1.16b}, [%0], #32
            subs %1, %1, #16
            prfm pldl1keep, [%0, 448]
            st1 {v0.16b}, [%2], #16
            bgt GRAY_L16

        GRAY_L2:
            cmp x0, #2
            blt GRAY_END
            ld4 {v0.b, v1.b, v2.b, v3.b}[0], [%0], #4
            mov v0.b[1], v2.b[0]
            sub x0, x0, #2
            st1 {v0.h}[0], [%2], #2
            b GRAY_L2
        GRAY_END:
        )" :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        : "cc", "memory", "x0"
        , "v0", "v1");
}

void yuyv_to_rgb(const Image& src, const Image& dst)
{
    asm volatile(
        LOAD_RGB_PARAM
        R"(
            and x0, %1, #15
            sub %1, %1, x0
            cbz %1, RGB_L2
        RGB_L16:
            ld4 {v0.8b, v1.8b, v2.8b, v3.8b}, [%0], #32
            zip1 v4.16b, v0.16b, v2.16b
            ushll v0.8h, v4.8b, %[shift]
            zip1 v1.16b, v1.16b, v1.16b
            ushll2 v2.8h, v4.16b, %[shift]
            prfm pldl1keep, [%0, 448]
            zip1 v3.16b, v3.16b, v3.16b

            // r
            umull v4.8h, v3.8b, v28.8b
            umull2 v7.8h, v3.16b, v28.16b
            add v4.8h, v0.8h, v4.8h
            add v7.8h, v2.8h, v7.8h
            uqsub v4.8h, v4.8h, v25.8h
            uqsub v7.8h, v7.8h, v25.8h
            uqrshrn v4.8b, v4.8h, %[shift]
            uqrshrn2 v4.16b, v7.8h, %[shift]

            // g
            umull v5.8h, v1.8b, v29.8b
            umull2 v6.8h, v1.16b, v29.16b
            umlal v5.8h, v3.8b, v30.8b
            umlal2 v6.8h, v3.16b, v30.16b
            add v8.8h, v0.8h, v26.8h
            add v9.8h, v2.8h, v26.8h
            uqsub v8.8h, v8.8h, v5.8h
            uqsub v9.8h, v9.8h, v6.8h
            uqrshrn v5.8b, v8.8h, %[shift]
            uqrshrn2 v5.16b, v9.8h, %[shift]

            // b
            umull v6.8h, v1.8b, v31.8b
            umull2 v7.8h, v1.16b, v31.16b
            add v0.8h, v0.8h, v6.8h
            add v2.8h, v2.8h, v7.8h
            uqsub v0.8h, v0.8h, v27.8h
            uqsub v2.8h, v2.8h, v27.8h
            uqrshrn v6.8b, v0.8h, %[shift]
            uqrshrn2 v6.16b, v2.8h, %[shift]

            st3 {v4.16b, v5.16b, v6.16b}, [%2], #48
            subs %1, %1, #16
            bgt RGB_L16

        RGB_L2:
            cmp %1, #2
            blt RGB_END

            ld2 {v0.b, v1.b}[0], [%0], #2
            ld2 {v0.b, v1.b}[1], [%0], #2
            zip1 v1.16b, v1.16b, v1.16b
            ushll v0.8h, v0.8b, %[shift]
            mov v3.b[0], v1.b[2]
            zip1 v3.16b, v3.16b, v3.16b

            // r
            umull v4.8h, v3.8b, v28.8b
            add v4.8h, v0.8h, v4.8h
            uqsub v4.8h, v4.8h, v25.8h
            uqrshrn v4.8b, v4.8h, %[shift]

            // g
            umull v5.8h, v1.8b, v29.8b
            umlal v5.8h, v3.8b, v30.8b
            add v2.8h, v0.8h, v26.8h
            uqsub v2.8h, v2.8h, v5.8h
            uqrshrn v5.8b, v2.8h, %[shift]

            // b
            umull v6.8h, v1.8b, v31.8b
            add v0.8h, v0.8h, v6.8h
            uqsub v0.8h, v0.8h, v27.8h
            uqrshrn v6.8b, v0.8h, %[shift]

            sub %1, %1, #2
            st3 {v4.b, v5.b, v6.b}[0], [%2], #3
            st3 {v4.b, v5.b, v6.b}[1], [%2], #3
            b RGB_L2

        RGB_END:
        )" :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [param] "r"(&k_yuv_2_rgb), [shift] "i"(k_shift)
        : "cc", "memory", "x0"
        , "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", USED_RGB_REG);
}

void yuyv_to_rgba(const Image& src, const Image& dst)
{
    asm volatile(
        LOAD_RGB_PARAM
        R"(
            mov w0, %[alpha]
            dup v7.16b, w0
            and x0, %1, #15
            sub %1, %1, x0
            cbz %1, RGBA_L2
        RGBA_L16:
            ld4 {v0.8b, v1.8b, v2.8b, v3.8b}, [%0], #32
            zip1 v4.16b, v0.16b, v2.16b
            ushll v0.8h, v4.8b, %[shift]
            zip1 v1.16b, v1.16b, v1.16b
            ushll2 v2.8h, v4.16b, %[shift]
            prfm pldl1keep, [%0, 448]
            zip1 v3.16b, v3.16b, v3.16b

            // r
            umull v4.8h, v3.8b, v28.8b
            umull2 v8.8h, v3.16b, v28.16b
            add v4.8h, v0.8h, v4.8h
            add v8.8h, v2.8h, v8.8h
            uqsub v4.8h, v4.8h, v25.8h
            uqsub v8.8h, v8.8h, v25.8h
            uqrshrn v4.8b, v4.8h, %[shift]
            uqrshrn2 v4.16b, v8.8h, %[shift]

            // g
            umull v5.8h, v1.8b, v29.8b
            umull2 v6.8h, v1.16b, v29.16b
            umlal v5.8h, v3.8b, v30.8b
            umlal2 v6.8h, v3.16b, v30.16b
            add v8.8h, v0.8h, v26.8h
            add v9.8h, v2.8h, v26.8h
            uqsub v8.8h, v8.8h, v5.8h
            uqsub v9.8h, v9.8h, v6.8h
            uqrshrn v5.8b, v8.8h, %[shift]
            uqrshrn2 v5.16b, v9.8h, %[shift]

            // b
            umull v6.8h, v1.8b, v31.8b
            umull2 v1.8h, v1.16b, v31.16b
            add v0.8h, v0.8h, v6.8h
            add v2.8h, v2.8h, v1.8h
            uqsub v0.8h, v0.8h, v27.8h
            uqsub v2.8h, v2.8h, v27.8h
            uqrshrn v6.8b, v0.8h, %[shift]
            uqrshrn2 v6.16b, v2.8h, %[shift]

            st4 {v4.16b, v5.16b, v6.16b, v7.16b}, [%2], #64
            subs %1, %1, #16
            bgt RGBA_L16

        RGBA_L2:
            cmp x0, #2
            blt RGBA_END

            ld2 {v0.b, v1.b}[0], [%0], #2
            ld2 {v0.b, v1.b}[1], [%0], #2
            zip1 v1.16b, v1.16b, v1.16b
            ushll v0.8h, v0.8b, %[shift]
            mov v3.b[0], v1.b[2]
            zip1 v3.16b, v3.16b, v3.16b

            // r
            umull v4.8h, v3.8b, v28.8b
            add v4.8h, v0.8h, v4.8h
            uqsub v4.8h, v4.8h, v25.8h
            uqrshrn v4.8b, v4.8h, %[shift]

            // g
            umull v5.8h, v1.8b, v29.8b
            umlal v5.8h, v3.8b, v30.8b
            add v2.8h, v0.8h, v26.8h
            uqsub v2.8h, v2.8h, v5.8h
            uqrshrn v5.8b, v2.8h, %[shift]

            // b
            umull v6.8h, v1.8b, v31.8b
            add v0.8h, v0.8h, v6.8h
            uqsub v0.8h, v0.8h, v27.8h
            uqrshrn v6.8b, v0.8h, %[shift]

            sub x0, x0, #2
            st4 {v4.b, v5.b, v6.b, v7.b}[0], [%2], #4
            st4 {v4.b, v5.b, v6.b, v7.b}[1], [%2], #4
            b RGBA_L2
        RGBA_END:
        )" :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [param] "r"(&k_yuv_2_rgb), [shift] "i"(k_shift), [alpha] "i"(k_alpha)
        : "cc", "memory", "x0"
        , "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", USED_RGB_REG);
}

void yuyv_to_bgr(const Image& src, const Image& dst)
{
    asm volatile(
        LOAD_RGB_PARAM
        R"(
            and x0, %1, #15
            sub %1, %1, x0
            cbz %1, BGR_L2
        BGR_L16:
            ld4 {v0.8b, v1.8b, v2.8b, v3.8b}, [%0], #32
            zip1 v4.16b, v0.16b, v2.16b
            ushll v0.8h, v4.8b, %[shift]
            zip1 v1.16b, v1.16b, v1.16b
            ushll2 v2.8h, v4.16b, %[shift]
            prfm pldl1keep, [%0, 448]
            zip1 v3.16b, v3.16b, v3.16b

            // b
            umull v4.8h, v1.8b, v31.8b
            umull2 v6.8h, v1.16b, v31.16b
            add v4.8h, v0.8h, v4.8h
            add v6.8h, v2.8h, v6.8h
            uqsub v4.8h, v4.8h, v27.8h
            uqsub v6.8h, v6.8h, v27.8h
            uqrshrn v4.8b, v4.8h, %[shift]
            uqrshrn2 v4.16b, v6.8h, %[shift]

            // g
            umull v5.8h, v1.8b, v29.8b
            umull2 v6.8h, v1.16b, v29.16b
            umlal v5.8h, v3.8b, v30.8b
            umlal2 v6.8h, v3.16b, v30.16b
            add v7.8h, v0.8h, v26.8h
            add v8.8h, v2.8h, v26.8h
            uqsub v7.8h, v7.8h, v5.8h
            uqsub v8.8h, v8.8h, v6.8h
            uqrshrn v5.8b, v7.8h, %[shift]
            uqrshrn2 v5.16b, v8.8h, %[shift]

            // r
            umull v6.8h, v3.8b, v28.8b
            umull2 v1.8h, v3.16b, v28.16b
            add v6.8h, v0.8h, v6.8h
            add v1.8h, v2.8h, v1.8h
            uqsub v6.8h, v6.8h, v25.8h
            uqsub v1.8h, v1.8h, v25.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v1.8h, %[shift]

            st3 {v4.16b, v5.16b, v6.16b}, [%2], #48
            subs %1, %1, #16
            bgt BGR_L16

        BGR_L2:
            cmp x0, #2
            blt BGR_END

            ld2 {v0.b, v1.b}[0], [%0], #2
            ld2 {v0.b, v1.b}[1], [%0], #2
            zip1 v1.16b, v1.16b, v1.16b
            ushll v0.8h, v0.8b, %[shift]
            mov v3.b[0], v1.b[2]
            zip1 v3.16b, v3.16b, v3.16b

            // b
            umull v4.8h, v1.8b, v31.8b
            add v4.8h, v0.8h, v4.8h
            uqsub v4.8h, v4.8h, v27.8h
            uqrshrn v4.8b, v4.8h, %[shift]

            // g
            umull v5.8h, v1.8b, v29.8b
            umlal v5.8h, v3.8b, v30.8b
            add v2.8h, v0.8h, v26.8h
            uqsub v2.8h, v2.8h, v5.8h
            uqrshrn v5.8b, v2.8h, %[shift]

            // r
            umull v6.8h, v3.8b, v28.8b
            add v6.8h, v0.8h, v6.8h
            uqsub v6.8h, v6.8h, v25.8h
            uqrshrn v6.8b, v6.8h, %[shift]

            sub x0, x0, #2
            st3 {v4.b, v5.b, v6.b}[0], [%2], #3
            st3 {v4.b, v5.b, v6.b}[1], [%2], #3
            b BGR_L2

        BGR_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [param] "r"(&k_yuv_2_rgb), [shift] "i"(k_shift)
        : "cc", "memory", "x0"
        , "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", USED_RGB_REG);
}

void yuyv_to_bgra(const Image& src, const Image& dst)
{
    asm volatile(
        LOAD_RGB_PARAM
        R"(
            mov w0, %[alpha]
            dup v7.16b, w0
            and x0, %1, #15
            sub %1, %1, x0
            cbz %1, BGRA_L2
        BGRA_L16:
            ld4 {v0.8b, v1.8b, v2.8b, v3.8b}, [%0], #32
            zip1 v4.16b, v0.16b, v2.16b
            ushll v0.8h, v4.8b, %[shift]
            zip1 v1.16b, v1.16b, v1.16b
            ushll2 v2.8h, v4.16b, %[shift]
            prfm pldl1keep, [%0, 448]
            zip1 v3.16b, v3.16b, v3.16b

            // b
            umull v4.8h, v1.8b, v31.8b
            umull2 v6.8h, v1.16b, v31.16b
            add v4.8h, v0.8h, v4.8h
            add v6.8h, v2.8h, v6.8h
            uqsub v4.8h, v4.8h, v27.8h
            uqsub v6.8h, v6.8h, v27.8h
            uqrshrn v4.8b, v4.8h, %[shift]
            uqrshrn2 v4.16b, v6.8h, %[shift]

            // g
            umull v5.8h, v1.8b, v29.8b
            umull2 v6.8h, v1.16b, v29.16b
            umlal v5.8h, v3.8b, v30.8b
            umlal2 v6.8h, v3.16b, v30.16b
            add v8.8h, v0.8h, v26.8h
            add v9.8h, v2.8h, v26.8h
            uqsub v8.8h, v8.8h, v5.8h
            uqsub v9.8h, v9.8h, v6.8h
            uqrshrn v5.8b, v8.8h, %[shift]
            uqrshrn2 v5.16b, v9.8h, %[shift]

            // r
            umull v6.8h, v3.8b, v28.8b
            umull2 v8.8h, v3.16b, v28.16b
            add v6.8h, v0.8h, v6.8h
            add v8.8h, v2.8h, v8.8h
            uqsub v6.8h, v6.8h, v25.8h
            uqsub v8.8h, v8.8h, v25.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v8.8h, %[shift]

            st4 {v4.16b, v5.16b, v6.16b, v7.16b}, [%2], #64

            subs %1, %1, #16
            bgt BGRA_L16

        BGRA_L2:
            cmp x0, #2
            blt BGRA_END

            ld2 {v0.b, v1.b}[0], [%0], #2
            ld2 {v0.b, v1.b}[1], [%0], #2
            zip1 v1.16b, v1.16b, v1.16b
            ushll v0.8h, v0.8b, %[shift]
            mov v3.b[0], v1.b[2]
            zip1 v3.16b, v3.16b, v3.16b

            // b
            umull v4.8h, v1.8b, v31.8b
            add v4.8h, v0.8h, v4.8h
            uqsub v4.8h, v4.8h, v27.8h
            uqrshrn v4.8b, v4.8h, %[shift]

            // g
            umull v5.8h, v1.8b, v29.8b
            umlal v5.8h, v3.8b, v30.8b
            add v2.8h, v0.8h, v26.8h
            uqsub v2.8h, v2.8h, v5.8h
            uqrshrn v5.8b, v2.8h, %[shift]

            // r
            umull v6.8h, v3.8b, v28.8b
            add v6.8h, v0.8h, v6.8h
            uqsub v6.8h, v6.8h, v25.8h
            uqrshrn v6.8b, v6.8h, %[shift]

            st4 {v4.b, v5.b, v6.b, v7.b}[0], [%2], #4
            st4 {v4.b, v5.b, v6.b, v7.b}[1], [%2], #4
            sub x0, x0, #2
            b BGRA_L2
        BGRA_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [param] "r"(&k_yuv_2_rgb), [shift] "i"(k_shift), [alpha] "i"(k_alpha)
        : "cc", "memory", "x0"
        , "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", USED_RGB_REG);
}

void yuyv_to_yuyv(const Image& src, const Image& dst)
{
    asm volatile(
        R"(
            and x0, %1, #15
            sub %1, %1, x0
            cbz %1, YUYV_L2

        YUYV_L16:
            ld1 {v0.16b, v1.16b}, [%0], #32
            st1 {v0.16b, v1.16b}, [%2], #32
            prfm pldl1keep, [%0, 448]
            subs %1, %1, #16
            bgt YUYV_L16

        YUYV_L2:
            cmp x0, #2
            blt YUYV_END
            ld1 {v0.s}[0], [%0], #4
            st1 {v0.s}[0], [%2], #4
            sub x0, x0, #2

        YUYV_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        : "cc", "memory", "x0", "v0", "v1");
}

void yuyv_to_uyvy(const Image& src, const Image& dst)
{
    asm volatile(
        R"(
            and x0, %1, #15
            sub %1, %1, x0
            cbz %1, UYVY_L2

        UYVY_L16:
            ld4 {v0.8b, v1.8b, v2.8b, v3.8b}, [%0], #32
            mov v4.8b, v2.8b
            prfm pldl1keep, [%0, 448]
            mov v2.8b, v0.8b
            st4 {v1.8b, v2.8b, v3.8b, v4.8b}, [%2], #32
            subs %1, %1, #16
            bgt UYVY_L16

        UYVY_L2:
            cmp x0, #2
            blt UYVY_END
            ld4 {v0.b, v1.b, v2.b, v3.b}[0], [%0], #4
            mov v4.8b, v2.8b
            mov v2.8b, v0.8b
            st4 {v1.b, v2.b, v3.b, v4.b}[0], [%2], #4
            sub x0, x0, #2

        UYVY_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        : "cc", "memory", "x0", "v0", "v1", "v2", "v3");
}

void yuyv_to_i420(const Image& src, const Image& dst)
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
        R"(
            mov x1, %0
            mov x3, %1
        I420_2H:
            cmp %[h], #2
            blt I420_END
            sub %[h], %[h], #2
            and x5, %[w], #15
            sub x4, %[w], x5
            mov x0, x1
            mov x2, x3
            add x1, x1, %[ss]
            add x3, x3, %[ys]
            cbz x4, I420_2H_W2
            I420_2H_W16:
                ld2 {v0.16b, v1.16b}, [x0], #32
                ld2 {v2.16b, v3.16b}, [x1], #32
                uhadd v1.16b, v1.16b, v3.16b
                st1 {v0.16b}, [x2], #16
                prfm pldl1keep, [x0, 448]
                uzp1 v3.16b, v1.16b, v1.16b
                st1 {v2.16b}, [x3], #16
                prfm pldl1keep, [x1, 448]
                st1 {v3.8b}, [%2], #8
                uzp2 v1.16b, v1.16b, v1.16b
                st1 {v1.8b}, [%3], #8
                subs x4, x4, #16
                bgt I420_2H_W16
            I420_2H_W2:
                cmp x5, #2
                blt I420_2H
                ld2 {v0.b, v1.b}[0], [x0], #2
                ld2 {v0.b, v1.b}[1], [x0], #2
                ld2 {v2.b, v3.b}[0], [x0], #2
                ld2 {v2.b, v3.b}[1], [x0], #2
                uhadd v1.16b, v1.16b, v3.16b
                st1 {v0.h}[0], [x2], #2
                uzp1 v3.16b, v1.16b, v1.16b
                st1 {v2.h}[0], [x3], #2
                st1 {v3.b}[0], [%2], #1
                uzp2 v1.16b, v1.16b, v1.16b
                st1 {v1.b}[0], [%3], #1
                b I420_2H_W2
        I420_END:
        )"
        :
        : "r"(src.data()), "r"(y), "r"(u), "r"(v)
        , [w] "r"(1UL * src.cols()), [h] "r"(1UL * src.rows()), [ss] "r"(src.stride()), [ys] "r"(1UL * dst.cols())
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5"
        , "v0", "v1", "v2", "v3", "v4");
}

void yuyv_to_nv12(const Image& src, const Image& dst)
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
    auto uv    = y + dst.pixels();
    asm volatile(
        R"(
            mov x1, %0
            mov x3, %1
        NV12_2H:
            cmp %[h], #2
            blt NV12_END
            sub %[h], %[h], #2
            and x5, %[w], #15
            sub x4, %[w], x5
            mov x0, x1
            mov x2, x3
            add x1, x1, %[ss]
            add x3, x3, %[ys]
            cbz x4, NV12_2H_W2
            NV12_2H_W16:
                ld2 {v0.16b, v1.16b}, [x0], #32
                ld2 {v2.16b, v3.16b}, [x1], #32
                uhadd v1.16b, v1.16b, v3.16b
                st1 {v0.16b}, [x2], #16
                prfm pldl1keep, [x0, 448]
                uzp1 v3.16b, v1.16b, v1.16b
                st1 {v2.16b}, [x3], #16
                prfm pldl1keep, [x1, 448]
                uzp2 v4.16b, v1.16b, v1.16b
                st2 {v3.8b, v4.8b}, [%2], #16
                subs x4, x4, #16
                bgt NV12_2H_W16
            NV12_2H_W2:
                cmp x5, #2
                blt NV12_2H
                ld2 {v0.b, v1.b}[0], [x0], #2
                ld2 {v0.b, v1.b}[1], [x0], #2
                ld2 {v2.b, v3.b}[0], [x0], #2
                ld2 {v2.b, v3.b}[1], [x0], #2
                uhadd v1.16b, v1.16b, v3.16b
                st1 {v0.h}[0], [x2], #2
                uzp1 v3.16b, v1.16b, v1.16b
                st1 {v2.h}[0], [x3], #2
                uzp2 v4.16b, v1.16b, v1.16b
                st2 {v3.b, v4.b}[0], [%2], #2
                b NV12_2H_W2
        NV12_END:
        )"
        :
        : "r"(src.data()), "r"(y), "r"(uv)
        , [w] "r"(1UL * src.cols()), [h] "r"(1UL * src.rows()), [ss] "r"(src.stride()), [ys] "r"(1UL * dst.cols())
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5"
        , "v0", "v1", "v2", "v3", "v4", "v5");
}

void yuyv_to_nv21(const Image& src, const Image& dst)
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
    auto uv    = y + dst.pixels();
    asm volatile(
        R"(
            mov x1, %0
            mov x3, %1
        NV21_2H:
            cmp %[h], #2
            blt NV21_END
            sub %[h], %[h], #2
            and x5, %[w], #15
            sub x4, %[w], x5
            mov x0, x1
            mov x2, x3
            add x1, x1, %[ss]
            add x3, x3, %[ys]
            cbz x4, NV21_2H_W2
            NV21_2H_W16:
                ld2 {v0.16b, v1.16b}, [x0], #32
                ld2 {v2.16b, v3.16b}, [x1], #32
                uhadd v1.16b, v1.16b, v3.16b
                st1 {v0.16b}, [x2], #16
                prfm pldl1keep, [x0, 448]
                uzp1 v4.16b, v1.16b, v1.16b
                st1 {v2.16b}, [x3], #16
                prfm pldl1keep, [x1, 448]
                uzp2 v3.16b, v1.16b, v1.16b
                st2 {v3.8b, v4.8b}, [%2], #16
                subs x4, x4, #16
                bgt NV21_2H_W16
            NV21_2H_W2:
                cmp x5, #2
                blt NV21_2H
                ld2 {v0.b, v1.b}[0], [x0], #2
                ld2 {v0.b, v1.b}[1], [x0], #2
                ld2 {v2.b, v3.b}[0], [x0], #2
                ld2 {v2.b, v3.b}[1], [x0], #2
                uhadd v1.16b, v1.16b, v3.16b
                st1 {v0.h}[0], [x2], #2
                uzp1 v4.16b, v1.16b, v1.16b
                st1 {v2.h}[0], [x3], #2
                uzp2 v3.16b, v1.16b, v1.16b
                st2 {v3.b, v4.b}[0], [%2], #2
                b NV21_2H_W2
        NV21_END:
        )"
        :
        : "r"(src.data()), "r"(y), "r"(uv)
        , [w] "r"(1UL * src.cols()), [h] "r"(1UL * src.rows()), [ss] "r"(src.stride()), [ys] "r"(1UL * dst.cols())
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5"
        , "v0", "v1", "v2", "v3", "v4", "v5");
}

NAMESPACE_END
