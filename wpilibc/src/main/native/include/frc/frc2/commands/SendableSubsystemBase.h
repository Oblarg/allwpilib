#pragma once

#include "Subsystem.h"
#include <frc/smartdashboard/Sendable.h>

namespace frc2 {
class SendableSubsystemBase : public Subsystem, public frc::Sendable {
 public:
  std::string GetName() const override { return m_name; }
  void SetName(const wpi::Twine& name) override { m_name = name.str(); }
  std::string GetSubsystem() const override { return GetName(); }
  void SetSubsystem(const wpi::Twine& name) override { SetName(name); }
  void InitSendable(frc::SendableBuilder& builder) override;
 protected:
  SendableSubsystemBase();
  std::string m_name;
};
}
