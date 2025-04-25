#include "tranxGUI.hpp"

namespace front {

tsl::elm::Element *TraNXGUI::createUI() { return new TraNXElement(); }

void TraNXGUI::update() {}

bool TraNXGUI::handleInput(u64 keysDown, u64 keysHeld,
                           const HidTouchState &touchPos,
                           HidAnalogStickState joyStickPosLeft,
                           HidAnalogStickState joyStickPosRight) {
  return app::Manager::getInstance().processingInput(
      keysDown, keysHeld, touchPos, joyStickPosLeft, joyStickPosRight);
}

} // namespace front