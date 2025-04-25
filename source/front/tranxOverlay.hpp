#pragma once

#include "tranxGUI.hpp"
#include <tesla.hpp>

namespace front {

class TraNXOverlay final : public tsl::Overlay {
public:
  void initServices() override;
  void onShow() override;
  void onHide() override;
  std::unique_ptr<tsl::Gui> loadInitialGui() override;
};

} // namespace front