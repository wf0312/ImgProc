#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#define NAMESPACE_BEGIN namespace zycs {
#define NAMESPACE_END }

NAMESPACE_BEGIN

using uint8_t   = std::uint8_t;
using uint16_t  = std::uint16_t;
using uint32_t  = std::uint32_t;
using uint64_t  = std::uint64_t;
using int8_t    = std::int8_t;
using int16_t   = std::int16_t;
using int32_t   = std::int32_t;
using int64_t   = std::int64_t;
using size_t    = std::size_t;
using ptrdiff_t = std::ptrdiff_t;

template <typename T, typename U = std::underlying_type_t<T>>
constexpr auto getValueOf(T t)
{
    return static_cast<U>(t);
}

constexpr uint8_t saturate_u8(int64_t value)
{
    return static_cast<uint8_t>(std::min(std::max(value, 0L), 255L));
}

NAMESPACE_END
