#include "manager.hpp"

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
    auto json = nlohmann::json::parse(jsonResponse)["text"];
    text =
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

  if (utils::checkCombo(keysDown, changeModeComboKeys)) {
    handleModeChange();
    return true;
  }

  if (state != State::COMBO_PROCESSING) {
    return false;
  }

  if (utils::checkCombo(keysDown, backComboKeys)) {
    tsl::goBack();
    return true;
  }

  if (utils::checkCombo(keysDown, uploadComboKeys)) {
    handleUpload();
    return true;
  }

  if (utils::checkCombo(keysDown, cleanScreenComboKeys)) {
    text.text.clear();
    return true;
  }

  return handleFrameTracking(keysHeld, touchState);
}

void Manager::handleModeChange() {
  bool isComboProcessing = state == State::COMBO_PROCESSING;
  state = isComboProcessing ? State::RUNNING : State::COMBO_PROCESSING;
  if (isComboProcessing) {
    text.text.clear();
  }
  tsl::hlp::requestForeground(!isComboProcessing);
}

void Manager::handleUpload() {
  tsl::Overlay::get()->clearScreen();
  auto screenshot = utils::captureScreenshotToBuffer();
  state = State::UPLOAD;
  try {
    updateText(httpRequester.sendScreenshotAndFrames(
        config.getUploadURL(), screenshot, translationFrame, outputFrame,
        useOutputFrame));
  } catch (...) {
  }
  state = State::COMBO_PROCESSING;
}

bool Manager::handleFrameTracking(u64 keysHeld,
                                  const HidTouchScreenState &touchState) {
  static Frame *currentFrame = nullptr;

  if (!tracking && (utils::checkCombo(keysHeld, translationFrameComboKeys) ||
                    utils::checkCombo(keysHeld, outputFrameComboKeys))) {
    return startFrameTracking(keysHeld, touchState, currentFrame);
  }

  if (tracking && touchState.count > 0) {
    currentFrame->endX = touchState.touches[0].x;
    currentFrame->endY = touchState.touches[0].y;
    return true;
  }

  if (tracking && !utils::checkCombo(keysHeld, translationFrameComboKeys) &&
      !(utils::checkCombo(keysHeld, outputFrameComboKeys) &&
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

  if (utils::checkCombo(keysHeld, translationFrameComboKeys)) {
    currentFrame = &translationFrame;
  } else if (utils::checkCombo(keysHeld, outputFrameComboKeys)) {
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

State Manager::getState() noexcept { return state; }
const Frame &Manager::getTranslationFrame() noexcept {
  return translationFrame;
}
const Frame &Manager::getOutputFrame() noexcept { return outputFrame; };
bool Manager::getTrackingOutputFrame() noexcept { return trackingOutputFrame; }
bool Manager::getTracking() noexcept { return tracking; }
bool Manager::getUseOutputFrame() noexcept { return useOutputFrame; }

} // namespace app