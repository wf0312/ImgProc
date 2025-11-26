#pragma once

#include "types.hpp"
#include "image.hpp"

NAMESPACE_BEGIN

enum class ResizeType : uint8_t {
    NEAREST = 0,
    BILINEAR = 1,
    CUBIC = 2,
};

struct ResizeParam {
    bool m_half_pixel{true};    // add 0.5, (x, y) --> (x + 0.5, y + 0.5)
    bool m_align_corner{false};   // x -= 1, y -= 1
    ResizeType m_resize_type{ResizeType::BILINEAR};
};

void resize(const Image&, const Image &, ResizeParam);
NAMESPACE_END
