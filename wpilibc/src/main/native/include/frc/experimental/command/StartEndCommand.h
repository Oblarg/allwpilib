#pragma once

#include "SendableCommandBase.h"
#include "CommandHelper.h"

namespace frc {
namespace experimental {
class StartEndCommand : public CommandHelper<SendableCommandBase, StartEndCommand> {
 public:
  StartEndCommand(std::function<void()> onInit, std::function<void()> onEnd, std::initializer_list<Subsystem*> requirements) 
    : m_onInit{std::move(onInit)}, m_onEnd{std::move(onEnd)} {
    AddRequirements(requirements);
  }

  StartEndCommand(StartEndCommand&& other) = default;

  StartEndCommand(const StartEndCommand& other) : CommandHelper(other) {
    m_onInit = other.m_onInit;
    m_onEnd = other.m_onEnd;
  };
  
  void Initialize() override {
    m_onInit();
  }
  
  void End(bool interrupted) override {
    m_onEnd();
  }
 protected:
  std::function<void()> m_onInit;
  std::function<void()> m_onEnd;
};
}
}
