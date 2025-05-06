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

enum class State { RUNNING, COMBO_PROCESSING, UPLOAD, ALL };

class Manager {
private:
  core::Text text;
  Config &config = Config::getInstance();

  std::pair<u64, bool> backComboKeys{0, false};
  std::pair<u64, bool> changeModeComboKeys{0, false};
  std::pair<u64, bool> uploadComboKeys{0, false};
  std::pair<u64, bool> translationFrameComboKeys{0, false};
  std::pair<u64, bool> outputFrameComboKeys{0, false};
  std::pair<u64, bool> cleanScreenComboKeys{0, false};

  Manager() = default;

  bool tracking = false;
  bool trackingOutputFrame = false;
  Frame translationFrame;
  bool useOutputFrame = false;
  Frame outputFrame;
  State currentState = State::RUNNING;
  utils::HttpRequester httpRequester;
  bool initialized = false;

public:
  Manager(const Manager &) = delete;
  Manager &operator=(const Manager &) = delete;

  static Manager &getInstance();

  void init();

  [[nodiscard]] const core::Text &getText() const noexcept;
  void updateText(const std::string &jsonResponse);

  [[nodiscard]] bool processingInput(u64 keysDown, u64 keysHeld,
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
  [[nodiscard]] bool handleFrameTracking(u64 keysHeld,
                                         const HidTouchScreenState &touchState);
  [[nodiscard]] bool startFrameTracking(u64 keysHeld,
                                        const HidTouchScreenState &touchState,
                                        Frame *&currentFrame);
  [[nodiscard]] bool stopFrameTracking(Frame *currentFrame);
  [[nodiscard]] bool checkComboAndState(const u64 keysDown,
                                        std::pair<u64, bool> combo,
                                        const State state) noexcept;
};

} // namespace app