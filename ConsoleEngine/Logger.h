#pragma once
#include <format>
#include <iostream>

#ifdef _DEBUG
#define LogDebug(format, ...) \
    Logger::Log("{}:{} - " format "\n", __FILE__, __LINE__, __VA_ARGS__)
#else
#define LogDebug(format, ...) void(0)
#endif

class Logger {
    static std::ostream* out;

   public:
    static void Init(std::ostream* out);

    template <typename... Args>
    static void Log(const std::format_string<Args...> fmt, Args&&... args)
    {
        *out << std::vformat(fmt.get(), std::make_format_args(args...));
    }
};
