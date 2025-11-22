#include "cvt_test.hpp"
#include <array>
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
struct CvtFuncStruct {
    CvtFunction m_func_c{nullptr};
    CvtFunction m_func{nullptr};
};

static const std::array<std::array<CvtFuncStruct, getValueOf(ImageFormat::END)>, getValueOf(ImageFormat::END)> g_cvt_func = {
    CvtFuncStruct{.m_func_c = gray_to_gray_c,    .m_func = gray_to_gray},
    CvtFuncStruct{.m_func_c = gray_to_rgba_c,    .m_func = gray_to_rgba},
    CvtFuncStruct{.m_func_c = gray_to_rgb_c,     .m_func = gray_to_rgb},
    CvtFuncStruct{.m_func_c = gray_to_bgra_c,    .m_func = gray_to_bgra},
    CvtFuncStruct{.m_func_c = gray_to_bgr_c,     .m_func = gray_to_bgr},
    CvtFuncStruct{.m_func_c = gray_to_yuyv_c,    .m_func = gray_to_yuyv},
    CvtFuncStruct{.m_func_c = gray_to_uyvy_c,    .m_func = gray_to_uyvy},
    CvtFuncStruct{.m_func_c = gray_to_i420_c,    .m_func = gray_to_i420},
    CvtFuncStruct{.m_func_c = gray_to_nv12_c,    .m_func = gray_to_nv12},
    CvtFuncStruct{.m_func_c = gray_to_nv21_c,    .m_func = gray_to_nv21},

    CvtFuncStruct{.m_func_c = rgba_to_gray_c,    .m_func = rgba_to_gray},
    CvtFuncStruct{.m_func_c = rgba_to_rgba_c,    .m_func = rgba_to_rgba},
    CvtFuncStruct{.m_func_c = rgba_to_rgb_c,     .m_func = rgba_to_rgb},
    CvtFuncStruct{.m_func_c = rgba_to_bgra_c,    .m_func = rgba_to_bgra},
    CvtFuncStruct{.m_func_c = rgba_to_bgr_c,     .m_func = rgba_to_bgr},
    CvtFuncStruct{.m_func_c = rgba_to_yuyv_c,    .m_func = rgba_to_yuyv},
    CvtFuncStruct{.m_func_c = rgba_to_uyvy_c,    .m_func = rgba_to_uyvy},
    CvtFuncStruct{.m_func_c = rgba_to_i420_c,    .m_func = rgba_to_i420},
    CvtFuncStruct{.m_func_c = rgba_to_nv12_c,    .m_func = rgba_to_nv12},
    CvtFuncStruct{.m_func_c = rgba_to_nv21_c,    .m_func = rgba_to_nv21},

    CvtFuncStruct{.m_func_c = rgb_to_gray_c,    .m_func = rgb_to_gray},
    CvtFuncStruct{.m_func_c = rgb_to_rgba_c,    .m_func = rgb_to_rgba},
    CvtFuncStruct{.m_func_c = rgb_to_rgb_c,     .m_func = rgb_to_rgb},
    CvtFuncStruct{.m_func_c = rgb_to_bgra_c,    .m_func = rgb_to_bgra},
    CvtFuncStruct{.m_func_c = rgb_to_bgr_c,     .m_func = rgb_to_bgr},
    CvtFuncStruct{.m_func_c = rgb_to_yuyv_c,    .m_func = rgb_to_yuyv},
    CvtFuncStruct{.m_func_c = rgb_to_uyvy_c,    .m_func = rgb_to_uyvy},
    CvtFuncStruct{.m_func_c = rgb_to_i420_c,    .m_func = rgb_to_i420},
    CvtFuncStruct{.m_func_c = rgb_to_nv12_c,    .m_func = rgb_to_nv12},
    CvtFuncStruct{.m_func_c = rgb_to_nv21_c,    .m_func = rgb_to_nv21},

    CvtFuncStruct{.m_func_c = bgra_to_gray_c,    .m_func = bgra_to_gray},
    CvtFuncStruct{.m_func_c = bgra_to_rgba_c,    .m_func = bgra_to_rgba},
    CvtFuncStruct{.m_func_c = bgra_to_rgb_c,     .m_func = bgra_to_rgb},
    CvtFuncStruct{.m_func_c = bgra_to_bgra_c,    .m_func = bgra_to_bgra},
    CvtFuncStruct{.m_func_c = bgra_to_bgr_c,     .m_func = bgra_to_bgr},
    CvtFuncStruct{.m_func_c = bgra_to_yuyv_c,    .m_func = bgra_to_yuyv},
    CvtFuncStruct{.m_func_c = bgra_to_uyvy_c,    .m_func = bgra_to_uyvy},
    CvtFuncStruct{.m_func_c = bgra_to_i420_c,    .m_func = bgra_to_i420},
    CvtFuncStruct{.m_func_c = bgra_to_nv12_c,    .m_func = bgra_to_nv12},
    CvtFuncStruct{.m_func_c = bgra_to_nv21_c,    .m_func = bgra_to_nv21},

    CvtFuncStruct{.m_func_c = bgr_to_gray_c,    .m_func = bgr_to_gray},
    CvtFuncStruct{.m_func_c = bgr_to_rgba_c,    .m_func = bgr_to_rgba},
    CvtFuncStruct{.m_func_c = bgr_to_rgb_c,     .m_func = bgr_to_rgb},
    CvtFuncStruct{.m_func_c = bgr_to_bgra_c,    .m_func = bgr_to_bgra},
    CvtFuncStruct{.m_func_c = bgr_to_bgr_c,     .m_func = bgr_to_bgr},
    CvtFuncStruct{.m_func_c = bgr_to_yuyv_c,    .m_func = bgr_to_yuyv},
    CvtFuncStruct{.m_func_c = bgr_to_uyvy_c,    .m_func = bgr_to_uyvy},
    CvtFuncStruct{.m_func_c = bgr_to_i420_c,    .m_func = bgr_to_i420},
    CvtFuncStruct{.m_func_c = bgr_to_nv12_c,    .m_func = bgr_to_nv12},
    CvtFuncStruct{.m_func_c = bgr_to_nv21_c,    .m_func = bgr_to_nv21},

    CvtFuncStruct{.m_func_c = yuyv_to_gray_c,    .m_func = yuyv_to_gray},
    CvtFuncStruct{.m_func_c = yuyv_to_rgba_c,    .m_func = yuyv_to_rgba},
    CvtFuncStruct{.m_func_c = yuyv_to_rgb_c,     .m_func = yuyv_to_rgb},
    CvtFuncStruct{.m_func_c = yuyv_to_bgra_c,    .m_func = yuyv_to_bgra},
    CvtFuncStruct{.m_func_c = yuyv_to_bgr_c,     .m_func = yuyv_to_bgr},
    CvtFuncStruct{.m_func_c = yuyv_to_yuyv_c,    .m_func = yuyv_to_yuyv},
    CvtFuncStruct{.m_func_c = yuyv_to_uyvy_c,    .m_func = yuyv_to_uyvy},
    CvtFuncStruct{.m_func_c = yuyv_to_i420_c,    .m_func = yuyv_to_i420},
    CvtFuncStruct{.m_func_c = yuyv_to_nv12_c,    .m_func = yuyv_to_nv12},
    CvtFuncStruct{.m_func_c = yuyv_to_nv21_c,    .m_func = yuyv_to_nv21},

    CvtFuncStruct{.m_func_c = uyvy_to_gray_c,    .m_func = uyvy_to_gray},
    CvtFuncStruct{.m_func_c = uyvy_to_rgba_c,    .m_func = uyvy_to_rgba},
    CvtFuncStruct{.m_func_c = uyvy_to_rgb_c,     .m_func = uyvy_to_rgb},
    CvtFuncStruct{.m_func_c = uyvy_to_bgra_c,    .m_func = uyvy_to_bgra},
    CvtFuncStruct{.m_func_c = uyvy_to_bgr_c,     .m_func = uyvy_to_bgr},
    CvtFuncStruct{.m_func_c = uyvy_to_yuyv_c,    .m_func = uyvy_to_yuyv},
    CvtFuncStruct{.m_func_c = uyvy_to_uyvy_c,    .m_func = uyvy_to_uyvy},
    CvtFuncStruct{.m_func_c = uyvy_to_i420_c,    .m_func = uyvy_to_i420},
    CvtFuncStruct{.m_func_c = uyvy_to_nv12_c,    .m_func = uyvy_to_nv12},
    CvtFuncStruct{.m_func_c = uyvy_to_nv21_c,    .m_func = uyvy_to_nv21},

    CvtFuncStruct{.m_func_c = i420_to_gray_c,    .m_func = i420_to_gray},
    CvtFuncStruct{.m_func_c = i420_to_rgba_c,    .m_func = i420_to_rgba},
    CvtFuncStruct{.m_func_c = i420_to_rgb_c,     .m_func = i420_to_rgb},
    CvtFuncStruct{.m_func_c = i420_to_bgra_c,    .m_func = i420_to_bgra},
    CvtFuncStruct{.m_func_c = i420_to_bgr_c,     .m_func = i420_to_bgr},
    CvtFuncStruct{.m_func_c = i420_to_yuyv_c,    .m_func = i420_to_yuyv},
    CvtFuncStruct{.m_func_c = i420_to_uyvy_c,    .m_func = i420_to_uyvy},
    CvtFuncStruct{.m_func_c = i420_to_i420_c,    .m_func = i420_to_i420},
    CvtFuncStruct{.m_func_c = i420_to_nv12_c,    .m_func = i420_to_nv12},
    CvtFuncStruct{.m_func_c = i420_to_nv21_c,    .m_func = i420_to_nv21},

    CvtFuncStruct{.m_func_c = nv12_to_gray_c,    .m_func = nv12_to_gray},
    CvtFuncStruct{.m_func_c = nv12_to_rgba_c,    .m_func = nv12_to_rgba},
    CvtFuncStruct{.m_func_c = nv12_to_rgb_c,     .m_func = nv12_to_rgb},
    CvtFuncStruct{.m_func_c = nv12_to_bgra_c,    .m_func = nv12_to_bgra},
    CvtFuncStruct{.m_func_c = nv12_to_bgr_c,     .m_func = nv12_to_bgr},
    CvtFuncStruct{.m_func_c = nv12_to_yuyv_c,    .m_func = nv12_to_yuyv},
    CvtFuncStruct{.m_func_c = nv12_to_uyvy_c,    .m_func = nv12_to_uyvy},
    CvtFuncStruct{.m_func_c = nv12_to_i420_c,    .m_func = nv12_to_i420},
    CvtFuncStruct{.m_func_c = nv12_to_nv12_c,    .m_func = nv12_to_nv12},
    CvtFuncStruct{.m_func_c = nv12_to_nv21_c,    .m_func = nv12_to_nv21},

    CvtFuncStruct{.m_func_c = nv21_to_gray_c,    .m_func = nv21_to_gray},
    CvtFuncStruct{.m_func_c = nv21_to_rgba_c,    .m_func = nv21_to_rgba},
    CvtFuncStruct{.m_func_c = nv21_to_rgb_c,     .m_func = nv21_to_rgb},
    CvtFuncStruct{.m_func_c = nv21_to_bgra_c,    .m_func = nv21_to_bgra},
    CvtFuncStruct{.m_func_c = nv21_to_bgr_c,     .m_func = nv21_to_bgr},
    CvtFuncStruct{.m_func_c = nv21_to_yuyv_c,    .m_func = nv21_to_yuyv},
    CvtFuncStruct{.m_func_c = nv21_to_uyvy_c,    .m_func = nv21_to_uyvy},
    CvtFuncStruct{.m_func_c = nv21_to_i420_c,    .m_func = nv21_to_i420},
    CvtFuncStruct{.m_func_c = nv21_to_nv12_c,    .m_func = nv21_to_nv12},
    CvtFuncStruct{.m_func_c = nv21_to_nv21_c,    .m_func = nv21_to_nv21},
};

void fromGray(const Image& src, uint32_t dst_type, uint32_t loop) {
    LOGD("From Gray");
    const auto src_fmt = ImageFormat::GRAY;
    auto src_idx       = static_cast<uint32_t>(src_fmt);
    const auto& src_type_name = getImgFmtName(src_fmt);
    for (auto i = 0U; i < getValueOf(ImageFormat::END); ++i) {
        if (0 != (dst_type & (1 << i))) {
            auto img_fmt = static_cast<ImageFormat>(i);
            Image dst;
            dst.create(src.rows(), src.cols(), img_fmt);
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(img_fmt), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst, src_idx, i]() {
                                     g_cvt_func[src_idx][i].m_func_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst, src_idx, i]() {
                                    g_cvt_func[src_idx][i].m_func(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
}

void fromBgra(const Image& src, uint32_t dst_type, uint32_t loop) {
    LOGD("From Bgra");
    const auto src_fmt = ImageFormat::BGRA;
    auto src_idx       = static_cast<uint32_t>(src_fmt);
    const auto& src_type_name = getImgFmtName(src_fmt);
    for (auto i = 0U; i < getValueOf(ImageFormat::END); ++i) {
        if (0 != (dst_type & (1 << i))) {
            auto img_fmt = static_cast<ImageFormat>(i);
            Image dst;
            dst.create(src.rows(), src.cols(), img_fmt);
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(img_fmt), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst, src_idx, i]() {
                                     g_cvt_func[src_idx][i].m_func_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst, src_idx, i]() {
                                    g_cvt_func[src_idx][i].m_func(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
}

void fromBgr(const Image& src, uint32_t dst_type, uint32_t loop) {
    LOGD("From Bgr");
    const auto src_fmt        = ImageFormat::BGR;
    auto src_idx              = static_cast<uint32_t>(src_fmt);
    const auto& src_type_name = getImgFmtName(src_fmt);
    for (auto i = 0U; i < getValueOf(ImageFormat::END); ++i) {
        if (0 != (dst_type & (1 << i))) {
            auto img_fmt = static_cast<ImageFormat>(i);
            Image dst;
            dst.create(src.rows(), src.cols(), img_fmt);
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(img_fmt), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst, src_idx, i]() {
                                     g_cvt_func[src_idx][i].m_func_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst, src_idx, i]() {
                                    g_cvt_func[src_idx][i].m_func(src, dst);  // NOLINT
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
    auto src_idx       = static_cast<uint32_t>(src_fmt);
    const auto& src_type_name = getImgFmtName(src_fmt);
    for (auto i = 0U; i < getValueOf(ImageFormat::END); ++i) {
        if (0 != (dst_type & (1 << i))) {
            auto img_fmt = static_cast<ImageFormat>(i);
            Image dst;
            dst.create(src.rows(), src.cols(), img_fmt);
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(img_fmt), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst, src_idx, i]() {
                                     g_cvt_func[src_idx][i].m_func_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst, src_idx, i]() {
                                    g_cvt_func[src_idx][i].m_func(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
}

void fromRgb(const Image& src, uint32_t dst_type, uint32_t loop)
{
    LOGD("From Rgb");
    const auto src_fmt        = ImageFormat::RGB;
    auto src_idx       = static_cast<uint32_t>(src_fmt);
    const auto& src_type_name = getImgFmtName(src_fmt);
    for (auto i = 0U; i < getValueOf(ImageFormat::END); ++i) {
        if (0 != (dst_type & (1 << i))) {
            auto img_fmt = static_cast<ImageFormat>(i);
            Image dst;
            dst.create(src.rows(), src.cols(), img_fmt);
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(img_fmt), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst, src_idx, i]() {
                                     g_cvt_func[src_idx][i].m_func_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst, src_idx, i]() {
                                    g_cvt_func[src_idx][i].m_func(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
}

void fromYuyv(const Image& src, uint32_t dst_type, uint32_t loop)
{
    LOGD("From Yuyv");
    const auto src_fmt        = ImageFormat::YUYV;
    auto src_idx       = static_cast<uint32_t>(src_fmt);
    const auto& src_type_name = getImgFmtName(src_fmt);
    for (auto i = 0U; i < getValueOf(ImageFormat::END); ++i) {
        if (0 != (dst_type & (1 << i))) {
            auto img_fmt = static_cast<ImageFormat>(i);
            Image dst;
            dst.create(src.rows(), src.cols(), img_fmt);
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(img_fmt), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst, src_idx, i]() {
                                     g_cvt_func[src_idx][i].m_func_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst, src_idx, i]() {
                                    g_cvt_func[src_idx][i].m_func(src, dst);  // NOLINT
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
    auto src_idx       = static_cast<uint32_t>(src_fmt);
    const auto& src_type_name = getImgFmtName(src_fmt);
    for (auto i = 0U; i < getValueOf(ImageFormat::END); ++i) {
        if (0 != (dst_type & (1 << i))) {
            auto img_fmt = static_cast<ImageFormat>(i);
            Image dst;
            dst.create(src.rows(), src.cols(), img_fmt);
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(img_fmt), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst, src_idx, i]() {
                                     g_cvt_func[src_idx][i].m_func_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst, src_idx, i]() {
                                    g_cvt_func[src_idx][i].m_func(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
}
void fromI420(const Image& src, uint32_t dst_type, uint32_t loop) {
    const auto src_fmt        = ImageFormat::I420;
    LOGD("From {}", getImgFmtName(src_fmt));
    auto src_idx       = static_cast<uint32_t>(src_fmt);
    const auto& src_type_name = getImgFmtName(src_fmt);
    for (auto i = 0U; i < getValueOf(ImageFormat::END); ++i) {
        if (0 != (dst_type & (1 << i))) {
            auto img_fmt = static_cast<ImageFormat>(i);
            Image dst;
            dst.create(src.rows(), src.cols(), img_fmt);
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(img_fmt), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst, src_idx, i]() {
                                     g_cvt_func[src_idx][i].m_func_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst, src_idx, i]() {
                                    g_cvt_func[src_idx][i].m_func(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
}
void fromNv12(const Image& src, uint32_t dst_type, uint32_t loop) {
    const auto src_fmt        = ImageFormat::NV12;
    LOGD("From {}", getImgFmtName(src_fmt));
    auto src_idx       = static_cast<uint32_t>(src_fmt);
    const auto& src_type_name = getImgFmtName(src_fmt);
    for (auto i = 0U; i < getValueOf(ImageFormat::END); ++i) {
        if (0 != (dst_type & (1 << i))) {
            auto img_fmt = static_cast<ImageFormat>(i);
            Image dst;
            dst.create(src.rows(), src.cols(), img_fmt);
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(img_fmt), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst, src_idx, i]() {
                                     g_cvt_func[src_idx][i].m_func_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst, src_idx, i]() {
                                    g_cvt_func[src_idx][i].m_func(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
}
void fromNv21(const Image& src, uint32_t dst_type, uint32_t loop) {
    const auto src_fmt        = ImageFormat::NV21;
    LOGD("From {}", getImgFmtName(src_fmt));
    auto src_idx       = static_cast<uint32_t>(src_fmt);
    const auto& src_type_name = getImgFmtName(src_fmt);
    for (auto i = 0U; i < getValueOf(ImageFormat::END); ++i) {
        if (0 != (dst_type & (1 << i))) {
            auto img_fmt = static_cast<ImageFormat>(i);
            Image dst;
            dst.create(src.rows(), src.cols(), img_fmt);
            auto name  = format2str(k_cvt_name_fmt, src_type_name, getImgFmtName(img_fmt), src.cols(), src.rows());
            auto cost0 = doTest({name, [&src, &dst, src_idx, i]() {
                                     g_cvt_func[src_idx][i].m_func_c(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            name       = format2str(k_asm_name_fmt, name);
            auto cost1 = doTest({name, [&src, &dst, src_idx, i]() {
                                    g_cvt_func[src_idx][i].m_func(src, dst);  // NOLINT
                                 },
                                 dst.data(), dst.size()},
                                loop);
            LOGD("{} speed up {} times\n", name, cost0 / cost1);
        }
    }
}

NAMESPACE_END
