#pragma once

#include <functional>

#include "types.hpp"

NAMESPACE_BEGIN

struct TestBody
{
    TestBody(std::string_view name, std::function<void()> func, uint8_t* buff, size_t size)
        : m_name{name}
        , m_func{std::move(func)}
        , m_buff{buff}
        , m_size{size} {}
    std::string_view m_name;
    std::function<void()> m_func;
    uint8_t* m_buff;
    size_t m_size;
};

double doTest(const TestBody&, uint32_t);
void readSrcFile(std::string, uint8_t*, size_t);
void writeDstFile(std::string_view, const uint8_t*, size_t);
int cvtTest(int, char **);
int resizeTest(int, char **);

NAMESPACE_END
