#include "cvt_test.hpp"
#include "logger.hpp"

#include "types.hpp"
#include "cvt_color.hpp"

#include "image.hpp"
#include "test.hpp"

NAMESPACE_BEGIN

constexpr char k_cvt_name_fmt[] = "{}_{}_{}_{}";
constexpr char k_asm_name_fmt[] = "{}_asm";
constexpr char k_yuv_name_fmt[] = "{}_yuv";
using CvtFunction = void(*)(const Image&, const Image&);
struct CvtFunc {
    CvtFunction* m_func_c{nullptr};
    CvtFunction* m_func_asm{nullptr};
};
// static std::array<std::array<CvtFunc, getValueOf(ImageFormat::END)>, getValueOf(ImageFormat::END)> g_func = {
//     {CvtFunc{}, CvtFunc{}, CvtFunc{}},  // gray
//     {CvtFunc{}, CvtFunc{}, CvtFunc{}},  // rgba
//     {CvtFunc{}, CvtFunc{}, CvtFunc{}},  // rgb
//     {CvtFunc{}, CvtFunc{}, CvtFunc{}},  // bgra
//     {CvtFunc{}, CvtFunc{}, CvtFunc{}},  // bgr
//     {CvtFunc{.}, CvtFunc{}, CvtFunc{}},  // yuyv
// }

void fromRgb(const Image& src, uint32_t dst_type, uint32_t loop)
{
    LOGD("From Rgb");
    const auto src_fmt        = ImageFormat::RGB;
    auto src_idx              = static_cast<uint32_t>(src_fmt);
    const auto& src_type_name = getImgFmtName(src_fmt);
    {
        auto type = ImageFormat::GRAY;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgb_to_gray_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgb_to_gray(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::RGBA;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgb_to_rgba_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgb_to_rgba(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::RGB;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgb_to_rgb_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgb_to_rgb(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::BGRA;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgb_to_bgra_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgb_to_bgra(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::BGR;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgb_to_bgr_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgb_to_bgr(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::YUYV;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgb_to_yuyv_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgb_to_yuyv(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::UYVY;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgb_to_uyvy_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgb_to_uyvy(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::I420;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgb_to_i420_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgb_to_i420(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::NV12;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgb_to_nv12_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgb_to_nv12(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::NV21;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgb_to_nv21_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgb_to_nv21(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
}
void fromRgba(const Image& src, uint32_t dst_type, uint32_t loop)
{
    LOGD("From Rgba");
    const auto src_fmt        = ImageFormat::RGBA;
    auto src_idx              = static_cast<uint32_t>(src_fmt);
    const auto& src_type_name = getImgFmtName(src_fmt);
    {
        auto type = ImageFormat::GRAY;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgba_to_gray_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgba_to_gray(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::RGBA;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgba_to_rgba_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgba_to_rgba(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::RGB;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgba_to_rgb_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgba_to_rgb(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::BGRA;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgba_to_bgra_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgba_to_bgra(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::BGR;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgba_to_bgr_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgba_to_bgr(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::YUYV;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgba_to_yuyv_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgba_to_yuyv(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::UYVY;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgba_to_uyvy_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgba_to_uyvy(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::I420;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgba_to_i420_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgba_to_i420(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::NV12;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgba_to_nv12_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgba_to_nv12(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::NV21;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     rgba_to_nv21_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    rgba_to_nv21(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
}
/*
void fromBgra(const std::vector<uint8_t>& src_buf, uint32_t dst_type, int32_t w, int32_t h, int32_t loop)
{
    std::vector<uint8_t> dst_buf;
    const auto src_fmt        = ImageFormat::BGRA;
    auto src_idx              = static_cast<uint32_t>(src_fmt);
    const auto& src_type_name = getImgFmtName(src_fmt);
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::GRAY);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     rgba_to_gray(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_gray_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::RGBA);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_rgba(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_rgba_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::RGB);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_rgb(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_rgb_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::BGRA);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_bgra(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_bgra_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::BGR);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_bgr(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_bgr_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::YUYV);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_yuyv(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_yuyv_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::UYVY);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_uyvy(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_uyvy_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::I420);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_i420(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_i420_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::NV12);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_nv12(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_nv12_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::NV21);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_nv21(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgra_to_nv21_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
}

void fromBgr(const std::vector<uint8_t>& src_buf, uint32_t dst_type, int32_t w, int32_t h, int32_t loop)
{
    std::vector<uint8_t> dst_buf;
    const auto src_fmt        = ImageFormat::BGR;
    auto src_idx              = static_cast<uint32_t>(src_fmt);
    const auto& src_type_name = getImgFmtName(src_fmt);
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::GRAY);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_gray(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_gray_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::RGBA);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_rgba(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_rgba_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::RGB);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_rgb(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_rgb_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::BGRA);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_bgra(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_bgra_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::BGR);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_bgr(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_bgr_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::YUYV);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_yuyv(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_yuyv_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::UYVY);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_uyvy(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_uyvy_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::I420);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_i420(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_i420_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::NV12);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_nv12(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_nv12_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto dst_idx = static_cast<uint32_t>(ImageFormat::NV21);
        dst_buf.resize(getImgSize(w, h, static_cast<ImageFormat>(dst_idx)));
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(static_cast<ImageFormat>(dst_idx)), w, h);
            auto cost0 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_nv21(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src_buf, &dst_buf, w, h]() {
                                     bgr_to_nv21_asm(src_buf.data(), w, h, const_cast<uint8_t*>(dst_buf.data()));  // NOLINT
                                 },
                                 dst_buf},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
}
*/
void fromYuyv(const Image& src, uint32_t dst_type, uint32_t loop)
{
    LOGD("From Yuyv");
    const auto src_fmt        = ImageFormat::YUYV;
    auto src_idx              = static_cast<uint32_t>(src_fmt);
    const auto& src_type_name = getImgFmtName(src_fmt);
    {
        auto type = ImageFormat::GRAY;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     yuyv_to_gray_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    yuyv_to_gray(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::RGBA;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     yuyv_to_rgba_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    yuyv_to_rgba(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::RGB;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     yuyv_to_rgb_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    yuyv_to_rgb(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::BGRA;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     yuyv_to_bgra_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    yuyv_to_bgra(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::BGR;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     yuyv_to_bgr_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    yuyv_to_bgr(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::YUYV;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     yuyv_to_yuyv_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    yuyv_to_yuyv(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::UYVY;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     yuyv_to_uyvy_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    yuyv_to_uyvy(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::I420;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     yuyv_to_i420_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    yuyv_to_i420(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::NV12;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     yuyv_to_nv12_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    yuyv_to_nv12(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::NV21;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     yuyv_to_nv21_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    yuyv_to_nv21(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
}

void fromUyvy(const Image& src, uint32_t dst_type, uint32_t loop)
{
    LOGD("From Uyyv");
    const auto src_fmt        = ImageFormat::UYVY;
    auto src_idx              = static_cast<uint32_t>(src_fmt);
    const auto& src_type_name = getImgFmtName(src_fmt);
    {
        auto type = ImageFormat::GRAY;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     uyvy_to_gray_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    uyvy_to_gray(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::RGBA;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     uyvy_to_rgba_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    uyvy_to_rgba(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::RGB;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     uyvy_to_rgb_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    uyvy_to_rgb(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::BGRA;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     uyvy_to_bgra_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    uyvy_to_bgra(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::BGR;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     uyvy_to_bgr_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    uyvy_to_bgr(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::YUYV;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     uyvy_to_yuyv_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    uyvy_to_yuyv(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::UYVY;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     uyvy_to_uyvy_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    uyvy_to_uyvy(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::I420;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     uyvy_to_i420_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    uyvy_to_i420(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::NV12;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     uyvy_to_nv12_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    uyvy_to_nv12(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
    {
        auto type = ImageFormat::NV21;
        auto dst_idx = static_cast<uint32_t>(type);
        Image dst;
        dst.create(src.rows(), src.cols(), type);
        if (0 != (dst_type & (1 << dst_idx))) {
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(type), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst]() {
                                     uyvy_to_nv21_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst]() {
                                    uyvy_to_nv21(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
}

NAMESPACE_END
