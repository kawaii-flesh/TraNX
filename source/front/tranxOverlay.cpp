#include "tranxOverlay.hpp"
#include "tranxGUI.hpp"
#include <tesla.hpp>

namespace front {

void TraNXOverlay::initServices() {
  nifmInitialize(NifmServiceType_User);
  curl_global_init(CURL_GLOBAL_DEFAULT);
  app::Manager::getInstance().init();
}

void TraNXOverlay::exitServices() {
  curl_global_cleanup();
  nifmExit();
}

void TraNXOverlay::onShow() {}

void TraNXOverlay::onHide() {}

std::unique_ptr<tsl::Gui> TraNXOverlay::loadInitialGui() {
  return initially<TraNXGUI>();
}

} // namespace front