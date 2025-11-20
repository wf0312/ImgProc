#pragma once

#include "types.hpp"
#include "image.hpp"

NAMESPACE_BEGIN

#define ADD_IMG_CONVERT(F)                  \
    void F(const Image&, const Image&);     \
    void F##_c(const Image&, const Image&)

// from gray
ADD_IMG_CONVERT(gray_to_gray);
ADD_IMG_CONVERT(gray_to_rgba);
ADD_IMG_CONVERT(gray_to_rgb);
ADD_IMG_CONVERT(gray_to_bgra);
ADD_IMG_CONVERT(gray_to_bgr);
ADD_IMG_CONVERT(gray_to_yuyv);
ADD_IMG_CONVERT(gray_to_uyvy);
ADD_IMG_CONVERT(gray_to_i420);
ADD_IMG_CONVERT(gray_to_nv12);
ADD_IMG_CONVERT(gray_to_nv21);

// from rgba
ADD_IMG_CONVERT(rgba_to_gray);
ADD_IMG_CONVERT(rgba_to_rgba);
ADD_IMG_CONVERT(rgba_to_rgb);
ADD_IMG_CONVERT(rgba_to_bgra);
ADD_IMG_CONVERT(rgba_to_bgr);
ADD_IMG_CONVERT(rgba_to_yuyv);
ADD_IMG_CONVERT(rgba_to_uyvy);
ADD_IMG_CONVERT(rgba_to_i420);
ADD_IMG_CONVERT(rgba_to_nv12);
ADD_IMG_CONVERT(rgba_to_nv21);

// from rgb
ADD_IMG_CONVERT(rgb_to_gray);
ADD_IMG_CONVERT(rgb_to_rgba);
ADD_IMG_CONVERT(rgb_to_rgb);
ADD_IMG_CONVERT(rgb_to_bgra);
ADD_IMG_CONVERT(rgb_to_bgr);
ADD_IMG_CONVERT(rgb_to_yuyv);
ADD_IMG_CONVERT(rgb_to_uyvy);
ADD_IMG_CONVERT(rgb_to_i420);
ADD_IMG_CONVERT(rgb_to_nv12);
ADD_IMG_CONVERT(rgb_to_nv21);

// from bgr
ADD_IMG_CONVERT(bgr_to_gray);
ADD_IMG_CONVERT(bgr_to_rgba);
ADD_IMG_CONVERT(bgr_to_rgb);
ADD_IMG_CONVERT(bgr_to_bgra);
ADD_IMG_CONVERT(bgr_to_bgr);
ADD_IMG_CONVERT(bgr_to_yuyv);
ADD_IMG_CONVERT(bgr_to_uyvy);
ADD_IMG_CONVERT(bgr_to_i420);
ADD_IMG_CONVERT(bgr_to_nv12);
ADD_IMG_CONVERT(bgr_to_nv21);

// from bgra
ADD_IMG_CONVERT(bgra_to_gray);
ADD_IMG_CONVERT(bgra_to_rgba);
ADD_IMG_CONVERT(bgra_to_rgb);
ADD_IMG_CONVERT(bgra_to_bgra);
ADD_IMG_CONVERT(bgra_to_bgr);
ADD_IMG_CONVERT(bgra_to_yuyv);
ADD_IMG_CONVERT(bgra_to_uyvy);
ADD_IMG_CONVERT(bgra_to_i420);
ADD_IMG_CONVERT(bgra_to_nv12);
ADD_IMG_CONVERT(bgra_to_nv21);

// from yuyv
ADD_IMG_CONVERT(yuyv_to_gray);
ADD_IMG_CONVERT(yuyv_to_rgba);
ADD_IMG_CONVERT(yuyv_to_rgb);
ADD_IMG_CONVERT(yuyv_to_bgra);
ADD_IMG_CONVERT(yuyv_to_bgr);
ADD_IMG_CONVERT(yuyv_to_yuyv);
ADD_IMG_CONVERT(yuyv_to_uyvy);
ADD_IMG_CONVERT(yuyv_to_i420);
ADD_IMG_CONVERT(yuyv_to_nv12);
ADD_IMG_CONVERT(yuyv_to_nv21);

// from uyvy
ADD_IMG_CONVERT(uyvy_to_gray);
ADD_IMG_CONVERT(uyvy_to_rgba);
ADD_IMG_CONVERT(uyvy_to_rgb);
ADD_IMG_CONVERT(uyvy_to_bgra);
ADD_IMG_CONVERT(uyvy_to_bgr);
ADD_IMG_CONVERT(uyvy_to_yuyv);
ADD_IMG_CONVERT(uyvy_to_uyvy);
ADD_IMG_CONVERT(uyvy_to_i420);
ADD_IMG_CONVERT(uyvy_to_nv12);
ADD_IMG_CONVERT(uyvy_to_nv21);

NAMESPACE_END
