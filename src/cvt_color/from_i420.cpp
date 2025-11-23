#include "image.hpp"
#include "types.hpp"
#include <algorithm>
#include <cstddef>
#include "cvt_color.hpp"
#include "yuv.hpp"

NAMESPACE_BEGIN

void i420_to_gray_c(const Image &src, const Image &dst)
{
    auto size = src.pixels();
    auto src_buf = src.data();
    auto dst_buf = dst.data();
    std::copy_n(src_buf, size, dst_buf);
}

void i420_to_rgba_c(const Image &src, const Image &dst)
{
    auto h = src.rows();
    if (0 != h % 2)
    {
        throw std::invalid_argument("Height must be even");
    }

    auto ys = static_cast<uint64_t>(src.cols());
    auto y_buf0 = src.data();
    auto y_buf1 = y_buf0 + ys;

    auto uvs = static_cast<uint64_t>(src.cols()) >> 1;
    auto u_buf = src.data() + src.pixels();
    auto v_buf = u_buf + (src.pixels() >> 2);

    auto ds = dst.stride();
    auto dst0 = dst.data();
    auto dst1 = dst0 + ds;

    for (auto i = 0; i < h; i += 2) {
        for (auto j = 0, k = 0, m = 0; j + 2 < ys; j += 2, k += 8, m += 1) {
            auto y00 = y_buf0[j + 0] << k_shift;
            auto y01 = y_buf0[j + 1] << k_shift;
            auto y10 = y_buf1[j + 0] << k_shift;
            auto y11 = y_buf1[j + 1] << k_shift;
            auto u = u_buf[m];
            auto v = v_buf[m];

            dst0[k + 0] = saturate_u8((y00 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
            dst0[k + 1] = saturate_u8((y00 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
            dst0[k + 2] = saturate_u8((y00 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
            dst0[k + 3] = k_alpha;
            dst0[k + 4] = saturate_u8((y01 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
            dst0[k + 5] = saturate_u8((y01 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
            dst0[k + 6] = saturate_u8((y01 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
            dst0[k + 7] = k_alpha;

            dst1[k + 0] = saturate_u8((y10 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
            dst1[k + 1] = saturate_u8((y10 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
            dst1[k + 2] = saturate_u8((y10 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
            dst1[k + 3] = k_alpha;
            dst1[k + 4] = saturate_u8((y11 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
            dst1[k + 5] = saturate_u8((y11 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
            dst1[k + 6] = saturate_u8((y11 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
            dst1[k + 7] = k_alpha;
        }
        y_buf0 += (ys << 1);
        y_buf1 += (ys << 1);
        u_buf += uvs;
        v_buf += uvs;
        dst0 += (ds << 1);
        dst1 += (ds << 1);
    }
}

void i420_to_rgb_c(const Image &src, const Image &dst)
{
    auto h = src.rows();
    if (0 != h % 2)
    {
        throw std::invalid_argument("Height must be even");
    }

    auto ys = static_cast<uint64_t>(src.cols());
    auto y_buf0 = src.data();
    auto y_buf1 = y_buf0 + ys;

    auto uvs = static_cast<uint64_t>(src.cols()) >> 1;
    auto u_buf = src.data() + src.pixels();
    auto v_buf = u_buf + (src.pixels() >> 2);

    auto ds = dst.stride();
    auto dst0 = dst.data();
    auto dst1 = dst0 + ds;

    for (auto i = 0; i < h; i += 2) {
        for (auto j = 0, k = 0, m = 0; j + 2 < ys; j += 2, k += 6, m += 1) {
            auto y00 = y_buf0[j + 0] << k_shift;
            auto y01 = y_buf0[j + 1] << k_shift;
            auto y10 = y_buf1[j + 0] << k_shift;
            auto y11 = y_buf1[j + 1] << k_shift;
            auto u = u_buf[m];
            auto v = v_buf[m];

            dst0[k + 0] = saturate_u8((y00 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
            dst0[k + 1] = saturate_u8((y00 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
            dst0[k + 2] = saturate_u8((y00 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
            dst0[k + 3] = saturate_u8((y01 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
            dst0[k + 4] = saturate_u8((y01 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
            dst0[k + 5] = saturate_u8((y01 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);

            dst1[k + 0] = saturate_u8((y10 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
            dst1[k + 1] = saturate_u8((y10 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
            dst1[k + 2] = saturate_u8((y10 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
            dst1[k + 3] = saturate_u8((y11 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
            dst1[k + 4] = saturate_u8((y11 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
            dst1[k + 5] = saturate_u8((y11 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
        }
        y_buf0 += (ys << 1);
        y_buf1 += (ys << 1);
        u_buf += uvs;
        v_buf += uvs;
        dst0 += (ds << 1);
        dst1 += (ds << 1);
    }
}

void i420_to_bgra_c(const Image &src, const Image &dst)
{
    auto h = src.rows();
    if (0 != h % 2)
    {
        throw std::invalid_argument("Height must be even");
    }

    auto ys = static_cast<uint64_t>(src.cols());
    auto y_buf0 = src.data();
    auto y_buf1 = y_buf0 + ys;

    auto uvs = static_cast<uint64_t>(src.cols()) >> 1;
    auto u_buf = src.data() + src.pixels();
    auto v_buf = u_buf + (src.pixels() >> 2);

    auto ds = dst.stride();
    auto dst0 = dst.data();
    auto dst1 = dst0 + ds;

    for (auto i = 0; i < h; i += 2) {
        for (auto j = 0, k = 0, m = 0; j + 2 < ys; j += 2, k += 8, m += 1) {
            auto y00 = y_buf0[j + 0] << k_shift;
            auto y01 = y_buf0[j + 1] << k_shift;
            auto y10 = y_buf1[j + 0] << k_shift;
            auto y11 = y_buf1[j + 1] << k_shift;
            auto u = u_buf[m];
            auto v = v_buf[m];

            dst0[k + 0] = saturate_u8((y00 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
            dst0[k + 1] = saturate_u8((y00 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
            dst0[k + 2] = saturate_u8((y00 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
            dst0[k + 3] = k_alpha;
            dst0[k + 4] = saturate_u8((y01 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
            dst0[k + 5] = saturate_u8((y01 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
            dst0[k + 6] = saturate_u8((y01 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
            dst0[k + 7] = k_alpha;

            dst1[k + 0] = saturate_u8((y10 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
            dst1[k + 1] = saturate_u8((y10 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
            dst1[k + 2] = saturate_u8((y10 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
            dst1[k + 3] = k_alpha;
            dst1[k + 4] = saturate_u8((y11 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
            dst1[k + 5] = saturate_u8((y11 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
            dst1[k + 6] = saturate_u8((y11 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
            dst1[k + 7] = k_alpha;
        }
        y_buf0 += (ys << 1);
        y_buf1 += (ys << 1);
        u_buf += uvs;
        v_buf += uvs;
        dst0 += (ds << 1);
        dst1 += (ds << 1);
    }
}

void i420_to_bgr_c(const Image &src, const Image &dst)
{
    auto h = src.rows();
    if (0 != h % 2)
    {
        throw std::invalid_argument("Height must be even");
    }

    auto ys = static_cast<uint64_t>(src.cols());
    auto y_buf0 = src.data();
    auto y_buf1 = y_buf0 + ys;

    auto uvs = static_cast<uint64_t>(src.cols()) >> 1;
    auto u_buf = src.data() + src.pixels();
    auto v_buf = u_buf + (src.pixels() >> 2);

    auto ds = dst.stride();
    auto dst0 = dst.data();
    auto dst1 = dst0 + ds;

    for (auto i = 0; i < h; i += 2) {
        for (auto j = 0, k = 0, m = 0; j + 2 < ys; j += 2, k += 6, m += 1) {
            auto y00 = y_buf0[j + 0] << k_shift;
            auto y01 = y_buf0[j + 1] << k_shift;
            auto y10 = y_buf1[j + 0] << k_shift;
            auto y11 = y_buf1[j + 1] << k_shift;
            auto u = u_buf[m];
            auto v = v_buf[m];

            dst0[k + 0] = saturate_u8((y00 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
            dst0[k + 1] = saturate_u8((y00 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
            dst0[k + 2] = saturate_u8((y00 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
            dst0[k + 3] = saturate_u8((y01 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
            dst0[k + 4] = saturate_u8((y01 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
            dst0[k + 5] = saturate_u8((y01 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);

            dst1[k + 0] = saturate_u8((y10 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
            dst1[k + 1] = saturate_u8((y10 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
            dst1[k + 2] = saturate_u8((y10 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
            dst1[k + 3] = saturate_u8((y11 + k_yuv_2_rgb.m_ub * u - k_yuv_2_rgb.m_ofs_b) >> k_shift);
            dst1[k + 4] = saturate_u8((y11 - k_yuv_2_rgb.m_ug * u - k_yuv_2_rgb.m_vg * v + k_yuv_2_rgb.m_ofs_g) >> k_shift);
            dst1[k + 5] = saturate_u8((y11 + k_yuv_2_rgb.m_vr * v - k_yuv_2_rgb.m_ofs_r) >> k_shift);
        }
        y_buf0 += (ys << 1);
        y_buf1 += (ys << 1);
        u_buf += uvs;
        v_buf += uvs;
        dst0 += (ds << 1);
        dst1 += (ds << 1);
    }
}

void i420_to_yuyv_c(const Image &src, const Image &dst)
{
    auto h = src.rows();
    if (0 != h % 2)
    {
        throw std::invalid_argument("Height must be even");
    }

    auto ys = static_cast<uint64_t>(src.cols());
    auto y_buf0 = src.data();
    auto y_buf1 = y_buf0 + ys;

    auto uvs = static_cast<uint64_t>(src.cols()) >> 1;
    auto u_buf = src.data() + src.pixels();
    auto v_buf = u_buf + (src.pixels() >> 2);

    auto ds = dst.stride();
    auto dst0 = dst.data();
    auto dst1 = dst0 + ds;

    auto size = src.pixels();

    for (auto i = 0; i + 2 < h; i += 2) {
        for (auto j = 0, k = 0, m = 0; j + 2 < ys; j += 2, k += 4, m++) {
            auto y00 = y_buf0[j + 0];
            auto y01 = y_buf0[j + 1];
            auto y10 = y_buf1[j + 0];
            auto y11 = y_buf1[j + 1];
            auto u = u_buf[m];
            auto v = v_buf[m];

            dst0[k + 0] = y00;
            dst0[k + 1] = u;
            dst0[k + 2] = y01;
            dst0[k + 3] = v;
            dst1[k + 0] = y10;
            dst1[k + 1] = u;
            dst1[k + 2] = y11;
            dst1[k + 3] = v;
        }
        y_buf0 += (ys << 1);
        y_buf1 += (ys << 1);
        u_buf += uvs;
        v_buf += uvs;
        dst0 += (ds << 1);
        dst1 += (ds << 1);
    }
}

void i420_to_uyvy_c(const Image &src, const Image &dst)
{
    auto h = src.rows();
    if (0 != h % 2)
    {
        throw std::invalid_argument("Height must be even");
    }

    auto ys = static_cast<uint64_t>(src.cols());
    auto y_buf0 = src.data();
    auto y_buf1 = y_buf0 + ys;

    auto uvs = static_cast<uint64_t>(src.cols()) >> 1;
    auto u_buf = src.data() + src.pixels();
    auto v_buf = u_buf + (src.pixels() >> 2);

    auto ds = dst.stride();
    auto dst0 = dst.data();
    auto dst1 = dst0 + ds;

    auto size = src.pixels();

    for (auto i = 0; i + 2 < h; i += 2) {
        for (auto j = 0, k = 0, m = 0; j + 2 < ys; j += 2, k += 4, m++) {
            auto y00 = y_buf0[j + 0];
            auto y01 = y_buf0[j + 1];
            auto y10 = y_buf1[j + 0];
            auto y11 = y_buf1[j + 1];
            auto u = u_buf[m];
            auto v = v_buf[m];

            dst0[k + 0] = u;
            dst0[k + 1] = y00;
            dst0[k + 2] = v;
            dst0[k + 3] = y01;
            dst1[k + 0] = u;
            dst1[k + 1] = y10;
            dst1[k + 2] = v;
            dst1[k + 3] = y11;
        }
        y_buf0 += (ys << 1);
        y_buf1 += (ys << 1);
        u_buf += uvs;
        v_buf += uvs;
        dst0 += (ds << 1);
        dst1 += (ds << 1);
    }
}

void i420_to_i420_c(const Image &src, const Image &dst)
{
    auto size = src.size();
    auto src_buf = src.data();
    auto dst_buf = dst.data();
    std::copy_n(src_buf, size, dst_buf);
}

void i420_to_nv12_c(const Image &src, const Image &dst)
{
    auto h = src.rows();
    if (0 != h % 2)
    {
        throw std::invalid_argument("Height must be even");
    }

    auto src_buf = src.data();
    auto dst_buf = dst.data();

    auto size = src.pixels();

    std::copy_n(src_buf, size, dst_buf);

    auto uv_cnt = src.pixels() >> 1;
    auto u_buf = src.data() + src.pixels();
    auto v_buf = u_buf + (src.pixels() >> 2);
    auto uv_buf = dst.data() + dst.pixels();
    for (int i = 0, j = 0; i < uv_cnt; i += 2, j++) {
        uv_buf[i + 1] = v_buf[j];
        uv_buf[i + 0] = u_buf[j];
    }
}

void i420_to_nv21_c(const Image &src, const Image &dst)
{
    auto h = src.rows();
    if (0 != h % 2)
    {
        throw std::invalid_argument("Height must be even");
    }

    auto src_buf = src.data();
    auto dst_buf = dst.data();

    auto size = src.pixels();

    std::copy_n(src_buf, size, dst_buf);

    auto uv_cnt = src.pixels() >> 1;
    auto u_buf = src.data() + src.pixels();
    auto v_buf = u_buf + (src.pixels() >> 2);
    auto uv_buf = dst.data() + dst.pixels();
    for (int i = 0, j = 0; i < uv_cnt; i += 2, j++) {
        uv_buf[i + 1] = u_buf[j];
        uv_buf[i + 0] = v_buf[j];
    }
}

void i420_to_gray(const Image &src, const Image &dst)
{
    asm volatile(
        R"(
            and x0, %1, #15
            sub %1, %1, x0
            cbz %1, GRAY_L1

        GRAY_L16:
            ld1       {v0.16b}, [%0], #16
            prfm      pldl1keep, [%0, 448]
            st1       {v0.16b}, [%2], #16
            subs      %1, %1, #16
            bgt       GRAY_L16

        GRAY_L1:
            cmp       x0, #1
            blt       GRAY_END
            ld1       {v0.b}[0], [%0], #1
            st1       {v0.b}[1], [%2], #1
            sub       x0, x0, #1
            b GRAY_L1

        GRAY_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        : "memory", "cc", "x0", "v0");
}

void i420_to_rgba(const Image &src, const Image &dst)
{
    asm volatile(
        LOAD_RGB_PARAM
        R"(
            mov x1, %0
            add x2, x1, %1
            add x7, x2, %1, lsr #2

            mov x4, %2
            movi v9.16b, %[alpha]

        RGBA_H2:
            mov x0, x1
            add x1, x1, %[w]
            mov x3, x4
            add x4, x4, %[ds]
            cmp %[h], #2
            blt RGBA_END
            sub %[h], %[h], #2
            and x5, %[w], #15
            sub x6, %[w], x5
            cbz x6, RGBA_L2

        RGBA_L16:
            ld1 {v0.16b}, [x0], #16
            ushll2 v1.8h, v0.16b, %[shift]
            ushll v0.8h, v0.8b, %[shift]

            ld1 {v2.16b}, [x1], #16
            ushll2 v3.8h, v2.16b, %[shift]
            ushll v2.8h, v2.8b, %[shift]

            ld1 {v4.8b}, [x2], #8
            ld1 {v5.8b}, [x7], #8
            zip1 v4.16b, v4.16b, v4.16b
            zip1 v5.16b, v5.16b, v5.16b
            prfm pldl1keep, [x0, 448]

            // r0
            umull v6.8h, v5.8b, v28.8b
            umull2 v10.8h, v5.16b, v28.16b
            add v6.8h, v6.8h, v0.8h
            add v10.8h, v10.8h, v1.8h
            uqsub v6.8h, v6.8h, v25.8h
            uqsub v10.8h, v10.8h, v25.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v10.8h, %[shift]

            // g0
            umull v7.8h, v4.8b, v29.8b
            umull2 v11.8h, v4.16b, v29.16b
            umlal v7.8h, v5.8b, v30.8b
            umlal2 v11.8h, v5.16b, v30.16b
            add v12.8h, v0.8h, v26.8h
            add v13.8h, v1.8h, v26.8h
            uqsub v7.8h, v12.8h, v7.8h
            uqsub v11.8h, v13.8h, v11.8h
            uqrshrn v7.8b, v7.8h, %[shift]
            uqrshrn2 v7.16b, v11.8h, %[shift]
            prfm pldl1keep, [x1, 448]

            // b0
            umull v8.8h, v4.8b, v31.8b
            umull2 v12.8h, v4.16b, v31.16b
            add v8.8h, v8.8h, v0.8h
            add v12.8h, v12.8h, v1.8h
            uqsub v8.8h, v8.8h, v27.8h
            uqsub v12.8h, v12.8h, v27.8h
            uqrshrn v8.8b, v8.8h, %[shift]
            uqrshrn2 v8.16b, v12.8h, %[shift]
            st4 {v6.16b, v7.16b, v8.16b, v9.16b}, [x3], #64

            // r1
            umull v6.8h, v5.8b, v28.8b
            umull2 v10.8h, v5.16b, v28.16b
            add v6.8h, v6.8h, v2.8h
            add v10.8h, v10.8h, v3.8h
            uqsub v6.8h, v6.8h, v25.8h
            uqsub v10.8h, v10.8h, v25.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v10.8h, %[shift]
            prfm pldl1keep, [x2, 448]

            // g1
            umull v7.8h, v4.8b, v29.8b
            umull2 v11.8h, v4.16b, v29.16b
            umlal v7.8h, v5.8b, v30.8b
            umlal2 v11.8h, v5.16b, v30.16b
            add v12.8h, v2.8h, v26.8h
            add v13.8h, v3.8h, v26.8h
            uqsub v7.8h, v12.8h, v7.8h
            uqsub v11.8h, v13.8h, v11.8h
            uqrshrn v7.8b, v7.8h, %[shift]
            uqrshrn2 v7.16b, v11.8h, %[shift]

            // b1
            umull v8.8h, v4.8b, v31.8b
            umull2 v12.8h, v4.16b, v31.16b
            add v8.8h, v8.8h, v2.8h
            add v12.8h, v12.8h, v3.8h
            uqsub v8.8h, v8.8h, v27.8h
            uqsub v12.8h, v12.8h, v27.8h
            uqrshrn v8.8b, v8.8h, %[shift]
            uqrshrn2 v8.16b, v12.8h, %[shift]

            subs x6, x6, #16
            st4 {v6.16b, v7.16b, v8.16b, v9.16b}, [x4], #64

            bgt RGBA_L16

        RGBA_L2:
            cmp x5, #2
            blt RGBA_H2

            ld1 {v0.h}[0], [x0], #2
            ushll2 v1.8h, v0.16b, %[shift]
            ushll v0.8h, v0.8b, %[shift]

            ld1 {v2.h}[0], [x1], #2
            ushll2 v3.8h, v2.16b, %[shift]
            ushll v2.8h, v2.8b, %[shift]

            ld1 {v4.b}[0], [x2], #1
            ld1 {v5.b}[0], [x7], #1
            zip1 v4.16b, v4.16b, v4.16b
            zip1 v5.16b, v5.16b, v5.16b

            // r0
            umull v6.8h, v5.8b, v28.8b
            umull2 v10.8h, v5.16b, v28.16b
            add v6.8h, v6.8h, v0.8h
            add v10.8h, v10.8h, v1.8h
            uqsub v6.8h, v6.8h, v25.8h
            uqsub v10.8h, v10.8h, v25.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v10.8h, %[shift]

            // g0
            umull v7.8h, v4.8b, v29.8b
            umull2 v11.8h, v4.16b, v29.16b
            umlal v7.8h, v5.8b, v30.8b
            umlal2 v11.8h, v5.16b, v30.16b
            add v12.8h, v0.8h, v26.8h
            add v13.8h, v1.8h, v26.8h
            uqsub v7.8h, v12.8h, v7.8h
            uqsub v11.8h, v13.8h, v11.8h
            uqrshrn v7.8b, v7.8h, %[shift]
            uqrshrn2 v7.16b, v11.8h, %[shift]

            // b0
            umull v8.8h, v4.8b, v31.8b
            umull2 v12.8h, v4.16b, v31.16b
            add v8.8h, v8.8h, v0.8h
            add v12.8h, v12.8h, v1.8h
            uqsub v8.8h, v8.8h, v27.8h
            uqsub v12.8h, v12.8h, v27.8h
            uqrshrn v8.8b, v8.8h, %[shift]
            uqrshrn2 v8.16b, v12.8h, %[shift]
            st4 {v6.b, v7.b, v8.b, v9.b}[0], [x3], #4
            st4 {v6.b, v7.b, v8.b, v9.b}[1], [x3], #4

            // r1
            umull v6.8h, v5.8b, v28.8b
            umull2 v10.8h, v5.16b, v28.16b
            add v6.8h, v6.8h, v2.8h
            add v10.8h, v10.8h, v3.8h
            uqsub v6.8h, v6.8h, v25.8h
            uqsub v10.8h, v10.8h, v25.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v10.8h, %[shift]

            // g1
            umull v7.8h, v4.8b, v29.8b
            umull2 v11.8h, v4.16b, v29.16b
            umlal v7.8h, v5.8b, v30.8b
            umlal2 v11.8h, v5.16b, v30.16b
            add v12.8h, v2.8h, v26.8h
            add v13.8h, v3.8h, v26.8h
            uqsub v7.8h, v12.8h, v7.8h
            uqsub v11.8h, v13.8h, v11.8h
            uqrshrn v7.8b, v7.8h, %[shift]
            uqrshrn2 v7.16b, v11.8h, %[shift]

            // b1
            umull v8.8h, v4.8b, v31.8b
            umull2 v12.8h, v4.16b, v31.16b
            add v8.8h, v8.8h, v2.8h
            add v12.8h, v12.8h, v3.8h
            uqsub v8.8h, v8.8h, v27.8h
            uqsub v12.8h, v12.8h, v27.8h
            uqrshrn v8.8b, v8.8h, %[shift]
            uqrshrn2 v8.16b, v12.8h, %[shift]
            st4 {v6.b, v7.b, v8.b, v9.b}[0], [x4], #4
            st4 {v6.b, v7.b, v8.b, v9.b}[1], [x4], #4

            sub x5, x5, #2
            b RGBA_L2
        RGBA_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [h]"r"(1L * src.rows()), [w]"r"(1L * src.cols()), [ds]"r"(dst.stride())
        , [param]"r"(&k_yuv_2_rgb), [alpha] "I"(k_alpha), [shift]"I"(k_shift)
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7"
        , "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9");
}

void i420_to_rgb(const Image &src, const Image &dst)
{
    asm volatile(
        LOAD_RGB_PARAM
        R"(
            mov x1, %0
            add x2, x1, %1
            mov x4, %2
            add x7, x2, %1, lsr #2

        RGB_H2:
            mov x0, x1
            add x1, x1, %[w]
            mov x3, x4
            add x4, x4, %[ds]
            cmp %[h], #2
            blt RGB_END
            sub %[h], %[h], #2
            and x5, %[w], #15
            sub x6, %[w], x5
            cbz x6, RGB_L2

        RGB_L16:
            ld1 {v0.16b}, [x0], #16
            ushll2 v1.8h, v0.16b, %[shift]
            ushll v0.8h, v0.8b, %[shift]

            ld1 {v2.16b}, [x1], #16
            ushll2 v3.8h, v2.16b, %[shift]
            ushll v2.8h, v2.8b, %[shift]

            ld1 {v4.8b}, [x2], #8
            ld1 {v5.8b}, [x7], #8
            zip1 v4.16b, v4.16b, v4.16b
            zip1 v5.16b, v5.16b, v5.16b
            prfm pldl1keep, [x0, 448]

            // r0
            umull v6.8h, v5.8b, v28.8b
            umull2 v10.8h, v5.16b, v28.16b
            add v6.8h, v6.8h, v0.8h
            add v10.8h, v10.8h, v1.8h
            uqsub v6.8h, v6.8h, v25.8h
            uqsub v10.8h, v10.8h, v25.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v10.8h, %[shift]

            // g0
            umull v7.8h, v4.8b, v29.8b
            umull2 v11.8h, v4.16b, v29.16b
            umlal v7.8h, v5.8b, v30.8b
            umlal2 v11.8h, v5.16b, v30.16b
            add v12.8h, v0.8h, v26.8h
            add v13.8h, v1.8h, v26.8h
            uqsub v7.8h, v12.8h, v7.8h
            uqsub v11.8h, v13.8h, v11.8h
            uqrshrn v7.8b, v7.8h, %[shift]
            uqrshrn2 v7.16b, v11.8h, %[shift]
            prfm pldl1keep, [x1, 448]

            // b0
            umull v8.8h, v4.8b, v31.8b
            umull2 v12.8h, v4.16b, v31.16b
            add v8.8h, v8.8h, v0.8h
            add v12.8h, v12.8h, v1.8h
            uqsub v8.8h, v8.8h, v27.8h
            uqsub v12.8h, v12.8h, v27.8h
            uqrshrn v8.8b, v8.8h, %[shift]
            uqrshrn2 v8.16b, v12.8h, %[shift]
            st3 {v6.16b, v7.16b, v8.16b}, [x3], #48

            // r1
            umull v6.8h, v5.8b, v28.8b
            umull2 v10.8h, v5.16b, v28.16b
            add v6.8h, v6.8h, v2.8h
            add v10.8h, v10.8h, v3.8h
            uqsub v6.8h, v6.8h, v25.8h
            uqsub v10.8h, v10.8h, v25.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v10.8h, %[shift]
            prfm pldl1keep, [x2, 448]

            // g1
            umull v7.8h, v4.8b, v29.8b
            umull2 v11.8h, v4.16b, v29.16b
            umlal v7.8h, v5.8b, v30.8b
            umlal2 v11.8h, v5.16b, v30.16b
            add v12.8h, v2.8h, v26.8h
            add v13.8h, v3.8h, v26.8h
            uqsub v7.8h, v12.8h, v7.8h
            uqsub v11.8h, v13.8h, v11.8h
            uqrshrn v7.8b, v7.8h, %[shift]
            uqrshrn2 v7.16b, v11.8h, %[shift]

            // b1
            umull v8.8h, v4.8b, v31.8b
            umull2 v12.8h, v4.16b, v31.16b
            add v8.8h, v8.8h, v2.8h
            add v12.8h, v12.8h, v3.8h
            uqsub v8.8h, v8.8h, v27.8h
            uqsub v12.8h, v12.8h, v27.8h
            uqrshrn v8.8b, v8.8h, %[shift]
            uqrshrn2 v8.16b, v12.8h, %[shift]

            subs x6, x6, #16
            st3 {v6.16b, v7.16b, v8.16b}, [x4], #48

            bgt RGB_L16

        RGB_L2:
            cmp x5, #2
            blt RGB_H2

            ld1 {v0.h}[0], [x0], #2
            ushll2 v1.8h, v0.16b, %[shift]
            ushll v0.8h, v0.8b, %[shift]

            ld1 {v2.h}[0], [x1], #2
            ushll2 v3.8h, v2.16b, %[shift]
            ushll v2.8h, v2.8b, %[shift]

            ld1 {v4.b}[0], [x2], #1
            ld1 {v5.b}[0], [x7], #1
            zip1 v4.16b, v4.16b, v4.16b
            zip1 v5.16b, v5.16b, v5.16b

            // r0
            umull v6.8h, v5.8b, v28.8b
            umull2 v10.8h, v5.16b, v28.16b
            add v6.8h, v6.8h, v0.8h
            add v10.8h, v10.8h, v1.8h
            uqsub v6.8h, v6.8h, v25.8h
            uqsub v10.8h, v10.8h, v25.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v10.8h, %[shift]

            // g0
            umull v7.8h, v4.8b, v29.8b
            umull2 v11.8h, v4.16b, v29.16b
            umlal v7.8h, v5.8b, v30.8b
            umlal2 v11.8h, v5.16b, v30.16b
            add v12.8h, v0.8h, v26.8h
            add v13.8h, v1.8h, v26.8h
            uqsub v7.8h, v12.8h, v7.8h
            uqsub v11.8h, v13.8h, v11.8h
            uqrshrn v7.8b, v7.8h, %[shift]
            uqrshrn2 v7.16b, v11.8h, %[shift]

            // b0
            umull v8.8h, v4.8b, v31.8b
            umull2 v12.8h, v4.16b, v31.16b
            add v8.8h, v8.8h, v0.8h
            add v12.8h, v12.8h, v1.8h
            uqsub v8.8h, v8.8h, v27.8h
            uqsub v12.8h, v12.8h, v27.8h
            uqrshrn v8.8b, v8.8h, %[shift]
            uqrshrn2 v8.16b, v12.8h, %[shift]
            st3 {v6.b, v7.b, v8.b}[0], [x3], #3
            st3 {v6.b, v7.b, v8.b}[1], [x3], #3

            // r1
            umull v6.8h, v5.8b, v28.8b
            umull2 v10.8h, v5.16b, v28.16b
            add v6.8h, v6.8h, v2.8h
            add v10.8h, v10.8h, v3.8h
            uqsub v6.8h, v6.8h, v25.8h
            uqsub v10.8h, v10.8h, v25.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v10.8h, %[shift]

            // g1
            umull v7.8h, v4.8b, v29.8b
            umull2 v11.8h, v4.16b, v29.16b
            umlal v7.8h, v5.8b, v30.8b
            umlal2 v11.8h, v5.16b, v30.16b
            add v12.8h, v2.8h, v26.8h
            add v13.8h, v3.8h, v26.8h
            uqsub v7.8h, v12.8h, v7.8h
            uqsub v11.8h, v13.8h, v11.8h
            uqrshrn v7.8b, v7.8h, %[shift]
            uqrshrn2 v7.16b, v11.8h, %[shift]

            // b1
            umull v8.8h, v4.8b, v31.8b
            umull2 v12.8h, v4.16b, v31.16b
            add v8.8h, v8.8h, v2.8h
            add v12.8h, v12.8h, v3.8h
            uqsub v8.8h, v8.8h, v27.8h
            uqsub v12.8h, v12.8h, v27.8h
            uqrshrn v8.8b, v8.8h, %[shift]
            uqrshrn2 v8.16b, v12.8h, %[shift]
            st3 {v6.b, v7.b, v8.b}[0], [x4], #3
            st3 {v6.b, v7.b, v8.b}[1], [x4], #3

            sub x5, x5, #2
            b RGB_L2
        RGB_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [h]"r"(1L * src.rows()), [w]"r"(1L * src.cols()), [ds]"r"(dst.stride())
        , [param]"r"(&k_yuv_2_rgb), [shift]"I"(k_shift)
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7"
        , "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9");
}

void i420_to_bgr(const Image &src, const Image &dst)
{
    asm volatile(
        LOAD_RGB_PARAM
        R"(
            mov x1, %0
            add x2, x1, %1
            mov x4, %2
            add x7, x2, %1, lsr #2

        BGR_H2:
            mov x0, x1
            add x1, x1, %[w]
            mov x3, x4
            add x4, x4, %[ds]
            cmp %[h], #2
            blt BGR_END
            sub %[h], %[h], #2
            and x5, %[w], #15
            sub x6, %[w], x5
            cbz x6, BGR_L2

        BGR_L16:
            ld1 {v0.16b}, [x0], #16
            ushll2 v1.8h, v0.16b, %[shift]
            ushll v0.8h, v0.8b, %[shift]

            ld1 {v2.16b}, [x1], #16
            ushll2 v3.8h, v2.16b, %[shift]
            ushll v2.8h, v2.8b, %[shift]

            ld1 {v4.8b}, [x2], #8
            ld1 {v5.8b}, [x7], #8
            zip1 v4.16b, v4.16b, v4.16b
            zip1 v5.16b, v5.16b, v5.16b
            prfm pldl1keep, [x0, 448]

            // r0
            umull v8.8h, v5.8b, v28.8b
            umull2 v10.8h, v5.16b, v28.16b
            add v8.8h, v8.8h, v0.8h
            add v10.8h, v10.8h, v1.8h
            uqsub v8.8h, v8.8h, v25.8h
            uqsub v10.8h, v10.8h, v25.8h
            uqrshrn v8.8b, v8.8h, %[shift]
            uqrshrn2 v8.16b, v10.8h, %[shift]

            // g0
            umull v7.8h, v4.8b, v29.8b
            umull2 v11.8h, v4.16b, v29.16b
            umlal v7.8h, v5.8b, v30.8b
            umlal2 v11.8h, v5.16b, v30.16b
            add v12.8h, v0.8h, v26.8h
            add v13.8h, v1.8h, v26.8h
            uqsub v7.8h, v12.8h, v7.8h
            uqsub v11.8h, v13.8h, v11.8h
            uqrshrn v7.8b, v7.8h, %[shift]
            uqrshrn2 v7.16b, v11.8h, %[shift]
            prfm pldl1keep, [x1, 448]

            // b0
            umull v6.8h, v4.8b, v31.8b
            umull2 v12.8h, v4.16b, v31.16b
            add v6.8h, v6.8h, v0.8h
            add v12.8h, v12.8h, v1.8h
            uqsub v6.8h, v6.8h, v27.8h
            uqsub v12.8h, v12.8h, v27.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v12.8h, %[shift]
            st3 {v6.16b, v7.16b, v8.16b}, [x3], #48

            // r1
            umull v8.8h, v5.8b, v28.8b
            umull2 v10.8h, v5.16b, v28.16b
            add v8.8h, v8.8h, v2.8h
            add v10.8h, v10.8h, v3.8h
            uqsub v8.8h, v8.8h, v25.8h
            uqsub v10.8h, v10.8h, v25.8h
            uqrshrn v8.8b, v8.8h, %[shift]
            uqrshrn2 v8.16b, v10.8h, %[shift]
            prfm pldl1keep, [x2, 448]

            // g1
            umull v7.8h, v4.8b, v29.8b
            umull2 v11.8h, v4.16b, v29.16b
            umlal v7.8h, v5.8b, v30.8b
            umlal2 v11.8h, v5.16b, v30.16b
            add v12.8h, v2.8h, v26.8h
            add v13.8h, v3.8h, v26.8h
            uqsub v7.8h, v12.8h, v7.8h
            uqsub v11.8h, v13.8h, v11.8h
            uqrshrn v7.8b, v7.8h, %[shift]
            uqrshrn2 v7.16b, v11.8h, %[shift]

            // b1
            umull v6.8h, v4.8b, v31.8b
            umull2 v12.8h, v4.16b, v31.16b
            add v6.8h, v6.8h, v2.8h
            add v12.8h, v12.8h, v3.8h
            uqsub v6.8h, v6.8h, v27.8h
            uqsub v12.8h, v12.8h, v27.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v12.8h, %[shift]

            subs x6, x6, #16
            st3 {v6.16b, v7.16b, v8.16b}, [x4], #48

            bgt BGR_L16

        BGR_L2:
            cmp x5, #2
            blt BGR_H2

            ld1 {v0.h}[0], [x0], #2
            ushll2 v1.8h, v0.16b, %[shift]
            ushll v0.8h, v0.8b, %[shift]

            ld1 {v2.h}[0], [x1], #2
            ushll2 v3.8h, v2.16b, %[shift]
            ushll v2.8h, v2.8b, %[shift]

            ld1 {v4.b}[0], [x2], #1
            ld1 {v5.b}[0], [x7], #1
            zip1 v4.16b, v4.16b, v4.16b
            zip1 v5.16b, v5.16b, v5.16b

            // r0
            umull v8.8h, v5.8b, v28.8b
            umull2 v10.8h, v5.16b, v28.16b
            add v8.8h, v8.8h, v0.8h
            add v10.8h, v10.8h, v1.8h
            uqsub v8.8h, v8.8h, v25.8h
            uqsub v10.8h, v10.8h, v25.8h
            uqrshrn v8.8b, v8.8h, %[shift]
            uqrshrn2 v8.16b, v10.8h, %[shift]

            // g0
            umull v7.8h, v4.8b, v29.8b
            umull2 v11.8h, v4.16b, v29.16b
            umlal v7.8h, v5.8b, v30.8b
            umlal2 v11.8h, v5.16b, v30.16b
            add v12.8h, v0.8h, v26.8h
            add v13.8h, v1.8h, v26.8h
            uqsub v7.8h, v12.8h, v7.8h
            uqsub v11.8h, v13.8h, v11.8h
            uqrshrn v7.8b, v7.8h, %[shift]
            uqrshrn2 v7.16b, v11.8h, %[shift]

            // b0
            umull v6.8h, v4.8b, v31.8b
            umull2 v12.8h, v4.16b, v31.16b
            add v6.8h, v6.8h, v0.8h
            add v12.8h, v12.8h, v1.8h
            uqsub v6.8h, v6.8h, v27.8h
            uqsub v12.8h, v12.8h, v27.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v12.8h, %[shift]
            st3 {v6.b, v7.b, v8.b}[0], [x3], #3
            st3 {v6.b, v7.b, v8.b}[1], [x3], #3

            // r1
            umull v8.8h, v5.8b, v28.8b
            umull2 v10.8h, v5.16b, v28.16b
            add v8.8h, v8.8h, v2.8h
            add v10.8h, v10.8h, v3.8h
            uqsub v8.8h, v8.8h, v25.8h
            uqsub v10.8h, v10.8h, v25.8h
            uqrshrn v8.8b, v8.8h, %[shift]
            uqrshrn2 v8.16b, v10.8h, %[shift]

            // g1
            umull v7.8h, v4.8b, v29.8b
            umull2 v11.8h, v4.16b, v29.16b
            umlal v7.8h, v5.8b, v30.8b
            umlal2 v11.8h, v5.16b, v30.16b
            add v12.8h, v2.8h, v26.8h
            add v13.8h, v3.8h, v26.8h
            uqsub v7.8h, v12.8h, v7.8h
            uqsub v11.8h, v13.8h, v11.8h
            uqrshrn v7.8b, v7.8h, %[shift]
            uqrshrn2 v7.16b, v11.8h, %[shift]

            // b1
            umull v6.8h, v4.8b, v31.8b
            umull2 v12.8h, v4.16b, v31.16b
            add v6.8h, v6.8h, v2.8h
            add v12.8h, v12.8h, v3.8h
            uqsub v6.8h, v6.8h, v27.8h
            uqsub v12.8h, v12.8h, v27.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v12.8h, %[shift]
            st3 {v6.b, v7.b, v8.b}[0], [x4], #3
            st3 {v6.b, v7.b, v8.b}[1], [x4], #3

            sub x5, x5, #2
            b BGR_L2
        BGR_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [h]"r"(1L * src.rows()), [w]"r"(1L * src.cols()), [ds]"r"(dst.stride())
        , [param]"r"(&k_yuv_2_rgb), [shift]"I"(k_shift)
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7"
        , "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9");
}

void i420_to_bgra(const Image &src, const Image &dst)
{
    asm volatile(
        LOAD_RGB_PARAM
        R"(
            mov x1, %0
            add x2, x1, %1
            movi v9.16b, %[alpha]
            mov x4, %2
            add x7, x2, %1, lsr #2

        BGRA_H2:
            mov x0, x1
            add x1, x1, %[w]
            mov x3, x4
            add x4, x4, %[ds]
            cmp %[h], #2
            blt BGRA_END
            sub %[h], %[h], #2
            and x5, %[w], #15
            sub x6, %[w], x5
            cbz x6, BGRA_L2

        BGRA_L16:
            ld1 {v0.16b}, [x0], #16
            ushll2 v1.8h, v0.16b, %[shift]
            ushll v0.8h, v0.8b, %[shift]

            ld1 {v2.16b}, [x1], #16
            ushll2 v3.8h, v2.16b, %[shift]
            ushll v2.8h, v2.8b, %[shift]

            ld1 {v4.8b}, [x2], #8
            ld1 {v5.8b}, [x7], #8
            zip1 v4.16b, v4.16b, v4.16b
            zip1 v5.16b, v5.16b, v5.16b
            prfm pldl1keep, [x0, #448]

            // r0
            umull v8.8h, v5.8b, v28.8b
            umull2 v10.8h, v5.16b, v28.16b
            add v8.8h, v8.8h, v0.8h
            add v10.8h, v10.8h, v1.8h
            uqsub v8.8h, v8.8h, v25.8h
            uqsub v10.8h, v10.8h, v25.8h
            uqrshrn v8.8b, v8.8h, %[shift]
            uqrshrn2 v8.16b, v10.8h, %[shift]

            // g0
            umull v7.8h, v4.8b, v29.8b
            umull2 v11.8h, v4.16b, v29.16b
            umlal v7.8h, v5.8b, v30.8b
            umlal2 v11.8h, v5.16b, v30.16b
            add v12.8h, v0.8h, v26.8h
            add v13.8h, v1.8h, v26.8h
            uqsub v7.8h, v12.8h, v7.8h
            uqsub v11.8h, v13.8h, v11.8h
            uqrshrn v7.8b, v7.8h, %[shift]
            uqrshrn2 v7.16b, v11.8h, %[shift]
            prfm pldl1keep, [x1, #448]

            // b0
            umull v6.8h, v4.8b, v31.8b
            umull2 v12.8h, v4.16b, v31.16b
            add v6.8h, v6.8h, v0.8h
            add v12.8h, v12.8h, v1.8h
            uqsub v6.8h, v6.8h, v27.8h
            uqsub v12.8h, v12.8h, v27.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v12.8h, %[shift]
            st4 {v6.16b, v7.16b, v8.16b, v9.16b}, [x3], #64

            // r1
            umull v8.8h, v5.8b, v28.8b
            umull2 v10.8h, v5.16b, v28.16b
            add v8.8h, v8.8h, v2.8h
            add v10.8h, v10.8h, v3.8h
            uqsub v8.8h, v8.8h, v25.8h
            uqsub v10.8h, v10.8h, v25.8h
            uqrshrn v8.8b, v8.8h, %[shift]
            uqrshrn2 v8.16b, v10.8h, %[shift]
            prfm pldl1keep, [x2, 448]

            // g1
            umull v7.8h, v4.8b, v29.8b
            umull2 v11.8h, v4.16b, v29.16b
            umlal v7.8h, v5.8b, v30.8b
            umlal2 v11.8h, v5.16b, v30.16b
            add v12.8h, v2.8h, v26.8h
            add v13.8h, v3.8h, v26.8h
            uqsub v7.8h, v12.8h, v7.8h
            uqsub v11.8h, v13.8h, v11.8h
            uqrshrn v7.8b, v7.8h, %[shift]
            uqrshrn2 v7.16b, v11.8h, %[shift]

            // b1
            umull v6.8h, v4.8b, v31.8b
            umull2 v12.8h, v4.16b, v31.16b
            add v6.8h, v6.8h, v2.8h
            add v12.8h, v12.8h, v3.8h
            uqsub v6.8h, v6.8h, v27.8h
            uqsub v12.8h, v12.8h, v27.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v12.8h, %[shift]

            subs x6, x6, #16
            st4 {v6.16b, v7.16b, v8.16b, v9.16b}, [x4], #64

            bgt BGRA_L16

        BGRA_L2:
            cmp x5, #2
            blt BGRA_H2

            ld1 {v0.h}[0], [x0], #2
            ushll2 v1.8h, v0.16b, %[shift]
            ushll v0.8h, v0.8b, %[shift]

            ld1 {v2.h}[0], [x1], #2
            ushll2 v3.8h, v2.16b, %[shift]
            ushll v2.8h, v2.8b, %[shift]

            ld1 {v4.b}[0], [x2], #1
            ld1 {v5.b}[0], [x7], #1
            zip1 v4.16b, v4.16b, v4.16b
            zip1 v5.16b, v5.16b, v5.16b

            // r0
            umull v8.8h, v5.8b, v28.8b
            umull2 v10.8h, v5.16b, v28.16b
            add v8.8h, v8.8h, v0.8h
            add v10.8h, v10.8h, v1.8h
            uqsub v8.8h, v8.8h, v25.8h
            uqsub v10.8h, v10.8h, v25.8h
            uqrshrn v8.8b, v8.8h, %[shift]
            uqrshrn2 v8.16b, v10.8h, %[shift]

            // g0
            umull v7.8h, v4.8b, v29.8b
            umull2 v11.8h, v4.16b, v29.16b
            umlal v7.8h, v5.8b, v30.8b
            umlal2 v11.8h, v5.16b, v30.16b
            add v12.8h, v0.8h, v26.8h
            add v13.8h, v1.8h, v26.8h
            uqsub v7.8h, v11.8h, v7.8h
            uqsub v11.8h, v13.8h, v11.8h
            uqrshrn v7.8b, v7.8h, %[shift]
            uqrshrn2 v7.16b, v11.8h, %[shift]

            // b0
            umull v6.8h, v4.8b, v31.8b
            umull2 v12.8h, v4.16b, v31.16b
            add v6.8h, v6.8h, v0.8h
            add v12.8h, v12.8h, v1.8h
            uqsub v6.8h, v6.8h, v27.8h
            uqsub v12.8h, v12.8h, v27.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v12.8h, %[shift]
            st4 {v6.b, v7.b, v8.b, v9.b}[0], [x3], #4
            st4 {v6.b, v7.b, v8.b, v9.b}[1], [x3], #4

            // r1
            umull v8.8h, v5.8b, v28.8b
            umull2 v10.8h, v5.16b, v28.16b
            add v8.8h, v8.8h, v2.8h
            add v10.8h, v10.8h, v3.8h
            uqsub v8.8h, v8.8h, v25.8h
            uqsub v10.8h, v10.8h, v25.8h
            uqrshrn v8.8b, v8.8h, %[shift]
            uqrshrn2 v8.16b, v10.8h, %[shift]

            // g1
            umull v7.8h, v4.8b, v29.8b
            umull2 v11.8h, v4.16b, v29.16b
            umlal v7.8h, v5.8b, v30.8b
            umlal2 v11.8h, v5.16b, v30.16b
            add v12.8h, v2.8h, v26.8h
            add v13.8h, v3.8h, v26.8h
            uqsub v7.8h, v12.8h, v7.8h
            uqsub v11.8h, v13.8h, v11.8h
            uqrshrn v7.8b, v7.8h, %[shift]
            uqrshrn2 v7.16b, v11.8h, %[shift]

            // b1
            umull v6.8h, v4.8b, v31.8b
            umull2 v12.8h, v4.16b, v31.16b
            add v6.8h, v6.8h, v2.8h
            add v12.8h, v12.8h, v3.8h
            uqsub v6.8h, v6.8h, v27.8h
            uqsub v12.8h, v12.8h, v27.8h
            uqrshrn v6.8b, v6.8h, %[shift]
            uqrshrn2 v6.16b, v12.8h, %[shift]
            st4 {v6.b, v7.b, v8.b, v9.b}[0], [x4], #4
            st4 {v6.b, v7.b, v8.b, v9.b}[1], [x4], #4

            sub x5, x5, #2
            b BGRA_L2
        BGRA_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [h]"r"(1L * src.rows()), [w]"r"(1L * src.cols()), [ds]"r"(dst.stride())
        , [param]"r"(&k_yuv_2_rgb), [alpha]"I"(k_alpha), [shift]"I"(k_shift)
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7"
        , "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9");
}

void i420_to_yuyv(const Image &src, const Image &dst)
{
    asm volatile(
        R"(
            add x2, %0, %1
            mov x1, %0
            mov x6, %2
            add x7, x2, %1, lsr #2
        YUYV_H2:
            cmp %[h], #2
            blt YUYV_END
            sub %[h], %[h], #2
            mov x0, x1
            add x1, x1, %[w]
            mov x5, x6
            add x6, x6, %[ds]
            and x4, %[w], #15
            sub x3, %[w], x4
            cbz x3, YUYV_L2

            YUYV_L16:
                ld1 {v0.16b}, [x0], #16
                ld1 {v1.8b}, [x2], #8
                ld1 {v3.8b}, [x7], #8
                zip1 v1.16b, v1.16b, v3.16b
                mov v3.16b, v1.16b
                prfm pldl1keep, [x0, #448]
                ld1 {v2.16b}, [x1], #16
                st2 {v0.16b, v1.16b}, [x5], #32
                prfm pldl1keep, [x2, #448]
                prfm pldl1keep, [x7, #448]
                st2 {v2.16b, v3.16b}, [x6], #32
                subs x3, x3, #16
                prfm pldl1keep, [x1, #448]
                bgt YUYV_L16

            YUYV_L2:
                cmp x4, #2
                blt YUYV_H2
                ld1 {v0.h}[0], [x0], #2
                ld1 {v1.b}[0], [x2], #1
                ld1 {v3.b}[0], [x7], #1
                zip1 v1.16b, v1.16b, v3.16b
                mov v3.16b, v1.16b
                ld1 {v2.h}[0], [x1], #2
                st2 {v0.b, v1.b}[0], [x5], #2
                st2 {v0.b, v1.b}[1], [x5], #2
                sub x4, x4, #2
                st2 {v2.b, v3.b}[0], [x6], #2
                st2 {v2.b, v3.b}[1], [x6], #2
                b YUYV_L2
            YUYV_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [h]"r"(1L * src.rows()), [w]"r"(1L * src.cols()), [ds]"r"(dst.stride())
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7"
        , "v0", "v1", "v2", "v3");
}

void i420_to_uyvy(const Image &src, const Image &dst)
{
    asm volatile(
        R"(
            add x2, %0, %1
            mov x1, %0
            mov x6, %2
            add x7, x2, %1, lsr #2
        UYVY_H2:
            cmp %[h], #2
            blt UYVY_END
            sub %[h], %[h], #2
            mov x0, x1
            add x1, x1, %[w]
            mov x5, x6
            add x6, x6, %[ds]
            and x4, %[w], #15
            sub x3, %[w], x4
            cbz x3, UYVY_L2

            UYVY_L16:
                ld1 {v1.16b}, [x0], #16
                ld1 {v0.8b}, [x2], #8
                ld1 {v2.8b}, [x7], #8
                zip1 v0.16b, v0.16b, v2.16b
                mov v2.16b, v0.16b
                prfm pldl1keep, [x0, #448]
                ld1 {v3.16b}, [x1], #16
                st2 {v0.16b, v1.16b}, [x5], #32
                prfm pldl1keep, [x2, #448]
                st2 {v2.16b, v3.16b}, [x6], #32
                prfm pldl1keep, [x7, #448]
                subs x3, x3, #16
                prfm pldl1keep, [x1, #448]
                bgt UYVY_L16

            UYVY_L2:
                cmp x4, #2
                blt UYVY_H2
                ld1 {v1.h}[0], [x0], #2
                ld1 {v0.b}[0], [x2], #1
                ld1 {v2.b}[0], [x7], #1
                zip1 v0.16b, v0.16b, v2.16b
                mov v2.16b, v0.16b
                ld1 {v3.h}[0], [x1], #2
                st2 {v0.b, v1.b}[0], [x5], #2
                st2 {v0.b, v1.b}[1], [x5], #2
                sub x4, x4, #2
                st2 {v2.b, v3.b}[0], [x6], #2
                st2 {v2.b, v3.b}[1], [x6], #2
                b UYVY_L2
            UYVY_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        , [h]"r"(1L * src.rows()), [w]"r"(1L * src.cols()), [ds]"r"(dst.stride())
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7"
        , "v0", "v1", "v2", "v3");
}

void i420_to_i420(const Image &src, const Image &dst)
{
    asm volatile(
        R"(
            and x0, %1, #31
            sub %1, %1, x0
            cbz %1, I420_L2
        I420_L32:
            ld1 {v0.16b, v1.16b}, [%0], #32
            prfm pldl1keep, [%0, #448]
            st1 {v0.16b, v1.16b}, [%2], #32
            subs %1, %1, #32
            bgt I420_L32

        I420_L2:
            cmp x0, #2
            blt I420_END
            sub x0, x0, #2
            ld1 {v0.h}[0], [%0], #2
            st1 {v0.h}[0], [%2], #2
            b I420_L2

        I420_END:
        )"
        :
        : "r"(src.data()), "r"(src.size()), "r"(dst.data())
        : "cc", "memory", "x0"
        , "v0", "v1");
}

void i420_to_nv12(const Image &src, const Image &dst)
{
    asm volatile(
        R"(
            mov x0, %1
            and x1, %1, #15
            sub x0, x0, x1
            cbz x0, NV12_L2_Y
        NV12_L16_Y:
            ld1 {v0.16b}, [%0], #16
            prfm pldl1keep, [%0, #448]
            st1 {v0.16b}, [%2], #16
            subs x0, x0, #16
            bgt NV12_L16_Y

        NV12_L2_Y:
            cmp x1, #2
            blt NV12_UV
            sub x1, x1, #2
            ld1 {v0.h}[0], [%0], #2
            st1 {v0.h}[0], [%2], #2
            b NV12_L2_Y

        NV12_UV:
            add x6, %0, %1, lsr #2
            lsr %1, %1, #1
            and x1, %1, #15
            sub %1, %1, x1
            cbz %1, NV12_L2_UV

        NV12_L16_UV:
            ld1 {v0.8b}, [%0], #8
            ld1 {v1.8b}, [x6], #8
            prfm pldl1keep, [%0, #448]
            prfm pldl1keep, [x6, #448]
            st2 {v0.8b, v1.8b}, [%2], #16
            subs %1, %1, #16
            bgt NV12_L16_UV

        NV12_L2_UV:
            cmp x1, #2
            blt NV12_END
            sub x1, x1, #2
            ld1 {v0.b}[0], [%0], #1
            ld1 {v1.b}[0], [x6], #1
            st2 {v0.b, v1.b}[0], [%2], #2
            b NV12_L2_UV

        NV12_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6"
        , "v0", "v1", "v2");
}

void i420_to_nv21(const Image &src, const Image &dst)
{
    asm volatile(
        R"(
            mov x0, %1
            and x1, %1, #15
            sub x0, x0, x1
            cbz x0, NV21_L2_Y
        NV21_L16_Y:
            ld1 {v0.16b}, [%0], #16
            prfm pldl1keep, [%0, #448]
            st1 {v0.16b}, [%2], #16
            subs x0, x0, #16
            bgt NV21_L16_Y

        NV21_L2_Y:
            cmp x1, #2
            blt NV21_UV
            sub x1, x1, #2
            ld1 {v0.h}[0], [%0], #2
            st1 {v0.h}[0], [%2], #2
            b NV21_L2_Y

        NV21_UV:
            add x0, %0, %1, lsr #2
            lsr %1, %1, #1
            and x1, %1, #15
            sub %1, %1, x1
            cbz %1, NV21_L2_UV

        NV21_L16_UV:
            ld1 {v1.8b}, [%0], #8
            ld1 {v0.8b}, [x0], #8
            st2 {v0.8b, v1.8b}, [%2], #16
            prfm pldl1keep, [%0, #448]
            subs %1, %1, #16
            prfm pldl1keep, [x0, #448]
            bgt NV21_L16_UV

        NV21_L2_UV:
            cmp x1, #2
            blt NV21_END
            sub x1, x1, #2
            ld1 {v1.b}[0], [%0], #1
            ld1 {v0.b}[0], [x0], #1
            st2 {v0.b, v1.b}[0], [%2], #2
            b NV21_L2_UV

        NV21_END:
        )"
        :
        : "r"(src.data()), "r"(src.pixels()), "r"(dst.data())
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", "x5"
        , "v0", "v1");
}

NAMESPACE_END
