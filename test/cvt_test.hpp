#pragma once

#include "image.hpp"
#include "types.hpp"

NAMESPACE_BEGIN

void fromRgb(const Image&, uint32_t, uint32_t);
void fromRgba(const Image&, uint32_t, uint32_t);
void fromYuyv(const Image&, uint32_t, uint32_t);
void fromUyvy(const Image&, uint32_t, uint32_t);

NAMESPACE_END
