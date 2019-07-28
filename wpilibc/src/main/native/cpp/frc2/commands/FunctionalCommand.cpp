#include "frc/frc2/commands/FunctionalCommand.h"

using namespace frc2;

FunctionalCommand::FunctionalCommand(std::function<void()> onInit, std::function<void()> onExecute, std::function<void(bool)> onEnd, std::function<bool()> isFinished)
  : m_onInit{std::move(onInit)}, m_onExecute{std::move(onExecute)}, m_onEnd{std::move(onEnd)}, m_isFinished{std::move(isFinished)} {
}

void FunctionalCommand::Initialize() {
    m_onInit();
  }

void FunctionalCommand::Execute() {
  m_onExecute();
}

void FunctionalCommand::End(bool interrupted) {
  m_onEnd(interrupted);
}

bool FunctionalCommand::IsFinished() {
  return m_isFinished();
}