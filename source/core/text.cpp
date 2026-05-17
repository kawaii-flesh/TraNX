#include "text.hpp"

namespace core {

Text::Text(std::vector<u8> aImageData, int aX, int aY) noexcept
    : imageData(std::move(aImageData)), x(aX), y(aY) {}

} // namespace core