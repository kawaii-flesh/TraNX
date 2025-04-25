#pragma once
#include "../app/manager.hpp"
#include <algorithm>
#include <tesla.hpp>

namespace front {

constexpr tsl::Color green(0, 255, 0, 255);
constexpr tsl::Color yellow(255, 255, 0, 255);
constexpr tsl::Color red(255, 0, 0, 255);
constexpr tsl::Color cyan(0, 255, 255, 255);

class TraNXElement : public tsl::elm::Element {
public:
  TraNXElement();
  ~TraNXElement() override = default;

  void draw(tsl::gfx::Renderer *renderer) override;
  void layout(u16 parentX, u16 parentY, u16 parentWidth,
              u16 parentHeight) override;
  tsl::elm::Element *requestFocus(tsl::elm::Element *oldFocus,
                                  tsl::FocusDirection direction) override;

protected:
  void drawRectFrame(tsl::gfx::Renderer *renderer, const app::Frame &frame,
                     tsl::Color color, s32 addH = 0) noexcept;
};

} // namespace front