#include "types.hpp"
#include <algorithm>
#include <cvt_color.hpp>
#include <numeric>

NAMESPACE_BEGIN

constexpr int64_t k_pixel_size = 4L;
constexpr uint8_t k_shift      = 7U;
static const struct Rgb2YuvParam
{
    //  yr,  yg,  yb
    uint8_t m_y[3]{38, 75, 15};
    // -ur, -ug,  ub
    //  vr, -vg, -vb
    uint16_t m_uv[6]{22, 42, 64, 64, 54, 10};
} k_yuv_param;
constexpr uint8_t k_alpha           = 255;
static const uint8_t k_uv_tbl_idx[] = {
    0, 16, 1, 17, 2, 18, 3, 19,
    4, 20, 5, 21, 6, 22, 7, 23};

void bgra_to_gray(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto size = k_pixel_size * w * h;
    for (auto i = 0L; i < size; i += k_pixel_size) {
        dst[i >> 2] = (src[i] * k_yuv_param.m_y[2] + src[i + 1] * k_yuv_param.m_y[1] + src[i + 2] * k_yuv_param.m_y[0]) >> k_shift;
    }
}

void bgra_to_rgba(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto size = k_pixel_size * w * h;
    for (auto i = 0L, j = 0L; i < size; i += k_pixel_size, j += 4L) {
        dst[j + 0] = src[i + 2];
        dst[j + 1] = src[i + 1];
        dst[j + 2] = src[i + 0];
        dst[j + 3] = src[i + 3];
    }
}

void bgra_to_rgb(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto size = k_pixel_size * w * h;
    for (auto i = 0L, j = 0L; i < size; i += k_pixel_size, j += 3L) {
        dst[j + 0] = src[i + 2];
        dst[j + 1] = src[i + 1];
        dst[j + 2] = src[i + 0];
    }
}

void bgra_to_bgra(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto size = k_pixel_size * w * h;
    std::copy_n(src, size, dst);
}

void bgra_to_bgr(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto size = k_pixel_size * w * h;
    for (auto i = 0L, j = 0L; i < size; i += k_pixel_size, j += 3L) {
        dst[j + 0] = src[i + 0];
        dst[j + 1] = src[i + 1];
        dst[j + 2] = src[i + 2];
    }
}

void bgra_to_yuyv(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto size                      = k_pixel_size * w * h;
    constexpr auto k_byte_per_loop = k_pixel_size << 1;
    constexpr auto k_offset        = 1U << k_shift;
    for (auto i = 0L; i < size; i += k_byte_per_loop) {
        auto u0 = (src[i + 0] * k_yuv_param.m_uv[2] - src[i + 1] * k_yuv_param.m_uv[1] - src[i + 2] * k_yuv_param.m_uv[0]) >> k_shift;
        auto v0 = (src[i + 2] * k_yuv_param.m_uv[3] - src[i + 1] * k_yuv_param.m_uv[4] - src[i + 0] * k_yuv_param.m_uv[5]) >> k_shift;

        auto u1 = (src[i + k_pixel_size + 0] * k_yuv_param.m_uv[2] - src[i + k_pixel_size + 1] * k_yuv_param.m_uv[1] - src[i + k_pixel_size + 2] * k_yuv_param.m_uv[0]) >> k_shift;
        auto v1 = (src[i + k_pixel_size + 2] * k_yuv_param.m_uv[3] - src[i + k_pixel_size + 1] * k_yuv_param.m_uv[4] - src[i + k_pixel_size + 0] * k_yuv_param.m_uv[5]) >> k_shift;

        dst[(i >> 1) + 0] = (src[i] * k_yuv_param.m_y[2] + src[i + 1] * k_yuv_param.m_y[1] + src[i + 2] * k_yuv_param.m_y[0]) >> k_shift;
        dst[(i >> 1) + 1] = ((u0 + u1) >> 1) + k_offset;
        dst[(i >> 1) + 2] = (src[i + k_pixel_size] * k_yuv_param.m_y[2] + src[i + k_pixel_size + 1] * k_yuv_param.m_y[1] + src[i + k_pixel_size + 2] * k_yuv_param.m_y[0]) >> k_shift;
        dst[(i >> 1) + 3] = ((v0 + v1) >> 1) + k_offset;
    }
}

void bgra_to_uyvy(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto size                      = k_pixel_size * w * h;
    constexpr auto k_byte_per_loop = k_pixel_size << 1;
    constexpr auto k_offset        = 1U << k_shift;
    for (auto i = 0L; i < size; i += k_byte_per_loop) {
        auto u0 = (src[i + 0] * k_yuv_param.m_uv[2] - src[i + 1] * k_yuv_param.m_uv[1] - src[i + 2] * k_yuv_param.m_uv[0]) >> k_shift;
        auto v0 = (src[i + 2] * k_yuv_param.m_uv[3] - src[i + 1] * k_yuv_param.m_uv[4] - src[i + 0] * k_yuv_param.m_uv[5]) >> k_shift;

        auto u1 = (src[i + k_pixel_size + 0] * k_yuv_param.m_uv[2] - src[i + k_pixel_size + 1] * k_yuv_param.m_uv[1] - src[i + k_pixel_size + 2] * k_yuv_param.m_uv[0]) >> k_shift;
        auto v1 = (src[i + k_pixel_size + 2] * k_yuv_param.m_uv[3] - src[i + k_pixel_size + 1] * k_yuv_param.m_uv[4] - src[i + k_pixel_size + 0] * k_yuv_param.m_uv[5]) >> k_shift;

        dst[(i >> 1) + 0] = ((u0 + u1) >> 1) + k_offset;
        dst[(i >> 1) + 1] = (src[i] * k_yuv_param.m_y[2] + src[i + 1] * k_yuv_param.m_y[1] + src[i + 2] * k_yuv_param.m_y[0]) >> k_shift;
        dst[(i >> 1) + 2] = ((v0 + v1) >> 1) + k_offset;
        dst[(i >> 1) + 3] = (src[i + k_pixel_size] * k_yuv_param.m_y[2] + src[i + k_pixel_size + 1] * k_yuv_param.m_y[1] + src[i + k_pixel_size + 2] * k_yuv_param.m_y[0]) >> k_shift;
    }
}

void bgra_to_i420(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto size = k_pixel_size * w * h;
    auto ss   = k_pixel_size * w;
    auto ys   = w;

    auto src0 = src;
    auto src1 = src + ss;

    auto y0 = dst;
    auto y1 = dst + ys;

    auto u   = dst + (static_cast<int64_t>(w) * h);
    auto v   = u + ((static_cast<int64_t>(w) * h) >> 2);
    auto uvs = static_cast<int64_t>(w) >> 1;

    constexpr auto k_offset = 1U << k_shift;

    for (int32_t i = 0; i + 2 < h; i += 2) {
        for (int64_t j = 0; j + 2 < w; j += 2) {
            y0[j + 0] = (src0[(j * k_pixel_size) + 2] * k_yuv_param.m_y[0] + src0[(j * k_pixel_size) + 1] * k_yuv_param.m_y[1] + src0[(j * k_pixel_size) + 0] * k_yuv_param.m_y[2]) >> k_shift;
            y1[j + 0] = (src1[(j * k_pixel_size) + 2] * k_yuv_param.m_y[0] + src1[(j * k_pixel_size) + 1] * k_yuv_param.m_y[1] + src1[(j * k_pixel_size) + 0] * k_yuv_param.m_y[2]) >> k_shift;
            y0[j + 1] = (src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_y[0] + src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_y[1] + src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_y[2]) >> k_shift;
            y1[j + 1] = (src1[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_y[0] + src1[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_y[1] + src1[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_y[2]) >> k_shift;

            auto u00 = (src0[(j * k_pixel_size) + 0] * k_yuv_param.m_uv[2] - src0[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[1] - src0[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[0]) >> k_shift;
            auto u10 = (src1[(j * k_pixel_size) + 0] * k_yuv_param.m_uv[2] - src1[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[1] - src1[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[0]) >> k_shift;
            auto v00 = (src0[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[3] - src0[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[4] - src0[(j * k_pixel_size) + 0] * k_yuv_param.m_uv[5]) >> k_shift;
            auto v10 = (src1[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[3] - src1[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[4] - src1[(j * k_pixel_size) + 0] * k_yuv_param.m_uv[5]) >> k_shift;
            auto u01 = (src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[2] - src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[0] - src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[1]) >> k_shift;
            auto u11 = (src1[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[2] - src1[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[0] - src1[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[1]) >> k_shift;
            auto v01 = (src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[3] - src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[4] - src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[5]) >> k_shift;
            auto v11 = (src1[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[3] - src1[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[4] - src1[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[5]) >> k_shift;

            u[(j >> 1)] = ((u00 + u01 + u10 + u11) >> 2) + k_offset;
            v[(j >> 1)] = ((v00 + v01 + v10 + v11) >> 2) + k_offset;
        }
        src0 += (ss << 1);
        src1 += (ss << 1);
        y0 += (ys << 1);
        y1 += (ys << 1);
        u += uvs;
        v += uvs;
    }
    if (0 != (h & 0x1)) {
        int32_t tmp_u[4];
        int32_t tmp_v[4];
        for (int64_t j = 0; j + 2 < w; j += 2) {
            y0[j + 0] = (src0[(j * k_pixel_size) + 2] * k_yuv_param.m_y[0] + src0[(j * k_pixel_size) + 1] * k_yuv_param.m_y[1] + src0[(j * k_pixel_size) + 0] * k_yuv_param.m_y[2]) >> k_shift;
            y0[j + 1] = (src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_y[0] + src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_y[1] + src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_y[2]) >> k_shift;

            tmp_u[(j & 0x3) + 0] = (src0[(j * k_pixel_size) + 0] * k_yuv_param.m_uv[2] - src0[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[1] - src0[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[0]) >> k_shift;
            tmp_v[(j & 0x3) + 0] = (src0[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[3] - src0[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[4] - src0[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[5]) >> k_shift;
            tmp_u[(j & 0x3) + 1] = (src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[2] - src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[1] - src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[0]) >> k_shift;
            tmp_v[(j & 0x3) + 1] = (src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[3] - src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[4] - src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[5]) >> k_shift;
            if (0 != (j & 0x4)) {
                *u = (std::accumulate(tmp_u, tmp_u + 4, 0) >> 2) + k_offset;
                *v = (std::accumulate(tmp_v, tmp_v + 4, 0) >> 2) + k_offset;
                u += 1;
                v += 1;
            }
        }
    }
}

void bgra_to_nv12(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto size = k_pixel_size * w * h;
    auto ss   = k_pixel_size * w;
    auto ys   = w;

    auto src0 = src;
    auto src1 = src + ss;

    auto y0 = dst;
    auto y1 = dst + ys;

    auto uv  = dst + (static_cast<int64_t>(w) * h);
    auto uvs = static_cast<int64_t>(w);

    constexpr auto k_offset = 1U << k_shift;

    for (int32_t i = 0; i + 2 < h; i += 2) {
        for (int64_t j = 0; j + 2 < w; j += 2) {
            y0[j + 0] = (src0[(j * k_pixel_size) + 2] * k_yuv_param.m_y[0] + src0[(j * k_pixel_size) + 1] * k_yuv_param.m_y[1] + src0[(j * k_pixel_size) + 0] * k_yuv_param.m_y[2]) >> k_shift;
            y1[j + 0] = (src1[(j * k_pixel_size) + 2] * k_yuv_param.m_y[0] + src1[(j * k_pixel_size) + 1] * k_yuv_param.m_y[1] + src1[(j * k_pixel_size) + 0] * k_yuv_param.m_y[2]) >> k_shift;
            y0[j + 1] = (src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_y[0] + src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_y[1] + src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_y[2]) >> k_shift;
            y1[j + 1] = (src1[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_y[0] + src1[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_y[1] + src1[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_y[2]) >> k_shift;

            auto u00 = (src0[(j * k_pixel_size) + 0] * k_yuv_param.m_uv[2] - src0[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[1] - src0[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[0]) >> k_shift;
            auto u10 = (src1[(j * k_pixel_size) + 0] * k_yuv_param.m_uv[2] - src1[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[1] - src1[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[0]) >> k_shift;
            auto v00 = (src0[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[3] - src0[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[4] - src0[(j * k_pixel_size) + 0] * k_yuv_param.m_uv[5]) >> k_shift;
            auto v10 = (src1[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[3] - src1[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[4] - src1[(j * k_pixel_size) + 0] * k_yuv_param.m_uv[5]) >> k_shift;
            auto u01 = (src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[2] - src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[1] - src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[0]) >> k_shift;
            auto u11 = (src1[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[2] - src1[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[1] - src1[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[0]) >> k_shift;
            auto v01 = (src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[3] - src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[4] - src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[5]) >> k_shift;
            auto v11 = (src1[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[3] - src1[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[4] - src1[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[5]) >> k_shift;

            uv[j + 0] = ((u00 + u01 + u10 + u11) >> 2) + k_offset;
            uv[j + 1] = ((v00 + v01 + v10 + v11) >> 2) + k_offset;
        }
        src0 += (ss << 1);
        src1 += (ss << 1);
        y0 += (ys << 1);
        y1 += (ys << 1);
        uv += uvs;
    }
    if (0 != (h & 0x1)) {
        int32_t tmp_u[4];
        int32_t tmp_v[4];
        for (int64_t j = 0; j + 2 < w; j += 2) {
            y0[j + 0] = (src0[(j * k_pixel_size) + 2] * k_yuv_param.m_y[0] + src0[(j * k_pixel_size) + 1] * k_yuv_param.m_y[1] + src0[(j * k_pixel_size) + 0] * k_yuv_param.m_y[2]) >> k_shift;
            y0[j + 1] = (src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_y[0] + src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_y[1] + src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_y[2]) >> k_shift;

            tmp_u[(j & 0x3) + 0] = (src0[(j * k_pixel_size) + 0] * k_yuv_param.m_uv[2] - src0[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[1] - src0[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[0]) >> k_shift;
            tmp_v[(j & 0x3) + 0] = (src0[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[3] - src0[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[4] - src0[(j * k_pixel_size) + 0] * k_yuv_param.m_uv[5]) >> k_shift;
            tmp_u[(j & 0x3) + 1] = (src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[2] - src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[1] - src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[0]) >> k_shift;
            tmp_v[(j & 0x3) + 1] = (src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[3] - src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[4] - src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[5]) >> k_shift;
            if (0 != (j & 0x4)) {
                uv[0] = (std::accumulate(tmp_u, tmp_u + 4, 0) >> 2) + k_offset;
                uv[1] = (std::accumulate(tmp_v, tmp_v + 4, 0) >> 2) + k_offset;
                uv += 2;
            }
        }
    }
}

void bgra_to_nv21(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto size = k_pixel_size * w * h;
    auto ss   = k_pixel_size * w;
    auto ys   = w;

    auto src0 = src;
    auto src1 = src + ss;

    auto y0 = dst;
    auto y1 = dst + ys;

    auto uv  = dst + (static_cast<int64_t>(w) * h);
    auto uvs = static_cast<int64_t>(w);

    constexpr auto k_byte_per_loop = k_pixel_size << 1;
    constexpr auto k_offset        = 1U << k_shift;

    for (int32_t i = 0; i + 2 < h; i += 2) {
        for (int64_t j = 0; j + 2 < w; j += 2) {
            y0[j + 0] = (src0[(j * k_pixel_size) + 2] * k_yuv_param.m_y[0] + src0[(j * k_pixel_size) + 1] * k_yuv_param.m_y[1] + src0[(j * k_pixel_size) + 0] * k_yuv_param.m_y[2]) >> k_shift;
            y1[j + 0] = (src1[(j * k_pixel_size) + 2] * k_yuv_param.m_y[0] + src1[(j * k_pixel_size) + 1] * k_yuv_param.m_y[1] + src1[(j * k_pixel_size) + 0] * k_yuv_param.m_y[2]) >> k_shift;
            y0[j + 1] = (src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_y[0] + src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_y[1] + src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_y[2]) >> k_shift;
            y1[j + 1] = (src1[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_y[0] + src1[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_y[1] + src1[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_y[2]) >> k_shift;

            auto u00 = (src0[(j * k_pixel_size) + 0] * k_yuv_param.m_uv[2] - src0[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[1] - src0[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[0]) >> k_shift;
            auto u10 = (src1[(j * k_pixel_size) + 0] * k_yuv_param.m_uv[2] - src1[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[1] - src1[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[0]) >> k_shift;
            auto v00 = (src0[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[3] - src0[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[4] - src0[(j * k_pixel_size) + 0] * k_yuv_param.m_uv[5]) >> k_shift;
            auto v10 = (src1[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[3] - src1[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[4] - src1[(j * k_pixel_size) + 0] * k_yuv_param.m_uv[5]) >> k_shift;
            auto u01 = (src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[2] - src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[1] - src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[0]) >> k_shift;
            auto u11 = (src1[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[2] - src1[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[1] - src1[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[0]) >> k_shift;
            auto v01 = (src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[3] - src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[4] - src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[5]) >> k_shift;
            auto v11 = (src1[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[3] - src1[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[4] - src1[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[5]) >> k_shift;

            uv[j + 1] = ((u00 + u01 + u10 + u11) >> 2) + k_offset;
            uv[j + 0] = ((v00 + v01 + v10 + v11) >> 2) + k_offset;
        }
        src0 += (ss << 1);
        src1 += (ss << 1);
        y0 += (ys << 1);
        y1 += (ys << 1);
        uv += uvs;
    }
    if (0 != (h & 0x1)) {
        int32_t tmp_u[4];
        int32_t tmp_v[4];
        for (int64_t j = 0; j + 2 < w; j += 2) {
            y0[j + 0] = (src0[(j * k_pixel_size) + 2] * k_yuv_param.m_y[0] + src0[(j * k_pixel_size) + 1] * k_yuv_param.m_y[1] + src0[(j * k_pixel_size) + 0] * k_yuv_param.m_y[2]) >> k_shift;
            y0[j + 1] = (src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_y[0] + src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_y[1] + src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_y[2]) >> k_shift;

            tmp_u[(j & 0x3) + 0] = (src0[(j * k_pixel_size) + 0] * k_yuv_param.m_uv[2] - src0[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[1] - src0[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[0]) >> k_shift;
            tmp_v[(j & 0x3) + 0] = (src0[(j * k_pixel_size) + 2] * k_yuv_param.m_uv[3] - src0[(j * k_pixel_size) + 1] * k_yuv_param.m_uv[4] - src0[(j * k_pixel_size) + 0] * k_yuv_param.m_uv[5]) >> k_shift;
            tmp_u[(j & 0x3) + 1] = (src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[2] - src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[1] - src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[0]) >> k_shift;
            tmp_v[(j & 0x3) + 1] = (src0[(j * k_pixel_size) + k_pixel_size + 2] * k_yuv_param.m_uv[3] - src0[(j * k_pixel_size) + k_pixel_size + 1] * k_yuv_param.m_uv[4] - src0[(j * k_pixel_size) + k_pixel_size + 0] * k_yuv_param.m_uv[5]) >> k_shift;
            if (0 != (j & 0x4)) {
                uv[0] = (std::accumulate(tmp_u, tmp_u + 4, 0) >> 2) + k_offset;
                uv[1] = (std::accumulate(tmp_v, tmp_v + 4, 0) >> 2) + k_offset;
                uv += 2;
            }
        }
    }
}

void bgra_to_gray_asm(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto size = 1L * w * h;
    asm volatile(
        LOAD_Y_PARAM
        R"(
        GRAY_L32:
            cmp        %1, #32
            blt        GRAY_L16
            sub        %1, %1, #32
            ld4        {v0.16b, v1.16b, v2.16b, v3.16b}, [%0], #64
            ld4        {v4.16b, v5.16b, v6.16b, v7.16b}, [%0], #64

            umull      v3.8h, v0.8b, v31.8b    // * r
            umlal      v3.8h, v1.8b, v30.8b    // * g
            umlal      v3.8h, v2.8b, v29.8b    // * b
            prfm    pldl1keep, [%0, 448]
            umull2     v7.8h, v0.16b, v31.16b  // * r
            umlal2     v7.8h, v1.16b, v30.16b  // * g
            umlal2     v7.8h, v2.16b, v29.16b  // * b
            uqshrn     v0.8b, v3.8h, %[shift]
            uqshrn2    v0.16b, v7.8h, %[shift]
            umull      v2.8h, v4.8b, v31.8b    // * r
            umlal      v2.8h, v5.8b, v30.8b    // * g
            umlal      v2.8h, v6.8b, v29.8b    // * b
            umull2     v8.8h, v4.16b, v31.16b  // * r
            umlal2     v8.8h, v5.16b, v30.16b  // * g
            umlal2     v8.8h, v6.16b, v29.16b  // * b
            uqshrn     v1.8b, v2.8h, %[shift]
            uqshrn2    v1.16b, v8.8h, %[shift]
            st1        {v0.16b, v1.16b}, [%2], #32
            b GRAY_L32

        GRAY_L16:
            cmp       %1, #16
            blt       GRAY_L8
            sub       %1, %1, #16
            ld4       {v0.16b, v1.16b, v2.16b, v3.16b}, [%0], #64
            umull     v3.8h, v0.8b, v31.8b    // * r
            umlal     v3.8h, v1.8b, v30.8b    // * g
            umlal     v3.8h, v2.8b, v29.8b    // * b
            umull2    v4.8h, v0.16b, v31.16b  // * r
            umlal2    v4.8h, v1.16b, v30.16b  // * g
            umlal2    v4.8h, v2.16b, v29.16b  // * b
            uqshrn    v0.8b, v3.8h, %[shift]
            uqshrn2   v0.16b, v4.8h, %[shift]
            st1       {v0.16b}, [%2], #16

        GRAY_L8:
            cmp       %1, #8
            blt       GRAY_L1
            sub       %1, %1, #8
            ld4       {v0.8b, v1.8b, v2.8b, v3.8b}, [%0], #32
            umull     v3.8h, v0.8b, v31.8b    // * r
            umlal     v3.8h, v1.8b, v30.8b    // * g
            umlal     v3.8h, v2.8b, v29.8b    // * b
            uqshrn    v0.8b, v3.8h, %[shift]
            st1       {v0.8b}, [%2], #8

        GRAY_L1:
            cmp       %1, #1
            blt       GRAY_END
            sub       %1, %1, #1
            ld4       {v0.b, v1.b, v2.b, v3.b}[0], [%0], #4
            umull     v3.8h, v0.8b, v31.8b    // * r
            umlal     v3.8h, v1.8b, v30.8b    // * g
            umlal     v3.8h, v2.8b, v29.8b    // * b
            uqshrn    v0.8b, v3.8h, %[shift]
            st1       {v0.b}[1], [%2], #1
            b GRAY_L1

        GRAY_END:
        )"
        : "+r"(src), "+r"(size), "+r"(dst)
        : [y_param] "r"(&k_yuv_param), [shift] "i"(k_shift)
        : "memory", "cc", "x0", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v29", "v30", "v31");
}

void bgra_to_rgba_asm(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    int64_t size = 1L * w * h;
    asm volatile(
        R"(
        RGBA_L32:
            cmp %1, #32
            blt RGBA_L16
            ld4 {v0.16b, v1.16b, v2.16b, v3.16b}, [%0], #64
            mov v8.16b, v0.16b
            mov v0.16b, v2.16b
            mov v2.16b, v8.16b
            ld4 {v4.16b, v5.16b, v6.16b, v7.16b}, [%0], #64
            mov v8.16b, v4.16b
            mov v4.16b, v6.16b
            mov v6.16b, v8.16b
            st4 {v0.16b, v1.16b, v2.16b, v3.16b}, [%2], #64
            st4 {v4.16b, v5.16b, v6.16b, v7.16b}, [%2], #64
            prfm pldl1keep, [%0, 448]
            sub %1, %1, #32
            b RGBA_L32
        RGBA_L16:
            cmp %1, #16
            blt RGBA_L8
            ld4 {v0.16b, v1.16b, v2.16b, v3.16b}, [%0], #64
            mov v8.16b, v0.16b
            mov v0.16b, v2.16b
            mov v2.16b, v8.16b
            st4 {v0.16b, v1.16b, v2.16b, v3.16b}, [%2], #64
            sub %1, %1, #16
        RGBA_L8:
            cmp %1, #8
            blt RGBA_L1
            ld4 {v0.8b, v1.8b, v2.8b, v3.8b}, [%0], #32
            mov v8.8b, v0.8b
            mov v0.8b, v2.8b
            mov v2.8b, v8.8b
            st4 {v0.8b, v1.8b, v2.8b, v3.8b}, [%2], #32
            sub %1, %1, #8
        RGBA_L1:
            cmp %1, #1
            blt RGBA_END
            ld4 {v0.b, v1.b, v2.b, v3.b}[0], [%0], #4
            mov v8.8b, v0.8b
            mov v0.8b, v2.8b
            mov v2.8b, v8.8b
            st4 {v0.b, v1.b, v2.b, v3.b}[0], [%2], #4
            sub %1, %1, #1
        RGBA_END:
        )"
        : "+r"(src), "+r"(size), "+r"(dst)
        :
        : "cc", "memory");
}

void bgra_to_rgb_asm(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto size = 1L * w * h;

    asm volatile(
        R"(
        RGB_L32:
            cmp       %1, #32
            blt       RGB_L16
            sub       %1, %1, #32
            ld4       {v0.16b, v1.16b, v2.16b, v3.16b}, [%0], #64
            mov v8.16b, v0.16b
            mov v0.16b, v2.16b
            mov v2.16b, v8.16b
            ld4       {v4.16b, v5.16b, v6.16b, v7.16b}, [%0], #64
            mov v8.16b, v4.16b
            mov v4.16b, v6.16b
            mov v6.16b, v8.16b
            st3       {v0.16b, v1.16b, v2.16b}, [%2], #48
            st3       {v4.16b, v5.16b, v6.16b}, [%2], #48
            b RGB_L32

        RGB_L16:
            cmp       %1, #16
            blt       RGB_L8
            sub       %1, %1, #16
            ld4       {v0.16b, v1.16b, v2.16b, v3.16b}, [%0], #64
            mov v8.16b, v0.16b
            mov v0.16b, v2.16b
            mov v2.16b, v8.16b
            st3       {v0.16b, v1.16b, v2.16b}, [%2], #48

        RGB_L8:
            cmp       %1, #8
            blt       RGB_L1
            sub       %1, %1, #8
            ld4       {v0.8b, v1.8b, v2.8b, v3.8b}, [%0], #32
            mov v8.8b, v0.8b
            mov v0.8b, v2.8b
            mov v2.8b, v8.8b
            st3       {v0.8b, v1.8b, v2.8b}, [%2], #24

        RGB_L1:
            cmp       %1, #1
            blt       RGB_END
            sub       %1, %1, #1
            ld4       {v0.b, v1.b, v2.b, v3.b}[0], [%0], #4
            mov v8.8b, v0.8b
            mov v0.8b, v2.8b
            mov v2.8b, v8.8b
            st3       {v0.b, v1.b, v2.b}[0], [%2], #3
            b RGB_L1

        RGB_END:
        )"
        : "+r"(src), "+r"(size), "+r"(dst)
        :
        : "memory", "cc", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7");
}

void bgra_to_bgr_asm(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto size = 1L * w * h;
    asm volatile(
        R"(
        BGR_L32:
            cmp %1, #32
            blt BGR_L16
            sub %1, %1, #32
            ld4 {v0.16b, v1.16b, v2.16b, v3.16b}, [%0], #64
            ld4 {v4.16b, v5.16b, v6.16b, v7.16b}, [%0], #64
            prfm      pldl1keep, [%0, 448]
            st3 {v0.16b, v1.16b, v2.16b}, [%2], #48
            st3 {v4.16b, v5.16b, v6.16b}, [%2], #48
            b BGR_L32

        BGR_L16:
            cmp %1, #16
            blt BGR_L8
            sub %1, %1, #16
            ld4 {v0.16b, v1.16b, v2.16b, v3.16b}, [%0], #64
            st3 {v0.16b, v1.16b, v2.16b}, [%2], #48

        BGR_L8:
            cmp %1, #8
            blt BGR_L1
            sub %1, %1, #8
            ld4 {v0.8b, v1.8b, v2.8b, v3.8b}, [%0], #32
            st3 {v0.8b, v1.8b, v2.8b}, [%2], #24

        BGR_L1:
            cmp       %1, #1
            blt       BGR_END
            sub       %1, %1, #1
            ld4       {v0.b, v1.b, v2.b, v3.b}[0], [%0], #4
            st3       {v0.b, v1.b, v2.b}[0], [%2], #3
            b RGB_L1

        BGR_END:
        )"
        : "+r"(src), "+r"(size), "+r"(dst)
        :
        : "memory", "cc", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7");
}

void bgra_to_bgra_asm(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto size = 1L * w * h;
    asm volatile(
        R"(
        BGRA_L32:
            cmp %1, #32
            blt BGRA_L16
            sub %1, %1, #32
            ld1 {v0.16b, v1.16b, v2.16b, v3.16b}, [%0], #64
            ld1 {v4.16b, v5.16b, v6.16b, v7.16b}, [%0], #64
            prfm      pldl1keep, [%0, 448]
            st1 {v0.16b, v1.16b, v2.16b, v3.16b}, [%2], #64
            st1 {v4.16b, v5.16b, v6.16b, v7.16b}, [%2], #64
            b BGRA_L32

        BGRA_L16:
            cmp %1, #16
            blt BGRA_L8
            sub %1, %1, #16
            ld1 {v0.16b, v1.16b, v2.16b, v3.16b}, [%0], #64
            st1 {v0.16b, v1.16b, v2.16b, v3.16b}, [%2], #64

        BGRA_L8:
            cmp %1, #8
            blt BGRA_L1
            sub %1, %1, #8
            ld1 {v0.16b, v1.16b}, [%0], #32
            st1 {v0.16b, v1.16b}, [%2], #32

        BGRA_L4:
            cmp %1, #4
            blt BGRA_L2
            sub %1, %1, #4
            ld1 {v0.16b}, [%0], #16
            st1 {v0.16b}, [%2], #16

        BGRA_L2:
            cmp %1, #4
            blt BGRA_L1
            sub %1, %1, #2
            ld1 {v0.8b}, [%0], #8
            st1 {v0.8b}, [%2], #8

        BGRA_L1:
            cmp %1, #1
            blt BGRA_END
            sub %1, %1, #1
            ld1 {v0.s}[0], [%0], #4
            st1 {v0.s}[0], [%2], #4

        BGRA_END:
        )"
        : "+r"(src), "+r"(size), "+r"(dst)
        :
        : "memory", "cc", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9");
}

void bgra_to_yuyv_asm(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto size = 1L * w * h;
    asm volatile(
        LOAD_YUV_PARAM
        R"(
            ld1 {v23.16b}, [%[idx]]

            movi v22.16b, #128
            uaddlp v22.8h, v22.16b
            shl v22.8h, v22.8h, %[shift]

        YUYV_L16:
            cmp %1, #16
            blt YUYV_L8
            sub %1, %1, #16
            ld4 {v0.16b, v1.16b, v2.16b, v3.16b}, [%0], #64

            umull v3.8h, v0.8b, v31.8b
            umlal v3.8h, v1.8b, v30.8b
            umlal v3.8h, v2.8b, v29.8b
            prfm pldl1keep, [%0, 448]
            umull2 v4.8h, v0.16b, v31.16b
            umlal2 v4.8h, v1.16b, v30.16b
            umlal2 v4.8h, v2.16b, v29.16b

            uqshrn v3.8b, v3.8h, %[shift]
            uqshrn2 v3.16b, v4.8h, %[shift]

            // uv
            uaddlp v0.8h, v0.16b
            uaddlp v1.8h, v1.16b
            uaddlp v2.8h, v2.16b

            mul v4.8h, v0.8h, v26.8h
            mls v4.8h, v1.8h, v25.8h
            mls v4.8h, v2.8h, v24.8h

            mul v5.8h, v2.8h, v26.8h
            mls v5.8h, v1.8h, v27.8h
            mls v5.8h, v0.8h, v28.8h

            addhn v0.8b, v4.8h, v22.8h
            addhn v1.8b, v5.8h, v22.8h

            tbl v4.16b, {v0.16b, v1.16b}, v23.16b

            st2 {v3.16b, v4.16b}, [%2], #32
            b YUYV_L16

        YUYV_L8:
            cmp %1, #8
            blt YUYV_L2
            sub %1, %1, #8
            ld4 {v0.8b, v1.8b, v2.8b, v3.8b}, [%0], #32
            umull v3.8h, v0.8b, v31.8b
            umlal v3.8h, v1.8b, v30.8b
            umlal v3.8h, v2.8b, v29.8b
            uqshrn v3.8b, v3.8h, %[shift]

            uaddlp v0.4h, v0.8b
            uaddlp v1.4h, v1.8b
            uaddlp v2.4h, v2.8b

            mul v4.8h, v0.8h, v26.8h
            mls v4.8h, v1.8h, v25.8h
            mls v4.8h, v2.8h, v24.8h

            mul v5.8h, v2.8h, v26.8h
            mls v5.8h, v1.8h, v27.8h
            mls v5.8h, v0.8h, v28.8h

            addhn v0.8b, v5.8h, v22.8h
            addhn v1.8b, v6.8h, v22.8h

            tbl v4.8b, {v0.16b, v1.16b}, v23.8b

            st2 {v3.8b, v4.8b}, [%2], #16

        YUYV_L2:
            cmp %1, #2
            blt YUYV_END
            sub %1, %1, #2
            ld4 {v0.b, v1.b, v2.b, v3.b}[0], [%0], #4
            ld4 {v0.b, v1.b, v2.b, v3.b}[1], [%0], #4

            umull v3.8h, v0.8b, v31.8b
            umlal v3.8h, v1.8b, v30.8b
            umlal v3.8h, v2.8b, v29.8b
            umull2 v4.8h, v0.16b, v31.16b
            umlal2 v4.8h, v1.16b, v30.16b
            umlal2 v4.8h, v2.16b, v29.16b

            uqshrn v3.8b, v3.8h, %[shift]
            uqshrn2 v3.16b, v4.8h, %[shift]

            // uv
            uaddlp v0.8h, v0.16b
            uaddlp v1.8h, v1.16b
            uaddlp v2.8h, v2.16b

            mul v4.8h, v0.8h, v26.8h
            mls v4.8h, v1.8h, v25.8h
            mls v4.8h, v2.8h, v24.8h

            mul v5.8h, v2.8h, v26.8h
            mls v5.8h, v1.8h, v27.8h
            mls v5.8h, v0.8h, v28.8h

            addhn v0.8b, v4.8h, v22.8h
            addhn v1.8b, v5.8h, v22.8h

            tbl v4.16b, {v0.16b, v1.16b}, v23.16b

            st2 {v3.b, v4.b}[0], [%2], #2   // YU
            st2 {v3.b, v4.b}[1], [%2], #2   // YV
            b YUYV_L2

        YUYV_END:
        )"
        :
        : "r"(src), "r"(size), "r"(dst), [y_param] "r"(k_yuv_param.m_y), [uv_param] "r"(k_yuv_param.m_uv), [idx] "r"(k_uv_tbl_idx), [shift] "i"(k_shift)
        : "cc", "memory", "x0");
}

void bgra_to_uyvy_asm(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto size = 1L * w * h;
    asm volatile(
        LOAD_YUV_PARAM
        R"(
            ld1 {v23.16b}, [%[idx]]

            movi v22.16b, #128
            uaddlp v22.8h, v22.16b
            shl v22.8h, v22.8h, %[shift]

        UYVY_L16:
            cmp %1, #16
            blt UYVY_L8
            sub %1, %1, #16
            ld4 {v0.16b, v1.16b, v2.16b, v3.16b}, [%0], #64

            umull v4.8h, v0.8b, v31.8b
            umlal v4.8h, v1.8b, v30.8b
            umlal v4.8h, v2.8b, v29.8b
            prfm pldl1keep, [%0, 448]
            umull2 v3.8h, v0.16b, v31.16b
            umlal2 v3.8h, v1.16b, v30.16b
            umlal2 v3.8h, v2.16b, v29.16b

            uqshrn v4.8b, v4.8h, %[shift]
            uqshrn2 v4.16b, v3.8h, %[shift]

            // uv
            uaddlp v0.8h, v0.16b
            uaddlp v1.8h, v1.16b
            uaddlp v2.8h, v2.16b

            mul v5.8h, v0.8h, v26.8h
            mls v5.8h, v1.8h, v25.8h
            mls v5.8h, v2.8h, v24.8h

            mul v3.8h, v2.8h, v26.8h
            mls v3.8h, v1.8h, v27.8h
            mls v3.8h, v0.8h, v28.8h

            addhn v0.8b, v5.8h, v22.8h
            addhn v1.8b, v3.8h, v22.8h

            tbl v3.16b, {v0.16b, v1.16b}, v23.16b

            st2 {v3.16b, v4.16b}, [%2], #32
            b UYVY_L16

        UYVY_L8:
            cmp %1, #8
            blt UYVY_L2
            sub %1, %1, #32
            ld4 {v0.8b, v1.8b, v2.8b, v3.8b}, [%0], #32
            umull v4.8h, v0.8b, v31.8b
            umlal v4.8h, v1.8b, v30.8b
            umlal v4.8h, v2.8b, v29.8b
            uqshrn v4.8b, v3.8h, %[shift]

            uaddlp v0.4h, v0.8b
            uaddlp v1.4h, v1.8b
            uaddlp v2.4h, v2.8b

            mul v5.8h, v0.8h, v26.8h
            mls v5.8h, v1.8h, v25.8h
            mls v5.8h, v2.8h, v24.8h

            mul v3.8h, v2.8h, v26.8h
            mls v3.8h, v1.8h, v27.8h
            mls v3.8h, v0.8h, v28.8h

            addhn v0.8b, v5.8h, v22.8h
            addhn v1.8b, v3.8h, v22.8h

            tbl v3.8b, {v0.16b, v1.16b}, v23.8b
            st2 {v3.8b, v4.8b}, [%2], #16

        UYVY_L2:
            cmp %1, #2
            blt UYVY_END
            sub %1, %1, #2
            ld4 {v0.b, v1.b, v2.b, v3.b}[0], [%0], #4
            ld4 {v0.b, v1.b, v2.b, v3.b}[1], [%0], #4

            umull v4.8h, v0.8b, v31.8b
            umlal v4.8h, v1.8b, v30.8b
            umlal v4.8h, v2.8b, v29.8b
            umull2 v3.8h, v0.16b, v31.16b
            umlal2 v3.8h, v1.16b, v30.16b
            umlal2 v3.8h, v2.16b, v29.16b

            uqshrn v4.8b, v4.8h, %[shift]
            uqshrn2 v4.16b, v3.8h, %[shift]

            // uv
            uaddlp v0.8h, v0.16b
            uaddlp v1.8h, v1.16b
            uaddlp v2.8h, v2.16b

            mul v5.8h, v0.8h, v26.8h
            mls v5.8h, v1.8h, v25.8h
            mls v5.8h, v2.8h, v24.8h

            mul v3.8h, v2.8h, v26.8h
            mls v3.8h, v1.8h, v27.8h
            mls v3.8h, v0.8h, v28.8h

            addhn v0.8b, v5.8h, v22.8h
            addhn v1.8b, v3.8h, v22.8h

            tbl v3.16b, {v0.16b, v1.16b}, v23.16b

            st2 {v3.b, v4.b}[0], [%2], #2
            st2 {v3.b, v4.b}[1], [%2], #2
            b UYVY_L2

        UYVY_END:
        )"
        :
        : "r"(src), "r"(size), "r"(dst), [y_param] "r"(k_yuv_param.m_y), [uv_param] "r"(k_yuv_param.m_uv), [idx] "r"(k_uv_tbl_idx), [shift] "i"(k_shift)
        : "cc", "memory", "x0");
}

void bgra_to_i420_asm(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto y = dst;
    auto u = dst + static_cast<ptrdiff_t>(w * h);
    auto v = u + static_cast<ptrdiff_t>((w * h) >> 2);
    asm volatile(
        LOAD_YUV_PARAM
        R"(
            ld1 {v23.16b}, [%[idx]]
            movi v22.16b, #128
            uaddlp v22.8h, v22.16b
            shl v22.8h, v22.8h, %[shift]
            mov x0, %0
            mov x1, %1
            mov w2, %w[h]
            mov w4, %w[w]
            lsl x4, x4, #2
        I420_2H:
            mov %0, x0
            mov %1, x1
            cmp x2, #2
            mov w3, %w[w]
            blt I420_END
            sub x2, x2, #2
            add x0, x0, x4
            add x1, x1, x3
        I420_2H_L16:
            cmp x3, #16
            blt I420_2H_L8
            sub x3, x3, #16
            ld4 {v0.16b, v1.16b, v2.16b, v3.16b}, [%0], #64
            ld4 {v4.16b, v5.16b, v6.16b, v7.16b}, [x0], #64

            umull v3.8h, v0.8b, v31.8b
            umlal v3.8h, v1.8b, v30.8b
            umlal v3.8h, v2.8b, v29.8b
            prfm pldl1keep, [%0, 448]
            umull v7.8h, v4.8b, v31.8b
            umlal v7.8h, v5.8b, v30.8b
            umlal v7.8h, v6.8b, v29.8b
            prfm pldl1keep, [x0, 448]
            umull2 v8.8h, v0.16b, v31.16b
            umlal2 v8.8h, v1.16b, v30.16b
            umlal2 v8.8h, v2.16b, v29.16b
            umull2 v9.8h, v4.16b, v31.16b
            umlal2 v9.8h, v5.16b, v30.16b
            umlal2 v9.8h, v6.16b, v29.16b

            uqshrn v3.8b, v3.8h, %[shift]
            uqshrn2 v3.16b, v8.8h, %[shift]
            st1 {v3.16b}, [%1], #16
            uqshrn v7.8b, v7.8h, %[shift]
            uqshrn2 v7.16b, v9.8h, %[shift]
            st1 {v7.16b}, [x1], #16

            uaddlp v0.8h, v0.16b
            uaddlp v1.8h, v1.16b
            uaddlp v2.8h, v2.16b
            uadalp v0.8h, v4.16b
            uadalp v1.8h, v5.16b
            uadalp v2.8h, v6.16b

            ushr v0.8h, v0.8h, #1
            ushr v1.8h, v1.8h, #1
            ushr v2.8h, v2.8h, #1

            mul v3.8h, v0.8h, v26.8h
            mls v3.8h, v1.8h, v25.8h
            mls v3.8h, v2.8h, v24.8h
            addhn v3.8b, v3.8h, v22.8h
            st1 {v3.8b}, [%2], #8

            mul v4.8h, v0.8h, v26.8h
            mls v4.8h, v1.8h, v27.8h
            mls v4.8h, v2.8h, v28.8h
            addhn v4.8b, v4.8h, v22.8h
            st1 {v4.8b}, [%3], #8

            b I420_2H_L16

        I420_2H_L8:
            cmp x3, #8
            blt I420_2H_L2
            sub x3, x3, #8
            ld4 {v0.8b, v1.8b, v2.8b, v3.8b}, [%0], #32
            ld4 {v4.8b, v5.8b, v6.8b, v7.8b}, [x0], #32

            umull v3.8h, v0.8b, v31.8b
            umlal v3.8h, v1.8b, v30.8b
            umlal v3.8h, v2.8b, v29.8b
            umull v7.8h, v4.8b, v31.8b
            umlal v7.8h, v5.8b, v30.8b
            umlal v7.8h, v6.8b, v29.8b

            uqshrn v3.8b, v3.8h, %[shift]
            st1 {v3.8b}, [%1], #8
            uqshrn v7.8b, v7.8h, %[shift]
            st1 {v7.8b}, [x1], #8

            uaddlp v0.4h, v0.8b
            uaddlp v1.4h, v1.8b
            uaddlp v2.4h, v2.8b
            uadalp v0.4h, v4.8b
            uadalp v1.4h, v5.8b
            uadalp v2.4h, v6.8b

            ushr v0.8h, v0.8h, #1
            ushr v1.8h, v1.8h, #1
            ushr v2.8h, v2.8h, #1

            mul v3.8h, v0.8h, v26.8h
            mls v3.8h, v1.8h, v25.8h
            mls v3.8h, v2.8h, v24.8h
            addhn v3.8b, v3.8h, v22.8h
            st1 {v3.s}[0], [%2], #4

            mul v4.8h, v0.8h, v26.8h
            mls v4.8h, v1.8h, v27.8h
            mls v4.8h, v2.8h, v28.8h
            addhn v4.8b, v4.8h, v22.8h
            st1 {v4.s}[0], [%3], #4

        I420_2H_L2:
            cmp x3, #2
            blt I420_2H
            sub x3, x3, #2
            ld4 {v0.b, v1.b, v2.b, v3.b}[0], [%0], #4
            ld4 {v0.b, v1.b, v2.b, v3.b}[1], [%0], #4
            ld4 {v4.b, v5.b, v6.b, v7.b}[0], [x0], #4
            ld4 {v4.b, v5.b, v6.b, v7.b}[1], [x0], #4

            umull v3.8h, v0.8b, v31.8b
            umlal v3.8h, v1.8b, v30.8b
            umlal v3.8h, v2.8b, v29.8b
            umull v7.8h, v4.8b, v31.8b
            umlal v7.8h, v5.8b, v30.8b
            umlal v7.8h, v6.8b, v29.8b

            uqshrn v3.8b, v3.8h, %[shift]
            st1 {v3.h}[0], [%1], #2
            uqshrn v7.8b, v7.8h, %[shift]
            st1 {v7.h}[0], [x1], #2

            uaddlp v0.4h, v0.8b
            uaddlp v1.4h, v1.8b
            uaddlp v2.4h, v2.8b
            uadalp v0.4h, v4.8b
            uadalp v1.4h, v5.8b
            uadalp v2.4h, v6.8b

            ushr v0.8h, v0.8h, #1
            ushr v1.8h, v1.8h, #1
            ushr v2.8h, v2.8h, #1

            mul v3.8h, v0.8h, v26.8h
            mls v3.8h, v1.8h, v25.8h
            mls v3.8h, v2.8h, v24.8h
            addhn v3.8b, v3.8h, v22.8h
            st1 {v3.b}[0], [%2], #1

            mul v4.8h, v0.8h, v26.8h
            mls v4.8h, v1.8h, v27.8h
            mls v4.8h, v2.8h, v28.8h
            addhn v4.8b, v4.8h, v22.8h
            st1 {v4.b}[0], [%3], #1
            b I420_2H_L2

        I420_END:
        )"
        :
        : "r"(src), "r"(y), "r"(u), "r"(v), [h] "r"(h), [w] "r"(w), [y_param] "r"(k_yuv_param.m_y), [uv_param] "r"(k_yuv_param.m_uv), [idx] "r"(k_uv_tbl_idx), [shift] "i"(k_shift)
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", USED_YUV_REG, "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9");
}

void bgra_to_nv12_asm(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto y  = dst;
    auto uv = dst + static_cast<ptrdiff_t>(w * h);
    asm volatile(
        LOAD_YUV_PARAM
        R"(
            ld1 {v23.16b}, [%[idx]]

            movi v22.16b, #128
            uaddlp v22.8h, v22.16b
            shl v22.8h, v22.8h, %[shift]
            mov x0, %0
            mov x1, %1
            mov w2, %w[h]
            mov w4, %w[w]
            lsl x4, x4, #2
        NV12_2H:
            mov %0, x0
            mov %1, x1
            cmp x2, #2
            mov w3, %w[w]
            blt NV12_END
            sub x2, x2, #2
            add x0, x0, x4
            add x1, x1, x3

        NV12_2H_L16:
            cmp x3, #16
            blt NV12_2H_L8
            sub x3, x3, #16
            ld4 {v0.16b, v1.16b, v2.16b, v3.16b}, [%0], #64
            ld4 {v4.16b, v5.16b, v6.16b, v7.16b}, [x0], #64

            umull v3.8h, v0.8b, v31.8b
            umlal v3.8h, v1.8b, v30.8b
            umlal v3.8h, v2.8b, v29.8b
            prfm pldl1keep, [%0, 448]
            umull v7.8h, v4.8b, v31.8b
            umlal v7.8h, v5.8b, v30.8b
            umlal v7.8h, v6.8b, v29.8b
            prfm pldl1keep, [x0, 448]
            umull2 v8.8h, v0.16b, v31.16b
            umlal2 v8.8h, v1.16b, v30.16b
            umlal2 v8.8h, v2.16b, v29.16b
            umull2 v9.8h, v4.16b, v31.16b
            umlal2 v9.8h, v5.16b, v30.16b
            umlal2 v9.8h, v6.16b, v29.16b

            uqshrn v3.8b, v3.8h, %[shift]
            uqshrn2 v3.16b, v8.8h, %[shift]
            st1 {v3.16b}, [%1], #16
            uqshrn v7.8b, v7.8h, %[shift]
            uqshrn2 v7.16b, v9.8h, %[shift]
            st1 {v7.16b}, [x1], #16

            uaddlp v0.8h, v0.16b
            uaddlp v1.8h, v1.16b
            uaddlp v2.8h, v2.16b
            uadalp v0.8h, v4.16b
            uadalp v1.8h, v5.16b
            uadalp v2.8h, v6.16b

            ushr v0.8h, v0.8h, #1
            ushr v1.8h, v1.8h, #1
            ushr v2.8h, v2.8h, #1

            mul v3.8h, v0.8h, v26.8h
            mls v3.8h, v1.8h, v25.8h
            mls v3.8h, v2.8h, v24.8h
            addhn v3.8b, v3.8h, v22.8h

            mul v4.8h, v2.8h, v26.8h
            mls v4.8h, v1.8h, v27.8h
            mls v4.8h, v0.8h, v28.8h
            addhn v4.8b, v4.8h, v22.8h

            st2 {v3.8b, v4.8b}, [%2], #16

            b NV12_2H_L16

        NV12_2H_L8:
            cmp x3, #8
            blt NV12_2H_L2
            sub x3, x3, #8
            ld4 {v0.8b, v1.8b, v2.8b, v3.8b}, [%0], #32
            ld4 {v4.8b, v5.8b, v6.8b, v7.8b}, [x0], #32

            umull v3.8h, v0.8b, v31.8b
            umlal v3.8h, v1.8b, v30.8b
            umlal v3.8h, v2.8b, v29.8b
            umull v7.8h, v4.8b, v31.8b
            umlal v7.8h, v5.8b, v30.8b
            umlal v7.8h, v6.8b, v29.8b

            uqshrn v3.8b, v3.8h, %[shift]
            st1 {v3.8b}, [%1], #8
            uqshrn v7.8b, v7.8h, %[shift]
            st1 {v7.8b}, [x1], #8

            uaddlp v0.8h, v0.16b
            uaddlp v1.8h, v1.16b
            uaddlp v2.8h, v2.16b
            uadalp v0.8h, v4.16b
            uadalp v1.8h, v5.16b
            uadalp v2.8h, v6.16b

            ushr v0.8h, v0.8h, #1
            ushr v1.8h, v1.8h, #1
            ushr v2.8h, v2.8h, #1

            mul v3.8h, v0.8h, v26.8h
            mls v3.8h, v1.8h, v25.8h
            mls v3.8h, v2.8h, v24.8h
            addhn v3.8b, v3.8h, v22.8h

            mul v4.8h, v2.8h, v26.8h
            mls v4.8h, v1.8h, v27.8h
            mls v4.8h, v0.8h, v28.8h
            addhn v4.8b, v4.8h, v22.8h

            tbl v5.8b, {v3.16b, v4.16b}, v23.8b
            st1 {v5.8b}, [%2], #8

        NV12_2H_L2:
            cmp x3, #2
            blt NV12_2H
            sub x3, x3, #2
            ld4 {v0.b, v1.b, v2.b, v3.b}[0], [%0], #4
            ld4 {v0.b, v1.b, v2.b, v3.b}[1], [%0], #4
            ld4 {v4.b, v5.b, v6.b, v7.b}[0], [x0], #4
            ld4 {v4.b, v5.b, v6.b, v7.b}[1], [x0], #4

            umull v3.8h, v0.8b, v31.8b
            umlal v3.8h, v1.8b, v30.8b
            umlal v3.8h, v2.8b, v29.8b
            umull v7.8h, v4.8b, v31.8b
            umlal v7.8h, v5.8b, v30.8b
            umlal v7.8h, v6.8b, v29.8b

            uqshrn v3.8b, v3.8h, %[shift]
            st1 {v3.h}[0], [%1], #2
            uqshrn v7.8b, v7.8h, %[shift]
            st1 {v7.h}[0], [x1], #2

            uaddlp v0.8h, v0.16b
            uaddlp v1.8h, v1.16b
            uaddlp v2.8h, v2.16b
            uadalp v0.8h, v4.16b
            uadalp v1.8h, v5.16b
            uadalp v2.8h, v6.16b

            ushr v0.8h, v0.8h, #1
            ushr v1.8h, v1.8h, #1
            ushr v2.8h, v2.8h, #1

            mul v3.8h, v0.8h, v26.8h
            mls v3.8h, v1.8h, v25.8h
            mls v3.8h, v2.8h, v24.8h
            addhn v3.8b, v3.8h, v22.8h

            mul v4.8h, v2.8h, v26.8h
            mls v4.8h, v1.8h, v27.8h
            mls v4.8h, v0.8h, v28.8h
            addhn v4.8b, v4.8h, v22.8h

            tbl v5.8b, {v3.16b, v4.16b}, v23.8b
            st1 {v5.h}[0], [%2], #2
            b NV12_2H_L2

        NV12_END:
        )"
        :
        : "r"(src), "r"(y), "r"(uv), [h] "r"(h), [w] "r"(w), [y_param] "r"(k_yuv_param.m_y), [uv_param] "r"(k_yuv_param.m_uv), [idx] "r"(k_uv_tbl_idx), [shift] "i"(k_shift)
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", USED_YUV_REG, "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9");
}

void bgra_to_nv21_asm(const uint8_t *src, int32_t w, int32_t h, uint8_t *dst)
{
    auto y  = dst;
    auto uv = dst + static_cast<ptrdiff_t>(w * h);
    asm volatile(
        LOAD_YUV_PARAM
        R"(
            ld1 {v23.16b}, [%[idx]]

            movi v22.16b, #128
            uaddlp v22.8h, v22.16b
            shl v22.8h, v22.8h, %[shift]
            mov x0, %0
            mov x1, %1
            mov w2, %w[h]
            mov w4, %w[w]
            lsl x4, x4, #2
        NV21_2H:
            mov %0, x0
            mov %1, x1
            cmp x2, #2
            mov w3, %w[w]
            blt NV21_END
            sub x2, x2, #2
            add x0, x0, x4
            add x1, x1, x3

        NV21_2H_L16:
            cmp x3, #16
            blt NV21_2H_L8
            sub x3, x3, #16
            ld4 {v0.16b, v1.16b, v2.16b, v3.16b}, [%0], #64
            ld4 {v4.16b, v5.16b, v6.16b, v7.16b}, [x0], #64

            umull v3.8h, v0.8b, v31.8b
            umlal v3.8h, v1.8b, v30.8b
            umlal v3.8h, v2.8b, v29.8b
            prfm pldl1keep, [%0, 448]
            umull v7.8h, v4.8b, v31.8b
            umlal v7.8h, v5.8b, v30.8b
            umlal v7.8h, v6.8b, v29.8b
            prfm pldl1keep, [x0, 448]
            umull2 v8.8h, v0.16b, v31.16b
            umlal2 v8.8h, v1.16b, v30.16b
            umlal2 v8.8h, v2.16b, v29.16b
            umull2 v9.8h, v4.16b, v31.16b
            umlal2 v9.8h, v5.16b, v30.16b
            umlal2 v9.8h, v6.16b, v29.16b

            uqshrn v3.8b, v3.8h, %[shift]
            uqshrn2 v3.16b, v8.8h, %[shift]
            st1 {v3.16b}, [%1], #16
            uqshrn v7.8b, v7.8h, %[shift]
            uqshrn2 v7.16b, v9.8h, %[shift]
            st1 {v7.16b}, [x1], #16

            uaddlp v0.8h, v0.16b
            uaddlp v1.8h, v1.16b
            uaddlp v2.8h, v2.16b
            uadalp v0.8h, v4.16b
            uadalp v1.8h, v5.16b
            uadalp v2.8h, v6.16b

            ushr v0.8h, v0.8h, #1
            ushr v1.8h, v1.8h, #1
            ushr v2.8h, v2.8h, #1

            mul v4.8h, v0.8h, v26.8h
            mls v4.8h, v1.8h, v25.8h
            mls v4.8h, v2.8h, v24.8h
            addhn v4.8b, v4.8h, v22.8h

            mul v3.8h, v2.8h, v26.8h
            mls v3.8h, v1.8h, v27.8h
            mls v3.8h, v0.8h, v28.8h
            addhn v3.8b, v3.8h, v22.8h

            st2 {v3.8b, v4.8b}, [%2], #16

            b NV21_2H_L16

        NV21_2H_L8:
            cmp x3, #8
            blt NV21_2H_L2
            sub x3, x3, #8
            ld4 {v0.8b, v1.8b, v2.8b, v3.8b}, [%0], #32
            ld4 {v4.8b, v5.8b, v6.8b, v7.8b}, [x0], #32

            umull v3.8h, v0.8b, v31.8b
            umlal v3.8h, v1.8b, v30.8b
            umlal v3.8h, v2.8b, v29.8b
            umull v7.8h, v4.8b, v31.8b
            umlal v7.8h, v5.8b, v30.8b
            umlal v7.8h, v6.8b, v29.8b

            uqshrn v3.8b, v3.8h, %[shift]
            st1 {v3.8b}, [%1], #8
            uqshrn v7.8b, v7.8h, %[shift]
            st1 {v7.8b}, [x1], #8

            uaddlp v0.8h, v0.16b
            uaddlp v1.8h, v1.16b
            uaddlp v2.8h, v2.16b
            uadalp v0.8h, v4.16b
            uadalp v1.8h, v5.16b
            uadalp v2.8h, v6.16b

            ushr v0.8h, v0.8h, #1
            ushr v1.8h, v1.8h, #1
            ushr v2.8h, v2.8h, #1

            mul v4.8h, v0.8h, v26.8h
            mls v4.8h, v1.8h, v25.8h
            mls v4.8h, v2.8h, v24.8h
            addhn v4.8b, v4.8h, v22.8h

            mul v3.8h, v2.8h, v26.8h
            mls v3.8h, v1.8h, v27.8h
            mls v3.8h, v0.8h, v28.8h
            addhn v3.8b, v3.8h, v22.8h

            tbl v5.8b, {v3.16b, v4.16b}, v23.8b
            st1 {v5.8b}, [%2], #8

        NV21_2H_L2:
            cmp x3, #2
            blt NV21_2H
            sub x3, x3, #2
            ld4 {v0.b, v1.b, v2.b, v3.b}[0], [%0], #4
            ld4 {v0.b, v1.b, v2.b, v3.b}[1], [%0], #4
            ld4 {v4.b, v5.b, v6.b, v7.b}[0], [x0], #4
            ld4 {v4.b, v5.b, v6.b, v7.b}[1], [x0], #4

            umull v3.8h, v0.8b, v31.8b
            umlal v3.8h, v1.8b, v30.8b
            umlal v3.8h, v2.8b, v29.8b
            umull v7.8h, v4.8b, v31.8b
            umlal v7.8h, v5.8b, v30.8b
            umlal v7.8h, v6.8b, v29.8b

            uqshrn v3.8b, v3.8h, %[shift]
            st1 {v3.h}[0], [%1], #2
            uqshrn v7.8b, v7.8h, %[shift]
            st1 {v7.h}[0], [x1], #2

            uaddlp v0.8h, v0.16b
            uaddlp v1.8h, v1.16b
            uaddlp v2.8h, v2.16b
            uadalp v0.8h, v4.16b
            uadalp v1.8h, v5.16b
            uadalp v2.8h, v6.16b

            ushr v0.8h, v0.8h, #1
            ushr v1.8h, v1.8h, #1
            ushr v2.8h, v2.8h, #1

            mul v4.8h, v0.8h, v26.8h
            mls v4.8h, v1.8h, v25.8h
            mls v4.8h, v2.8h, v24.8h
            addhn v4.8b, v4.8h, v22.8h

            mul v3.8h, v2.8h, v26.8h
            mls v3.8h, v1.8h, v27.8h
            mls v3.8h, v0.8h, v28.8h
            addhn v3.8b, v3.8h, v22.8h

            tbl v5.8b, {v3.16b, v4.16b}, v23.8b
            st1 {v5.h}[0], [%2], #2
            b NV21_2H_L2

        NV21_END:
        )"
        :
        : "r"(src), "r"(y), "r"(uv), [h] "r"(h), [w] "r"(w), [y_param] "r"(k_yuv_param.m_y), [uv_param] "r"(k_yuv_param.m_uv), [idx] "r"(k_uv_tbl_idx), [shift] "i"(k_shift)
        : "cc", "memory", "x0", "x1", "x2", "x3", "x4", USED_YUV_REG, "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9");
}

NAMESPACE_END
