#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <vector>
#include <switch.h>

namespace core {

class Text {
public:
  std::vector<u8> imageData;
  int x = 0;
  int y = 0;

  Text() = default;

  Text(std::vector<u8> aImageData, int aX, int aY) noexcept;

  ~Text() = default;
};

} // namespace core