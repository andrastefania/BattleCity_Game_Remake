#include "Validation.h"

bool Validation::IsValidPosition(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    return x < width && y < height;
}
