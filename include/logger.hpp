#pragma once

#include <types.hpp>
#include <iostream>
#include <format>

NAMESPACE_BEGIN

template <class... Args>
std::string format2str(std::format_string<Args...> fmt, Args &&...args)
{
    return std::format(fmt, std::forward<Args>(args)...);
}

template <class... Args>
void LOGD(std::format_string<Args...> fmt, Args &&...args)
{
    std::cout << std::format(fmt, std::forward<Args>(args)...) << std::endl;
}

template <class... Args>
void LOGI(std::format_string<Args...> fmt, Args &&...args)
{
    LOGD(fmt, std::forward<Args>(args)...);
}

template <class... Args>
void LOGW(std::format_string<Args...> fmt, Args &&...args)
{
    std::cerr << std::format(fmt, std::forward<Args>(args)...) << std::endl;
}

template <class... Args>
void LOGE(std::format_string<Args...> fmt, Args &&...args)
{
    LOGW(fmt, std::forward<Args>(args)...);
}

NAMESPACE_END
