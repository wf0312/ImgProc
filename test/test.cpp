#include "types.hpp"
#include <algorithm>
#include <chrono>
#include "image.hpp"
#include "test.hpp"
#include "cvt_test.hpp"
#include "resize.hpp"
#include "logger.hpp"

NAMESPACE_BEGIN

void readSrcFile(std::string file_name, uint8_t* buff, size_t size)
{
    auto f = std::fopen(file_name.c_str(), "rb");
    std::fread(buff, 1, size, f);
    std::fclose(f);
    LOGD("Read {} bytes from {}", size, file_name);
}

void writeDstFile(std::string_view name, const uint8_t* data, size_t size)
{
    auto file_name = format2str("bins/{}.bin", name);
    auto f         = std::fopen(file_name.c_str(), "wb");
    std::fwrite(data, 1, size, f);
    std::fclose(f);
    LOGD("Write {} bytes to {}", size, file_name);
}

double doTest(const TestBody& test_body, uint32_t loop)
{
    std::fill_n(test_body.m_buff, test_body.m_size, 0);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < loop; i++) {
        test_body.m_func();
    }
    auto end  = std::chrono::high_resolution_clock::now();
    auto cost = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
    LOGD("Run {} {} times cost {} us", test_body.m_name, loop, cost);
    writeDstFile(test_body.m_name, test_body.m_buff, test_body.m_size);
    return cost;
}

static int32_t getInt(const std::string& str)
{
    LOGD("{}", str);
    int32_t ret = 0;
    for (auto each : str) {
        ret = ret * 10 + each - '0';
    }
    return ret;
}

int resizeTest(int argc, char **argv) {
    /*
    auto bin = std::string(argv[0]);
    auto color_pos = bin.find('_');
    if (std::string::npos == color_pos) {
        LOGE("bin file must like xxx_xxx_xxx.bin");
        return -1;
    }
    auto size_pos = bin.find('_', color_pos + 1);
    if (std::string::npos == size_pos) {
        LOGE("bin file must like xxx_xxx_xxx.bin");
        return -1;
    }
    auto point_pos = bin.find('.', size_pos + 1);
    if (std::string::npos == point_pos) {
        LOGE("bin file must like xxx_xxx_xxx.bin");
        return -1;
    }
    auto color = bin.substr(0, color_pos);
    auto w     = getInt(bin.substr(color_pos + 1, size_pos - color_pos - 1));
    auto h     = getInt(bin.substr(size_pos + 1, point_pos - size_pos - 1));
    LOGD("{} {}x{}", color, w, h);
    auto src_type = getImgFmt(color);
    if (src_type >= ImageFormat::END) {
        LOGE("Unsupported src type {}", getValueOf(src_type));
        return -1;
    }

    Image src{h, w, src_type};
    readSrcFile(argv[0], src.data(), src.size());
    auto scale_w = std::stof(argv[1]);
    auto scale_h = std::stof(argv[2]);
    auto dst_w = static_cast<int32_t>(scale_w * w);
    auto dst_h = static_cast<int32_t>(scale_h * h);
    Image dst{dst_h, dst_w, src_type};

    LOGD("From {}x{} --> {}x{}", w, h, dst_w, dst_h);

    int32_t loop = argc > 3 ? std::stoi(argv[2]) : 1;

    resize(src, dst);
    writeDstFile(format2str("gray_rgb_{}_{}", dst_w, dst_h), dst.data(), dst.size());
*/
    return 0;
}

int cvtTest(int argc, char **argv) {
    Image src;
    auto bin = std::string(argv[0]);
    auto color_pos = bin.find('_');
    if (std::string::npos == color_pos) {
        LOGE("bin file must like xxx_xxx_xxx.bin");
        return -1;
    }
    auto size_pos = bin.find('_', color_pos + 1);
    if (std::string::npos == size_pos) {
        LOGE("bin file must like xxx_xxx_xxx.bin");
        return -1;
    }
    auto point_pos = bin.find('.', size_pos + 1);
    if (std::string::npos == point_pos) {
        LOGE("bin file must like xxx_xxx_xxx.bin");
        return -1;
    }
    auto color = bin.substr(0, color_pos);
    auto w     = getInt(bin.substr(color_pos + 1, size_pos - color_pos - 1));
    auto h     = getInt(bin.substr(size_pos + 1, point_pos - size_pos - 1));
    LOGD("{} {}x{}", color, w, h);
    auto src_type = getImgFmt(color);
    if (src_type >= ImageFormat::END) {
        LOGE("Unsupported src type {}", getValueOf(src_type));
        return -1;
    }

    src.create(h, w, src_type);
    readSrcFile(argv[0], src.data(), src.size());
    uint32_t dst_type = std::stoi(argv[1]);
    if (0 == dst_type) {
        for (auto i = 0; i < static_cast<uint8_t>(ImageFormat::END); ++i) {
            dst_type |= (1 << i);
        }
    }
    LOGD("Dst type is {:b}", dst_type);

    uint32_t loop = std::stoi(argv[2]);

    switch (src_type) {
    case ImageFormat::GRAY:
        break;
    case ImageFormat::RGB:
        fromRgb(src, dst_type, loop);
        break;
    case ImageFormat::RGBA:
        fromRgba(src, dst_type, loop);
        break;
    case ImageFormat::BGR:
        fromBgr(src, dst_type, loop);
        break;
    case ImageFormat::BGRA:
        fromBgra(src, dst_type, loop);
        break;
    case ImageFormat::YUYV:
        fromYuyv(src, dst_type, loop);
        break;
    case ImageFormat::UYVY:
        fromUyvy(src, dst_type, loop);
        break;
    case ImageFormat::I420:
        fromI420(src, dst_type, loop);
        break;
    case ImageFormat::NV12:
        fromNv12(src, dst_type, loop);
        break;
    case ImageFormat::NV21:
        fromNv21(src, dst_type, loop);
        break;
    default:
        break;
    }

    return 0;
}

NAMESPACE_END
