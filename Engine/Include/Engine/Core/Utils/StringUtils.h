#pragma once

#include <string>

namespace StringUtils
{
    // UTF-16 (wstring) ¡æ UTF-8 (string)
    std::string ToUTF8(const std::wstring& wide);

    // UTF-8 (string) ¡æ UTF-16 (wstring)
    std::wstring ToWide(const std::string& utf8);

    // Integer -> string (explicit overloads)
    std::string ToString(int32 value);
    std::string ToString(uint32 value);
    std::string ToString(int64 value);
    std::string ToString(uint64 value);

    // Generic integer -> string (template)
    template<typename T>
    std::string ToString(T value)
    {
        static_assert(std::is_integral_v<T>,
            "StringUtils::ToString<T> requires an integral type");

        return std::to_string(value);
    }
}