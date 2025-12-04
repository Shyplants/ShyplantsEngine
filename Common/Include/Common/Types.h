#pragma once

// -----------------------------------------
// Common Types for Engine, Client, Server
// -----------------------------------------
//
// - Based on <cstdint> standard fixed-width types
// - Avoids platform-dependent primitive types (int, long)
// - Matches modern engine conventions (UE style: int32, uint32)
// - Safe for binary serialization, networking, and cross-platform use
//
// -----------------------------------------

#include <cstdint>

// ----------------------------------------------------
// Signed integers
// ----------------------------------------------------
using int8  = int8_t;     // 1 byte,  signed
using int16 = int16_t;    // 2 bytes, signed
using int32 = int32_t;    // 4 bytes, signed
using int64 = int64_t;    // 8 bytes, signed

// ----------------------------------------------------
// Unsigned integers
// ----------------------------------------------------
using uint8  = uint8_t;    // 1 byte,  unsigned
using uint16 = uint16_t;   // 2 bytes, unsigned
using uint32 = uint32_t;   // 4 bytes, unsigned
using uint64 = uint64_t;   // 8 bytes, unsigned

// ----------------------------------------------------
// Floating-point types
// ----------------------------------------------------
using float32 = float;     // 4 bytes
using float64 = double;    // 8 bytes

// ----------------------------------------------------
// Boolean type (explicit use, prevents uint misuse)
// ----------------------------------------------------
using bool8 = uint8_t;

// ----------------------------------------------------
// Character types (explicit ASCII/UTF distinction)
// ----------------------------------------------------
using char8  = char;        // UTF-8 / ASCII
using char16 = char16_t;    // UTF-16
using char32 = char32_t;    // UTF-32
// Wide char is platform-dependent, avoid unless needed for WinAPI
using wchar = wchar_t;

// ----------------------------------------------------
// Size/Index types for containers & memory
// ----------------------------------------------------
using size32  = uint32_t;   // explicit 32-bit size
using size64  = uint64_t;   // explicit 64-bit size
using index32 = int32_t;    // array/map index
using index64 = int64_t;

// ----------------------------------------------------
// Engine-specific typedefs (optional)
// ----------------------------------------------------
// These can be used to differentiate time units explicitly.
using TimeMs  = uint64;     // milliseconds
using TimeSec = float64;    // seconds

// 64-bit tick value (replacement for Windows' ULONGLONG)
using Tick64 = uint64;

