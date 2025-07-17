#include "config.hpp"
#include "../utils/utils.hpp"
#include <tesla.hpp>

namespace app {

namespace defaults {
constexpr const char *UPLOAD_URL = "http://127.0.0.1:1785/upload";
constexpr const char *CHANGE_MODE_COMBO = "LS";
constexpr const char *BACK_COMBO = "DLEFT";
constexpr const char *UPLOAD_COMBO = "!RS";
constexpr const char *TRANSLATION_FRAME_COMBO = "DRIGHT";
constexpr const char *OUTPUT_FRAME_COMBO = "DUP";
constexpr const char *CLEAN_SCREEN_COMBO = "DDOWN";
constexpr const char *BACKGROUND_COLOR = "050505";
constexpr const char *FONT_COLOR = "FFFFFF";
} // namespace defaults

Config::Config() noexcept
    : uploadURL(defaults::UPLOAD_URL),
      changeModeCombo(defaults::CHANGE_MODE_COMBO),
      backCombo(defaults::BACK_COMBO), uploadCombo(defaults::UPLOAD_COMBO),
      translationFrameCombo(defaults::TRANSLATION_FRAME_COMBO),
      outputFrameCombo(defaults::OUTPUT_FRAME_COMBO),
      cleanScreenCombo(defaults::CLEAN_SCREEN_COMBO),
      backgroundColor(defaults::BACKGROUND_COLOR),
      fontColor(defaults::FONT_COLOR) {
  try {
    std::filesystem::create_directories(configDirPath);
  } catch (...) {
    utils::logTNX("Warning: Failed to create config directory\n");
  }
}

Config &Config::getInstance() noexcept {
  static Config instance;
  return instance;
}

const std::string &Config::getUploadURL() const noexcept { return uploadURL; }
const std::string &Config::getChangeModeCombo() const noexcept {
  return changeModeCombo;
}
const std::string &Config::getBackCombo() const noexcept { return backCombo; }
const std::string &Config::getUploadCombo() const noexcept {
  return uploadCombo;
}
const std::string &Config::getTranslationFrameCombo() const noexcept {
  return translationFrameCombo;
}
const std::string &Config::getOutputFrameCombo() const noexcept {
  return outputFrameCombo;
}
const std::string &Config::getCleanScreenCombo() const noexcept {
  return cleanScreenCombo;
}
const std::string &Config::getBackgroundColor() const noexcept {
  return backgroundColor;
}
const std::string &Config::getFontColor() const noexcept { return fontColor; }

void Config::setUploadURL(std::string newUrl) noexcept {
  uploadURL = std::move(newUrl);
}
void Config::setChangeModeCombo(std::string combo) noexcept {
  changeModeCombo = std::move(combo);
}
void Config::setBackCombo(std::string combo) noexcept {
  backCombo = std::move(combo);
}
void Config::setUploadCombo(std::string combo) noexcept {
  uploadCombo = std::move(combo);
}
void Config::setTranslationFrameCombo(std::string combo) noexcept {
  translationFrameCombo = std::move(combo);
}
void Config::setOutputFrameCombo(std::string combo) noexcept {
  outputFrameCombo = std::move(combo);
}
void Config::setCleanScreenCombo(std::string combo) noexcept {
  cleanScreenCombo = std::move(combo);
}
void Config::setBackgroundColor(std::string color) noexcept {
  backgroundColor = std::move(color);
}
void Config::setFontColor(std::string color) noexcept {
  fontColor = std::move(color);
}

void Config::load() noexcept {
  try {
    tsl::hlp::ini::IniData iniData =
        tsl::hlp::ini::readSettings(configFilePath.c_str());

    if (iniData.contains("General")) {
      const auto &section = iniData["General"];
      setUploadURL(section.contains("uploadURL") ? section.at("uploadURL")
                                                 : defaults::UPLOAD_URL);
      setChangeModeCombo(section.contains("changeModeCombo")
                             ? section.at("changeModeCombo")
                             : defaults::CHANGE_MODE_COMBO);
      setBackCombo(section.contains("backCombo") ? section.at("backCombo")
                                                 : defaults::BACK_COMBO);
      setUploadCombo(section.contains("uploadCombo") ? section.at("uploadCombo")
                                                     : defaults::UPLOAD_COMBO);
      setTranslationFrameCombo(section.contains("translationFrameCombo")
                                   ? section.at("translationFrameCombo")
                                   : defaults::TRANSLATION_FRAME_COMBO);
      setOutputFrameCombo(section.contains("outputFrameCombo")
                              ? section.at("outputFrameCombo")
                              : defaults::OUTPUT_FRAME_COMBO);
      setCleanScreenCombo(section.contains("cleanScreenCombo")
                              ? section.at("cleanScreenCombo")
                              : defaults::CLEAN_SCREEN_COMBO);
      setBackgroundColor(section.contains("backgroundColor")
                             ? section.at("backgroundColor")
                             : defaults::BACKGROUND_COLOR);
      setFontColor(section.contains("fontColor") ? section.at("fontColor")
                                                 : defaults::FONT_COLOR);
    }
  } catch (const std::exception &e) {
    utils::logTNX("Error loading config: %s\n", e.what());
  }
}

bool Config::write() const noexcept {
  try {
    if (uploadURL.empty()) {
      utils::logTNX("Error: uploadURL is empty!\n");
      return false;
    }

    tsl::hlp::ini::IniData iniData;
    iniData["General"]["uploadURL"] = uploadURL;
    iniData["General"]["changeModeCombo"] = changeModeCombo;
    iniData["General"]["backCombo"] = backCombo;
    iniData["General"]["uploadCombo"] = uploadCombo;
    iniData["General"]["translationFrameCombo"] = translationFrameCombo;
    iniData["General"]["outputFrameCombo"] = outputFrameCombo;
    iniData["General"]["cleanScreenCombo"] = cleanScreenCombo;
    iniData["General"]["backgroundColor"] = backgroundColor;
    iniData["General"]["fontColor"] = fontColor;

    tsl::hlp::ini::writeSettings(configFilePath.c_str(), iniData);
    return true;
  } catch (const std::exception &e) {
    utils::logTNX("Error writing config: %s\n", e.what());
    return false;
  }
}

const std::filesystem::path &Config::getConfigDirPath() const noexcept {
  return configDirPath;
}
const std::filesystem::path &Config::getConfigFilePath() const noexcept {
  return configFilePath;
}

} // namespace app