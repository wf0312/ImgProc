#pragma once

#include "types.hpp"

NAMESPACE_BEGIN

constexpr uint8_t k_shift = 7U;
constexpr uint8_t k_alpha = 255;
static const struct Rgb2YuvParam {  // NOLINT
    uint8_t m_yr = 38;
    uint8_t m_yg = 75;
    uint8_t m_yb = 15;
    uint8_t m_placehold = 0;
    uint16_t m_ur = 22;    // -
    uint16_t m_ug = 42;    // -
    uint16_t m_ub = 64;    // +
    uint16_t m_vr = 64;    // +
    uint16_t m_vg = 54;    // -
    uint16_t m_vb = 10;    // -
} k_rgb_2_yuv{};

#define LOAD_Y_PARAM                                        \
    "mov x0, #4 \n"                                         \
    "ld3r {v29.16b, v30.16b, v31.16b}, [%[param]], x0   \n"

#define LOAD_YUV_PARAM                                  \
    LOAD_Y_PARAM                                        \
    "ld3r {v24.8h, v25.8h, v26.8h}, [%[param]], #6  \n" \
    "ld3r {v26.8h, v27.8h, v28.8h}, [%[param]], #6  \n"

#define USED_Y_REG  "x0", "v31", "v30", "v29"
#define USED_YUV_REG  USED_Y_REG, "v28", "v27", "v26", "v25", "v24"

NAMESPACE_END
