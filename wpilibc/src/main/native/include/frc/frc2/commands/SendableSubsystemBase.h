#pragma once

#include "Subsystem.h"
#include <frc/smartdashboard/Sendable.h>

namespace frc2 {
/**
 * A base for subsystems that handles registration in the constructor, and provides a more intuitive
 * method for setting the default command.
 */
class SendableSubsystemBase : public Subsystem, public frc::Sendable {
 public:
  void InitSendable(frc::SendableBuilder& builder) override;
  std::string GetName() const override;
  void SetName(const wpi::Twine& name) override;
  std::string GetSubsystem() const override;
  void SetSubsystem(const wpi::Twine& name) override;
 protected:
  SendableSubsystemBase();
  std::string m_name;
};
}
