#pragma once
#include "Button.h"

namespace frc {
namespace frc2 {
class InternalButton : public Button {
 public:
  InternalButton() : InternalButton{false} {}
  explicit InternalButton(bool inverted) {
    m_pressed = m_inverted = inverted;
  }

  void SetInverted(bool inverted) {
    m_inverted = inverted;
  }

  void SetPressed(bool pressed) {
    m_pressed = pressed;
  }

  bool Get() const override {
    return m_pressed ^ m_inverted;
  }
 private:
  bool m_pressed{false};
  bool m_inverted{false};
};
}
}
