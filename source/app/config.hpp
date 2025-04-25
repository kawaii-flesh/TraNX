#pragma once

#include <cstdio>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <utility>

namespace app {

class Config {
private:
  Config() noexcept;
  Config(const Config &) = delete;
  Config(Config &&) = delete;
  Config &operator=(const Config &) = delete;
  Config &operator=(Config &&) = delete;

public:
  [[nodiscard]] static Config &getInstance() noexcept;

  [[nodiscard]] const std::string &getUploadURL() const noexcept;
  [[nodiscard]] const std::string &getChangeModeCombo() const noexcept;
  [[nodiscard]] const std::string &getBackCombo() const noexcept;
  [[nodiscard]] const std::string &getUploadCombo() const noexcept;
  [[nodiscard]] const std::string &getTranslationFrameCombo() const noexcept;
  [[nodiscard]] const std::string &getOutputFrameCombo() const noexcept;
  [[nodiscard]] const std::string &getCleanScreenCombo() const noexcept;
  [[nodiscard]] const std::string &getBackgroundColor() const noexcept;
  [[nodiscard]] const std::string &getFontColor() const noexcept;

  void setUploadURL(std::string newUrl) noexcept;
  void setChangeModeCombo(std::string combo) noexcept;
  void setBackCombo(std::string combo) noexcept;
  void setUploadCombo(std::string combo) noexcept;
  void setTranslationFrameCombo(std::string combo) noexcept;
  void setOutputFrameCombo(std::string combo) noexcept;
  void setCleanScreenCombo(std::string combo) noexcept;
  void setBackgroundColor(std::string color) noexcept;
  void setFontColor(std::string color) noexcept;

  void load() noexcept;
  [[nodiscard]] bool write() const noexcept;

  [[nodiscard]] const std::filesystem::path &getConfigDirPath() const noexcept;
  [[nodiscard]] const std::filesystem::path &getConfigFilePath() const noexcept;

private:
  const std::filesystem::path configDirPath{"/config/TraNX"};
  const std::filesystem::path configFilePath{configDirPath / "config.ini"};

  std::string uploadURL;
  std::string changeModeCombo;
  std::string backCombo;
  std::string uploadCombo;
  std::string translationFrameCombo;
  std::string outputFrameCombo;
  std::string cleanScreenCombo;
  std::string backgroundColor;
  std::string fontColor;
};

} // namespace app