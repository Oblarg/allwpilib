#pragma once

#include "SendableCommandBase.h"
#include "CommandHelper.h"

namespace frc2 {
class FunctionalCommand : public CommandHelper<SendableCommandBase, FunctionalCommand> {
 public:
    FunctionalCommand(std::function<void()> onInit, std::function<void()> onExecute, std::function<void(bool)> onEnd, std::function<bool()> isFinished)
    : m_onInit{std::move(onInit)}, m_onExecute{std::move(onExecute)}, m_onEnd{std::move(onEnd)}, m_isFinished{std::move(isFinished)} {
    }

    FunctionalCommand(FunctionalCommand&& other) = default;

    FunctionalCommand(const FunctionalCommand& other) = default;
    
    void Initialize() override {
      m_onInit();
    }
    
    void Execute() override {
      m_onExecute();
    }
    
    void End(bool interrupted) override {
      m_onEnd(interrupted);
    }
    
    bool IsFinished() override {
      return m_isFinished();
    }
 private:
  std::function<void()> m_onInit;
  std::function<void()> m_onExecute;
  std::function<void(bool)> m_onEnd;
  std::function<bool()> m_isFinished;
};
}
