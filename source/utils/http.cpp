#include "http.hpp"
#include <memory>
#include <nlohmann/json.hpp>
#include <switch.h>
#include <utility>

namespace {

struct CurlHeaders {
  curl_slist *list = nullptr;

  CurlHeaders() = default;
  ~CurlHeaders() {
    if (list)
      curl_slist_free_all(list);
  }

  void append(const char *header) { list = curl_slist_append(list, header); }
};

} // namespace

namespace utils {

HttpRequester::HttpRequester() : curl_(curl_easy_init(), &curl_easy_cleanup) {
  if (!curl_) {
    throw HttpException("Failed to initialize CURL");
  }
  curl_global_init(CURL_GLOBAL_DEFAULT);
}

size_t HttpRequester::writeCallback(void *contents, size_t size, size_t nmemb,
                                    std::string *data) {
  const auto readSize = size * nmemb;
  data->append(static_cast<const char *>(contents), readSize);
  return readSize;
}

std::string HttpRequester::sendScreenshotAndFrames(
    const std::string &url, const std::vector<unsigned char> &imageData,
    const app::Frame &translationFrame, const app::Frame &outputFrame,
    bool useOutputFrame) const {

  if (!curl_) {
    throw HttpException("CURL not initialized");
  }

  std::string response;

  u64 PID, programID;
  pmdmntGetApplicationProcessId(&PID);
  pmdmntGetProgramId(&programID, PID);

  curl_mime *mime = curl_mime_init(curl_.get());
  curl_mimepart *part;

  part = curl_mime_addpart(mime);
  curl_mime_name(part, "image");
  curl_mime_filename(part, "screenshot.jpg");
  curl_mime_data(part, reinterpret_cast<const char *>(imageData.data()),
                 imageData.size());
  curl_mime_type(part, "image/jpeg");

  part = curl_mime_addpart(mime);
  curl_mime_name(part, "translationFrame");
  curl_mime_data(part, translationFrame.toJson().dump().c_str(),
                 CURL_ZERO_TERMINATED);

  part = curl_mime_addpart(mime);
  curl_mime_name(part, "outputFrame");
  curl_mime_data(part, outputFrame.toJson().dump().c_str(),
                 CURL_ZERO_TERMINATED);

  part = curl_mime_addpart(mime);
  curl_mime_name(part, "useOutputFrame");
  curl_mime_data(part, useOutputFrame ? "true" : "false", CURL_ZERO_TERMINATED);

  part = curl_mime_addpart(mime);
  curl_mime_name(part, "pid");
  curl_mime_data(part, std::to_string(programID).c_str(), CURL_ZERO_TERMINATED);

  curl_easy_reset(curl_.get());
  curl_easy_setopt(curl_.get(), CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl_.get(), CURLOPT_USERAGENT, "tranx");
  curl_easy_setopt(curl_.get(), CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl_.get(), CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl_.get(), CURLOPT_SSL_VERIFYHOST, 0L);
  curl_easy_setopt(curl_.get(), CURLOPT_WRITEFUNCTION, writeCallback);
  curl_easy_setopt(curl_.get(), CURLOPT_WRITEDATA, &response);
  curl_easy_setopt(curl_.get(), CURLOPT_MIMEPOST, mime);
  curl_easy_setopt(curl_.get(), CURLOPT_BUFFERSIZE, 1024L);
  curl_easy_setopt(curl_.get(), CURLOPT_CONNECTTIMEOUT, 5L);

  if (const auto res = curl_easy_perform(curl_.get()); res != CURLE_OK) {
    curl_mime_free(mime);
    throw HttpException(curl_easy_strerror(res));
  }

  curl_mime_free(mime);
  return response;
}

} // namespace utils