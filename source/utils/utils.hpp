#pragma once

#include <memory>
#include <string_view>
#include <switch.h>
#include <tesla.hpp>
#include <vector>

namespace utils {

[[nodiscard]] bool checkCombo(const u64 keysDown, const u64 combo) noexcept;

[[nodiscard]] std::vector<unsigned char> captureScreenshotToBuffer();

[[nodiscard]] bool fsPathExists(const char *path) noexcept;

[[nodiscard]] tsl::Color hexToColor4444(std::string_view hexStr) noexcept;

[[nodiscard]] u64 comboStringToKeysOrDefault(const std::string &value, u64 defaultKeyCombo) noexcept;

void logTNX(const char* format, ...);

} // namespace utils