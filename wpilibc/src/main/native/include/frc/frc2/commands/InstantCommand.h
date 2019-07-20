#pragma once

#include "SendableCommandBase.h"
#include "CommandHelper.h"

namespace frc {
namespace frc2 {
class InstantCommand : public CommandHelper<SendableCommandBase, InstantCommand> {
 public:
  InstantCommand(std::function<void()> toRun, std::initializer_list<Subsystem*> requirements) : m_toRun{std::move(toRun)} {
    AddRequirements(requirements);
  }

  InstantCommand(InstantCommand&& other) = default;

  InstantCommand(const InstantCommand& other) = default;

  InstantCommand() : m_toRun{[]{}} {
  }

  void b() {

  }

  void Initialize() override {
    m_toRun();
  }

  bool IsFinished() final override {
    return true;
  }

 private:
  std::function<void()> m_toRun;
};
}
}
