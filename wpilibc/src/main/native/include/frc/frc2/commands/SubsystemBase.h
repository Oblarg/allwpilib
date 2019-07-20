#pragma once

#include "Subsystem.h"

namespace frc2 {
class SusbsystemBase : public Subsystem {
 public:
  SusbsystemBase();

  void SetDefaultCommand(Command* command) override {
    m_defaultCommand = command;
  }

  Command* GetDefaultCommand() const override {
    return m_defaultCommand;
  }
 private:
  Command* m_defaultCommand;
};
}
