#include "config.hpp"
#include "../utils/utils.hpp"
#include <tesla.hpp>

namespace app {

Config::Config() noexcept
    : uploadURL("http://127.0.0.1:1785/upload"), changeModeCombo("LS"),
      backCombo("DLEFT"), uploadCombo("RS"), translationFrameCombo("DRIGHT"),
      outputFrameCombo("DUP"), cleanScreenCombo("DDOWN"),
      backgroundColor("050505"), fontColor("FFFFFF") {
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
      if (section.contains("uploadURL"))
        setUploadURL(section.at("uploadURL"));
      if (section.contains("changeModeCombo"))
        setChangeModeCombo(section.at("changeModeCombo"));
      if (section.contains("backCombo"))
        setBackCombo(section.at("backCombo"));
      if (section.contains("uploadCombo"))
        setUploadCombo(section.at("uploadCombo"));
      if (section.contains("translationFrameCombo"))
        setTranslationFrameCombo(section.at("translationFrameCombo"));
      if (section.contains("outputFrameCombo"))
        setOutputFrameCombo(section.at("outputFrameCombo"));
      if (section.contains("cleanScreenCombo"))
        setCleanScreenCombo(section.at("cleanScreenCombo"));
      if (section.contains("backgroundColor"))
        setBackgroundColor(section.at("backgroundColor"));
      if (section.contains("fontColor"))
        setFontColor(section.at("fontColor"));
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