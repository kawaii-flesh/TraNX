#include "text.hpp"

namespace core {

Text::Text(std::string aText, int aX, int aY, int aWidth, int aHeight) noexcept
    : text(std::move(aText)), x(aX), y(aY), width(aWidth), height(aHeight) {}

[[nodiscard]] Text Text::fromJson(const nlohmann::json &json) {
  return Text(json.at("text").get<std::string>(), json.at("x").get<int>(),
              json.at("y").get<int>(), json.at("width").get<int>(),
              json.at("height").get<int>());
}

[[nodiscard]] nlohmann::json Text::toJson() const {
  return {
      {"text", text}, {"x", x}, {"y", y}, {"width", width}, {"height", height}};
}

[[nodiscard]] std::string Text::toJsonString() const { return toJson().dump(); }

} // namespace core