#pragma once
#include "Button.h"
#include <frc/GenericHID.h>

namespace frc {
namespace experimental {
class JoystickButton : public Button {
 public:
  explicit JoystickButton(frc::GenericHID* joystick, int buttonNumber) : m_joystick{joystick}, m_buttonNumber{buttonNumber} {}
  bool Get() const override {
    return m_joystick->GetRawButton(m_buttonNumber);
  }
 private:
  frc::GenericHID* m_joystick;
  int m_buttonNumber;
};
}
}
