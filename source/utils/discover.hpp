#pragma once

#include <cstdint>
#include <string>

namespace utils {

[[nodiscard]] std::string discoverServer(uint16_t discoveryPort,
                                         int timeoutMs = 3000);

} // namespace utils
