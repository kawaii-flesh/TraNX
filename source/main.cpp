#define TESLA_INIT_IMPL
#include "front/tranxOverlay.hpp"
#include <tesla.hpp>

int main(int argc, char **argv) {
  return tsl::loop<front::TraNXOverlay>(argc, argv);
}