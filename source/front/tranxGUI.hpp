#pragma once

#include "../app/manager.hpp"
#include "tranxElement.hpp"
#include <tesla.hpp>

namespace front {

class TraNXGUI final : public tsl::Gui {
public:
  TraNXGUI() = default;

  tsl::elm::Element *createUI() override;
  void update() override;
  [[nodiscard]] bool handleInput(u64 keysDown, u64 keysHeld,
                                 const HidTouchState &touchPos,
                                 HidAnalogStickState joyStickPosLeft,
                                 HidAnalogStickState joyStickPosRight) override;
};

} // namespace front