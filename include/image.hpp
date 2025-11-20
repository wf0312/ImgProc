#pragma once

#include "types.hpp"
#include <string>

NAMESPACE_BEGIN

enum class ImageFormat : uint8_t
{
    GRAY,       // 0
    RGBA,
    RGB,
    BGRA,
    BGR,
    YUYV,       // 5
    UYVY,
    I420,
    NV12,
    NV21,

    END
};

size_t getImgStride(int32_t, ImageFormat);
size_t getImgSize(int32_t, int32_t, ImageFormat);
const std::string &getImgFmtName(ImageFormat);
ImageFormat getImgFmt(const std::string &name);

class Image {
public:
    Image() = default;
    Image(int32_t, int32_t, ImageFormat, uint8_t* = nullptr);

    Image(const Image&) = delete;
    Image(Image&&) noexcept;
    Image& operator=(const Image&) = delete;
    Image& operator=(Image&&) noexcept;

    ~Image();

    void create(int32_t, int32_t, ImageFormat, uint8_t* = nullptr);
    Image clone() const;
    int32_t rows() const;
    int32_t cols() const;
    uint8_t *data() const;
    size_t pixels() const;
    size_t size() const;
    size_t stride() const;
    ImageFormat fmt() const;

private:
    int32_t width_{0};
    int32_t height_{0};
    uint64_t flag_{0};     // own_ptr | img_fmt | size
    uint8_t* data_{nullptr};
};

NAMESPACE_END
