#pragma once
#include "../core/text.hpp"
#include "../utils/http.hpp"
#include "../utils/utils.hpp"
#include "config.hpp"
#include "frame.hpp"
#include <cstdio>
#include <list>
#include <nlohmann/json.hpp>
#include <switch.h>
#include <tesla.hpp>

namespace app {

enum class State { RUNNING, COMBO_PROCESSING, UPLOAD };

class Manager {
private:
  core::Text text;
  Config &config = Config::getInstance();

  int backComboKeys = 0;
  int changeModeComboKeys = 0;
  int uploadComboKeys = 0;
  int translationFrameComboKeys = 0;
  int outputFrameComboKeys = 0;
  int cleanScreenComboKeys = 0;

  Manager() = default;

  bool tracking = false;
  bool trackingOutputFrame = false;
  Frame translationFrame;
  bool useOutputFrame = false;
  Frame outputFrame;
  State state = State::RUNNING;
  utils::HttpRequester httpRequester;
  bool initialized = false;

public:
  Manager(const Manager &) = delete;
  Manager &operator=(const Manager &) = delete;

  static Manager &getInstance();

  void init();

  [[nodiscard]] const core::Text &getText() const noexcept;
  void updateText(const std::string &jsonResponse);

  bool processingInput(u64 keysDown, u64 keysHeld,
                       const HidTouchState &touchPos,
                       HidAnalogStickState joyStickPosLeft,
                       HidAnalogStickState joyStickPosRight);
  [[nodiscard]] State getState() noexcept;
  [[nodiscard]] const Frame &getTranslationFrame() noexcept;
  [[nodiscard]] const Frame &getOutputFrame() noexcept;
  [[nodiscard]] bool getTrackingOutputFrame() noexcept;
  [[nodiscard]] bool getTracking() noexcept;
  [[nodiscard]] bool getUseOutputFrame() noexcept;

private:
  void handleModeChange();
  void handleUpload();
  bool handleFrameTracking(u64 keysHeld, const HidTouchScreenState &touchState);
  bool startFrameTracking(u64 keysHeld, const HidTouchScreenState &touchState,
                          Frame *&currentFrame);
  bool stopFrameTracking(Frame *currentFrame);
};

} // namespace app