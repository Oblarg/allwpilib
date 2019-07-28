/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once
#include "Button.h"

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
}  // namespace frc2
