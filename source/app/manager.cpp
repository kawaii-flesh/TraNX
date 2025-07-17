#include "manager.hpp"
#include "../utils/discover.hpp"

namespace app {

Manager &Manager::getInstance() {
  static Manager instance;
  return instance;
}

void Manager::init() {
  if (!initialized) {
    if (!utils::fsPathExists(config.getConfigFilePath().c_str())) {
      if (!config.write()) {
        utils::logTNX("Failed to write initial config\n");
      }
    } else {
      config.load();
      if (config.getUploadURL() == "*") {
        try {
          std::string discovered = utils::discoverServer(1786, 3000);
          config.setUploadURL("http://" + discovered + "/upload");
          utils::logTNX(
              ("Discovered upload URL: " + config.getUploadURL() + "\n")
                  .c_str());
        } catch (const std::exception &e) {
          utils::logTNX(
              ("Discovery failed: " + std::string(e.what()) + "\n").c_str());
          tsl::goBack();
        }
      }
    }

    backComboKeys = utils::comboStringToKeysOrDefault(config.getBackCombo(),
                                                      HidNpadButton_Left);
    changeModeComboKeys = utils::comboStringToKeysOrDefault(
        config.getChangeModeCombo(), HidNpadButton_StickL);
    uploadComboKeys = utils::comboStringToKeysOrDefault(config.getUploadCombo(),
                                                        HidNpadButton_StickR);
    translationFrameComboKeys = utils::comboStringToKeysOrDefault(
        config.getTranslationFrameCombo(), HidNpadButton_Right);
    outputFrameComboKeys = utils::comboStringToKeysOrDefault(
        config.getOutputFrameCombo(), HidNpadButton_Up);
    cleanScreenComboKeys = utils::comboStringToKeysOrDefault(
        config.getCleanScreenCombo(), HidNpadButton_Down);

    initialized = true;
  }
}

const core::Text &Manager::getText() const noexcept { return text; }

void Manager::updateText(const std::string &jsonResponse) {
  try {
    auto json = nlohmann::json::parse(jsonResponse);

    this->useOutputFrame = json.at("use_output_frame").get<bool>();
    this->translationFrame.updateFromJson(json["translation_frame"]);
    this->outputFrame.updateFromJson(json["output_frame"]);
    this->text =
        core::Text(json.at("text").get<std::string>(), json.at("x").get<int>(),
                   json.at("y").get<int>(), json.at("width").get<int>(),
                   json.at("height").get<int>());
  } catch (const std::exception &e) {
    utils::logTNX("Error updating text: %s\n", e.what());
  }
}

bool Manager::processingInput(u64 keysDown, u64 keysHeld,
                              const HidTouchState &touchPos,
                              HidAnalogStickState joyStickPosLeft,
                              HidAnalogStickState joyStickPosRight) {
  HidTouchScreenState touchState{};
  hidGetTouchScreenStates(&touchState, 1);

  if (keysDown & HidNpadButton_B) {
    return true;
  }

  if (checkComboAndState(keysDown, changeModeComboKeys, State::ALL)) {
    handleModeChange();
    return true;
  }

  if (checkComboAndState(keysDown, backComboKeys, State::COMBO_PROCESSING)) {
    tsl::goBack();
    return true;
  }

  if (checkComboAndState(keysDown, uploadComboKeys, State::COMBO_PROCESSING)) {
    handleUpload();
    return true;
  }

  if (checkComboAndState(keysDown, cleanScreenComboKeys,
                         State::COMBO_PROCESSING)) {
    text.text.clear();
    return true;
  }

  return handleFrameTracking(keysHeld, touchState);
}

void Manager::handleModeChange() {
  bool isComboProcessing = currentState == State::COMBO_PROCESSING;
  currentState = isComboProcessing ? State::RUNNING : State::COMBO_PROCESSING;
  if (isComboProcessing) {
    text.text.clear();
  }
  tsl::hlp::requestForeground(!isComboProcessing);
}

void Manager::handleUpload() {
  tsl::Overlay::get()->clearScreen();
  auto screenshot = utils::captureScreenshotToBuffer();
  try {
    updateText(httpRequester.sendScreenshotAndFrames(
        config.getUploadURL(), screenshot, translationFrame, outputFrame,
        useOutputFrame));
  } catch (...) {
  }
}

bool Manager::handleFrameTracking(u64 keysHeld,
                                  const HidTouchScreenState &touchState) {
  static Frame *currentFrame = nullptr;

  if (!tracking && (checkComboAndState(keysHeld, translationFrameComboKeys,
                                       State::COMBO_PROCESSING) ||
                    checkComboAndState(keysHeld, outputFrameComboKeys,
                                       State::COMBO_PROCESSING))) {
    return startFrameTracking(keysHeld, touchState, currentFrame);
  }

  if (tracking && touchState.count > 0) {
    currentFrame->endX = touchState.touches[0].x;
    currentFrame->endY = touchState.touches[0].y;
    return true;
  }

  if (tracking &&
      !checkComboAndState(keysHeld, translationFrameComboKeys,
                          State::COMBO_PROCESSING) &&
      !(checkComboAndState(keysHeld, outputFrameComboKeys,
                           State::COMBO_PROCESSING) &&
        trackingOutputFrame)) {
    return stopFrameTracking(currentFrame);
  }

  return false;
}

bool Manager::startFrameTracking(u64 keysHeld,
                                 const HidTouchScreenState &touchState,
                                 Frame *&currentFrame) {
  if (touchState.count == 0)
    return false;

  if (checkComboAndState(keysHeld, translationFrameComboKeys,
                         State::COMBO_PROCESSING)) {
    currentFrame = &translationFrame;
  } else if (checkComboAndState(keysHeld, outputFrameComboKeys,
                                State::COMBO_PROCESSING)) {
    currentFrame = &outputFrame;
    useOutputFrame = true;
    trackingOutputFrame = true;
  }

  currentFrame->startX = touchState.touches[0].x;
  currentFrame->startY = touchState.touches[0].y;
  currentFrame->endX = currentFrame->startX;
  currentFrame->endY = currentFrame->startY;
  tracking = true;
  text.text.clear();
  return true;
}

bool Manager::stopFrameTracking(Frame *currentFrame) {
  tracking = false;
  trackingOutputFrame = false;
  utils::logTNX(
      "Rectangle: Start(%u, %u), End(%u, %u), Width: %u, Height: %u\n",
      currentFrame->startX, currentFrame->startY, currentFrame->endX,
      currentFrame->endY,
      abs(static_cast<int>(currentFrame->endX) -
          static_cast<int>(currentFrame->startX)),
      abs(static_cast<int>(currentFrame->endY) -
          static_cast<int>(currentFrame->startY)));
  return true;
}

State Manager::getState() noexcept { return currentState; }
const Frame &Manager::getTranslationFrame() noexcept {
  return translationFrame;
}
const Frame &Manager::getOutputFrame() noexcept { return outputFrame; };
bool Manager::getTrackingOutputFrame() noexcept { return trackingOutputFrame; }
bool Manager::getTracking() noexcept { return tracking; }
bool Manager::getUseOutputFrame() noexcept { return useOutputFrame; }

bool Manager::checkComboAndState(const u64 keysDown, std::pair<u64, bool> combo,
                                 const State state) noexcept {
  return utils::checkCombo(keysDown, combo.first) &&
         (state == State::ALL || combo.second || state == currentState);
}

} // namespace app