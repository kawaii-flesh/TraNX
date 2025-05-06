#include "utils.hpp"
#include "http.hpp"
#include <cstdarg>
#include <cstdio>

namespace utils {

bool checkCombo(const u64 keysDown, const u64 combo) noexcept {
  return (keysDown & combo) == combo;
}

std::vector<unsigned char> captureScreenshotToBuffer() {
  std::vector<unsigned char> jpegBuffer;

  if (R_FAILED(capsscInitialize())) {
    logTNX("Failed to initialize caps:sc\n");
    return jpegBuffer;
  }
  tsl::hlp::ScopeGuard scGuard([] { capsscExit(); });

  auto jpegBuf = std::make_unique<unsigned char[]>(CAPSSC_JPEG_BUFFER_SIZE);
  u64 outSize = 0;

  Result rc = capsscCaptureJpegScreenShot(&outSize, jpegBuf.get(),
                                          CAPSSC_JPEG_BUFFER_SIZE,
                                          ViLayerStack_Screenshot, 100000000);

  if (R_SUCCEEDED(rc)) {
    jpegBuffer.assign(jpegBuf.get(), jpegBuf.get() + outSize);
  } else {
    logTNX("Failed to capture screenshot: 0x%X\n", rc);
  }

  return jpegBuffer;
}

bool fsPathExists(const char *path) noexcept {
  if (!path)
    return false;

  FsFileSystem fsSdmc;
  if (R_FAILED(fsOpenSdCardFileSystem(&fsSdmc))) {
    return false;
  }
  tsl::hlp::ScopeGuard fsGuard([&fsSdmc] { fsFsClose(&fsSdmc); });

  FsDirEntryType type;
  return R_SUCCEEDED(fsFsGetEntryType(&fsSdmc, path, &type)) &&
         (type == FsDirEntryType_Dir || type == FsDirEntryType_File);
}

tsl::Color hexToColor4444(std::string_view hexStr) noexcept {
  constexpr auto hexCharTo4bit = [](char c) noexcept -> u8 {
    if (c >= '0' && c <= '9')
      return c - '0';
    if (c >= 'A' && c <= 'F')
      return 10 + (c - 'A');
    if (c >= 'a' && c <= 'f')
      return 10 + (c - 'a');
    return 0;
  };

  if (hexStr.size() != 6)
    return {0, 0, 0, 0xFF};

  return {static_cast<u8>((hexCharTo4bit(hexStr[0]) << 4) |
                          hexCharTo4bit(hexStr[1])),
          static_cast<u8>((hexCharTo4bit(hexStr[2]) << 4) |
                          hexCharTo4bit(hexStr[3])),
          static_cast<u8>((hexCharTo4bit(hexStr[4]) << 4) |
                          hexCharTo4bit(hexStr[5])),
          0xFF};
}

[[nodiscard]] std::pair<u64, bool>
comboStringToKeysOrDefault(const std::string &value,
                           u64 defaultKeyCombo) noexcept {
  bool inAllModes = false;
  std::string_view view(value);

  if (!view.empty() && view[0] == '!') {
    inAllModes = true;
    view.remove_prefix(1);
  }

  u64 keyCombo = tsl::hlp::comboStringToKeys(std::string(view));
  return {keyCombo != 0x00 ? keyCombo : defaultKeyCombo, inAllModes};
}

u64 getPID() noexcept {
  u64 PID, programID;
  pmdmntGetApplicationProcessId(&PID);
  pmdmntGetProgramId(&programID, PID);
  return PID;
}

void logTNX(const char *format, ...) {
#ifdef DEBUG
  va_list args;
  va_start(args, format);
  printf("[TraNX] ");
  vprintf(format, args);
  printf("\n");
  va_end(args);
#endif
}

} // namespace utils