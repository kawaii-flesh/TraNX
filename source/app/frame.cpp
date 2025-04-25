#include "frame.hpp"

namespace app {

constexpr Frame::Frame(uint32_t sx, uint32_t sy, uint32_t ex,
                       uint32_t ey) noexcept
    : startX(sx), startY(sy), endX(ex), endY(ey) {}

constexpr uint32_t Frame::width() const noexcept { return endX - startX; }

constexpr uint32_t Frame::height() const noexcept { return endY - startY; }

nlohmann::json Frame::toJson() const {
  return nlohmann::json{
      {"startX", startX}, {"startY", startY}, {"endX", endX}, {"endY", endY}};
}

Frame Frame::fromJson(const nlohmann::json &json) {
  return Frame(
      json.at("startX").get<uint32_t>(), json.at("startY").get<uint32_t>(),
      json.at("endX").get<uint32_t>(), json.at("endY").get<uint32_t>());
}

std::string Frame::toJsonString() const { return toJson().dump(); }

} // namespace app