#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/Utils/StringUtils.h"

#include <Windows.h>

namespace StringUtils
{
	std::string ToUTF8(const std::wstring& wide)
	{
		if (wide.empty())
			return {};

        // 필요한 UTF-8 버퍼 크기 계산
        int sizeNeeded = ::WideCharToMultiByte(
            CP_UTF8,
            0,
            wide.data(),
            static_cast<int>(wide.size()),
            nullptr,
            0,
            nullptr,
            nullptr
        );

        if (sizeNeeded <= 0)
            return {};

        std::string utf8(sizeNeeded, 0);

        ::WideCharToMultiByte(
            CP_UTF8,
            0,
            wide.data(),
            static_cast<int>(wide.size()),
            utf8.data(),
            sizeNeeded,
            nullptr,
            nullptr
        );

        return utf8;
	}

    std::wstring ToWide(const std::string& utf8)
    {
        if (utf8.empty())
            return {};

        // 필요한 UTF-16 버퍼 크기 계산
        int sizeNeeded = ::MultiByteToWideChar(
            CP_UTF8,
            0,
            utf8.data(),
            static_cast<int>(utf8.size()),
            nullptr,
            0
        );

        if (sizeNeeded <= 0)
            return {};

        std::wstring wide(sizeNeeded, 0);

        ::MultiByteToWideChar(
            CP_UTF8,
            0,
            utf8.data(),
            static_cast<int>(utf8.size()),
            wide.data(),
            sizeNeeded
        );

        return wide;
    }

    std::string ToString(int32 value)
    {
        return std::to_string(value);
    }

    std::string ToString(uint32 value)
    {
        return std::to_string(value);
    }

    std::string ToString(int64 value)
    {
        return std::to_string(value);
    }

    std::string ToString(uint64 value)
    {
        return std::to_string(value);
    }
}