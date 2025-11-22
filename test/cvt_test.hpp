#pragma once

#include "image.hpp"
#include "types.hpp"

NAMESPACE_BEGIN

void fromGray(const Image&, uint32_t, uint32_t);
void fromRgba(const Image&, uint32_t, uint32_t);
void fromRgb(const Image&, uint32_t, uint32_t);
void fromBgra(const Image&, uint32_t, uint32_t);
void fromBgr(const Image&, uint32_t, uint32_t);
void fromYuyv(const Image&, uint32_t, uint32_t);
void fromUyvy(const Image&, uint32_t, uint32_t);
void fromI420(const Image&, uint32_t, uint32_t);
void fromNv12(const Image&, uint32_t, uint32_t);
void fromNv21(const Image&, uint32_t, uint32_t);

NAMESPACE_END
