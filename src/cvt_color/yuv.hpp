#pragma once

#include "types.hpp"

NAMESPACE_BEGIN

constexpr uint8_t k_shift = 6U;
constexpr uint8_t k_alpha = 255U;
static const struct Yuv2RgbParam {  // NOLINT
    uint8_t m_vr = 90;
    uint8_t m_ug = 22;
    uint8_t m_vg = 46;
    uint8_t m_ub = 113;
    uint16_t m_ofs_r = 90*128;
    uint16_t m_ofs_g = (22 + 46) * 128;
    uint16_t m_ofs_b = 113*128;
} k_yuv_2_rgb{};

#define LOAD_RGB_PARAM                                              \
    "ld4r {v28.16b, v29.16b, v30.16b, v31.16b}, [%[param]], #4  \n" \
    "ld3r {v25.8h, v26.8h, v27.8h}, [%[param]]                  \n"


NAMESPACE_END
