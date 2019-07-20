#pragma once
#include "Button.h"
#include <frc/GenericHID.h>

namespace frc {
namespace experimental {
class POVButton : public Button {
 public:
  POVButton(frc::GenericHID* joystick, int angle, int povNumber) : m_joystick{joystick}, m_angle{angle}, m_povNumber{povNumber} {}
  POVButton(frc::GenericHID* joystick, int angle) : POVButton{joystick, angle, 0} {}
  bool Get() const override {
    return m_joystick->GetPOV(m_povNumber) == m_angle;
  }
 private:
  frc::GenericHID* m_joystick;
  int m_angle;
  int m_povNumber;
};
}
}
