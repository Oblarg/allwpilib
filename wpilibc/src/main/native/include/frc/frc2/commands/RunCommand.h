#pragma once

#include "CommandHelper.h"
#include "SendableCommandBase.h"

namespace frc2 {
class RunCommand : public CommandHelper<SendableCommandBase, RunCommand> {
 public:
  RunCommand(std::function<void()> toRun, std::initializer_list<Subsystem*> requirements) 
    : m_toRun{std::move(toRun)} {
      AddRequirements(requirements);
    }

  RunCommand(RunCommand&& other) = default;

  RunCommand(const RunCommand& other) = default;
    
  void Execute() override {
    m_toRun();
  }
 protected:
  std::function<void()> m_toRun;
};
}
