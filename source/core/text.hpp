#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <utility>

namespace core {

class Text {
public:
  std::string text;
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;

  Text() = default;

  Text(std::string aText, int aX, int aY, int aWidth, int aHeight) noexcept;

  ~Text() = default;

  [[nodiscard]] static Text fromJson(const nlohmann::json &json);
  [[nodiscard]] nlohmann::json toJson() const;
  [[nodiscard]] std::string toJsonString() const;
};

} // namespace core