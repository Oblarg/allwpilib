#pragma once

#include <frc/smartdashboard/Sendable.h>
#include "Command.h"
#include <wpi/SmallVector.h>
#include <wpi/SmallSet.h>

namespace frc2 {
/**
 * A Sendable base class for Commands.
 */
class SendableCommandBase : public frc::Sendable, public Command {
 public:
  SendableCommandBase(SendableCommandBase&& other) = default;

  SendableCommandBase(const SendableCommandBase& other);

  /**
   * Adds the specified requirements to the command.
   *
   * @param requirements the requirements to add
   */
  void AddRequirements(std::initializer_list<Subsystem*> requirements);

  void AddRequirements(wpi::SmallSet<Subsystem*, 4> requirements);

  wpi::SmallSet<Subsystem*, 4> GetRequirements() const override;

  void SetName(const wpi::Twine& name) override {
    m_name = name.str();
  }

  std::string GetName() const override {
    return m_name;
  }

  std::string GetSubsystem() const override {
    return m_subsystem;
  }

  void SetSubsystem(const wpi::Twine& subsystem) override {
    m_subsystem = subsystem.str();
  }

  void InitSendable(frc::SendableBuilder& builder) override;


 protected:
  SendableCommandBase();
  std::string m_name;
  std::string m_subsystem;
  wpi::SmallSet<Subsystem*, 4> m_requirements;
};
}
