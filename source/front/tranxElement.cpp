#include "tranxElement.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../utils/stb_image.h"

namespace front {

TraNXElement::TraNXElement() : tsl::elm::Element() {}

void TraNXElement::drawRectFrame(tsl::gfx::Renderer *renderer,
                                 const app::Frame &frame, tsl::Color color,
                                 s32 addH) noexcept {
  const s32 x = std::min(frame.startX, frame.endX);
  const s32 y = std::min(frame.startY, frame.endY);
  const s32 w =
      std::abs(static_cast<s32>(frame.endX) - static_cast<s32>(frame.startX));
  const s32 h =
      std::abs(static_cast<s32>(frame.endY) - static_cast<s32>(frame.startY));
  renderer->drawRect(x, y, w, h + addH, color);
}

void TraNXElement::draw(tsl::gfx::Renderer *renderer) {
  auto &manager = app::Manager::getInstance();
  auto &config = app::Config::getInstance();

  const auto backgroundColor =
      utils::hexToColor4444(config.getBackgroundColor());

  renderer->clearScreen();

  app::State state = manager.getState();
  tsl::Color stateColor = green;
  switch (state) {
  case app::State::COMBO_PROCESSING:
    stateColor = yellow;
    break;
  default:
    break;
  }

  stateColor.a = 5;
  renderer->drawRect(0, 0, 10, 10, stateColor);

  auto bgColorWithAlpha = backgroundColor;
  bgColorWithAlpha.a = 6;

  if (manager.getTrackingOutputFrame()) {
    drawRectFrame(renderer, manager.getOutputFrame(), bgColorWithAlpha);
  } else if (manager.getTracking()) {
    drawRectFrame(renderer, manager.getTranslationFrame(), bgColorWithAlpha);
  }

  const core::Text &text = manager.getText();
  if (!text.imageData.empty()) {
    const app::Frame &frame = manager.getUseOutputFrame() ? manager.getOutputFrame() 
                                              : manager.getTranslationFrame();
    
    drawRectFrame(renderer, frame, backgroundColor, 0);

    int img_w, img_h, img_channels;
    unsigned char* pixels = stbi_load_from_memory(
        text.imageData.data(), 
        text.imageData.size(), 
        &img_w, &img_h, &img_channels, 
        4
    );

    if (pixels) {
      renderer->drawBitmap(text.x, text.y, img_w, img_h, pixels);
      stbi_image_free(pixels);
    }
  }
}

void TraNXElement::layout(u16 parentX, u16 parentY, u16 parentWidth,
                          u16 parentHeight) {
  this->setBoundaries(0, 0, tsl::cfg::FramebufferWidth,
                      tsl::cfg::FramebufferHeight);
}

tsl::elm::Element *TraNXElement::requestFocus(tsl::elm::Element *oldFocus,
                                              tsl::FocusDirection direction) {
  return nullptr;
}

} // namespace front