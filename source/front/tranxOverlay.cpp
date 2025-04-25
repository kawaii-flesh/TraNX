#include "tranxOverlay.hpp"
#include "tranxGUI.hpp"
#include <tesla.hpp>

namespace front {

void TraNXOverlay::initServices() { app::Manager::getInstance().init(); }

void TraNXOverlay::onShow() {}

void TraNXOverlay::onHide() {}

std::unique_ptr<tsl::Gui> TraNXOverlay::loadInitialGui() {
  return initially<TraNXGUI>();
}

} // namespace front