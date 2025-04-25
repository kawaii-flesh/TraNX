#pragma once
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

namespace app {
class Frame {
public:
  uint32_t startX = 0;
  uint32_t startY = 0;
  uint32_t endX = 0;
  uint32_t endY = 0;

  constexpr Frame() noexcept = default;
  constexpr Frame(uint32_t sx, uint32_t sy, uint32_t ex, uint32_t ey) noexcept;
  ~Frame() noexcept = default;

  [[nodiscard]] constexpr uint32_t width() const noexcept;
  [[nodiscard]] constexpr uint32_t height() const noexcept;

  [[nodiscard]] nlohmann::json toJson() const;
  [[nodiscard]] static Frame fromJson(const nlohmann::json &json);
  [[nodiscard]] std::string toJsonString() const;
};
} // namespace app