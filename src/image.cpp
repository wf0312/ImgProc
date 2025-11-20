#include <array>
#include <string>
#include "image.hpp"

#include "types.hpp"

NAMESPACE_BEGIN

constexpr uint8_t k_own_ptr_shift = 63;
constexpr uint64_t k_own_ptr_mask = 1LU << k_own_ptr_shift;
constexpr uint8_t k_img_fmt_shift = 56;
constexpr uint64_t k_img_fmt_mask = (0x7fLU) << k_img_fmt_shift;
constexpr uint64_t k_img_size_mask    = 0x0000'00ff'ffff'ffff;

static const std::array<std::string, getValueOf(ImageFormat::END) + 1> g_fmt_name{
    "gray", "rgba", "rgb", "bgra", "bgr",
    "yuyv", "uyvy", "i420", "nv12", "nv21",
    "end"
};

const std::string &getImgFmtName(ImageFormat fmt) {
    if (fmt >= ImageFormat::GRAY && fmt <= ImageFormat::END) {
        return g_fmt_name[getValueOf(fmt)];
    }
    return g_fmt_name[getValueOf(ImageFormat::END)];
}

ImageFormat getImgFmt(const std::string &name) {
    for (auto i = 0; i < getValueOf(ImageFormat::END); ++i) {
        if (name == g_fmt_name[i]) {
            return static_cast<ImageFormat>(i);
        }
    }

    return ImageFormat::END;
}

size_t getImgStride(int32_t w, ImageFormat fmt) {
    switch (fmt) {
        case ImageFormat::GRAY:
            return 1UL * w;
        case ImageFormat::RGBA:
        case ImageFormat::BGRA:
            return 4UL * w;
        case ImageFormat::RGB:
        case ImageFormat::BGR:
            return 3UL * w;
        case ImageFormat::YUYV:
        case ImageFormat::UYVY:
            return 2UL * w;
        case ImageFormat::I420:
        case ImageFormat::NV12:
        case ImageFormat::NV21:
            return (3UL * w) >> 1;
        case ImageFormat::END:
            return 0;
    }
}

size_t getImgSize(int32_t w, int32_t h, ImageFormat fmt) {
    return getImgStride(w, fmt) * h;
}

Image::Image(int32_t h, int32_t w, ImageFormat fmt, uint8_t* data)
    : width_{w}
    , height_{h}
    , flag_{(static_cast<uint64_t>(fmt) << k_img_fmt_shift) | (getImgSize(w, h, fmt) & k_img_size_mask)}
    , data_{data}
{
}

Image::Image(Image&& that) noexcept
    : width_{that.width_}
    , height_{that.height_}
    , flag_{that.flag_}
    , data_{that.data_}
{
    that.data_ = nullptr;
}

Image& Image::operator=(Image&& rhs) noexcept {
    if (this != &rhs) {
        width_ = rhs.width_;
        height_ = rhs.height_;
        flag_ = rhs.flag_;
        data_ = rhs.data_;
        rhs.data_ = nullptr;
    }
    return *this;
}

Image::~Image() {
    if (0 != (k_own_ptr_mask & flag_)) {
        delete[] data_;
        data_ = nullptr;
    }
}

void Image::create(int32_t h, int32_t w, ImageFormat fmt, uint8_t* data) {
    width_ = w;
    height_ = h;
    flag_ = (static_cast<uint64_t>(fmt) << k_img_fmt_shift) | (getImgSize(w, h, fmt) & k_img_size_mask);
    delete[] data_;
    if (nullptr == data) {
        data_ = new uint8_t[getImgSize(w, h, fmt)];
        flag_ |= k_own_ptr_mask;
    } else {
        data_ = data;
        flag_ &= ~k_own_ptr_mask;
    }
}

Image Image::clone() const {
    Image tmp;
    tmp.create(height_, width_, fmt());
    std::copy_n(data_, size(), tmp.data());
    return tmp;
}

int32_t Image::rows() const
{
    return height_;
}

int32_t Image::cols() const {
    return width_;
}

uint8_t* Image::data() const {
    return data_;
}

size_t Image::pixels() const {
    return static_cast<size_t>(width_) * height_;
}

size_t Image::size() const {
    return flag_ & k_img_size_mask;
}

size_t Image::stride() const {
    return getImgStride(width_, fmt());
}

ImageFormat Image::fmt() const {
    return static_cast<ImageFormat>((flag_ & k_img_fmt_mask) >> k_img_fmt_shift);
}

NAMESPACE_END
