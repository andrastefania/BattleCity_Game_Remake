#pragma once
#ifndef VALIDATion_H
#define VALIDATION_H

#include <cstdint>

// Macro pentru export DLL
#ifdef VALIDATION_DLL_EXPORT
#define VALIDATION_API __declspec(dllexport)
#else
#define VALIDATION_API __declspec(dllimport)
#endif

class VALIDATION_API Validation {
public:
    static bool IsValidPosition(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
};

#endif // VALIDATION_H
