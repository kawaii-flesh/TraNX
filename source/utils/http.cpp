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

HttpRequester::HttpRequester() : curl(curl_easy_init()) {
  if (!curl) {
    throw HttpException("Failed to initialize CURL");
  }
}

HttpRequester::~HttpRequester() { curl_easy_cleanup(curl); }

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

  if (!curl) {
    throw HttpException("CURL not initialized");
  }

  std::string response;

  u64 PID, programID;
  pmdmntGetApplicationProcessId(&PID);
  pmdmntGetProgramId(&programID, PID);
  if (programID < 0x0100000000000000) {
    programID = 0;
  }

  curl_mime *mime = curl_mime_init(curl);
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

  curl_easy_reset(curl);
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "tranx");
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
  curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
  curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 1024L);
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);

  if (const auto res = curl_easy_perform(curl); res != CURLE_OK) {
    curl_mime_free(mime);
    throw HttpException(curl_easy_strerror(res));
  }

  curl_mime_free(mime);
  return response;
}

} // namespace utils