#pragma once

#include "../app/frame.hpp"
#include <cstddef>
#include <curl/curl.h>
#include <exception>
#include <memory>
#include <string>
#include <vector>

namespace utils {

class HttpException : public std::exception {
  std::string message_;

public:
  explicit HttpException(const std::string &msg) : message_(msg) {}
  const char *what() const noexcept override { return message_.c_str(); }
};

class HttpRequester {
  CURL *curl = nullptr;

public:
  struct ScreenshotData {
    const std::vector<unsigned char> *buffer;
    size_t bytesSent;
  };

  HttpRequester();
  ~HttpRequester();

  [[nodiscard]] std::string sendScreenshotAndFrames(
      const std::string &url, const std::vector<unsigned char> &imageData,
      const app::Frame &translationFrame, const app::Frame &outputFrame,
      bool useOutputFrame) const;

  [[nodiscard]] std::string getText(const std::string &url, long timeout) const;

private:
  static size_t writeCallback(void *contents, size_t size, size_t nmemb,
                              std::string *data);
  static std::string base64Encode(const std::vector<unsigned char> &data);
};

} // namespace utils