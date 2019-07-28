#include "frc/frc2/commands/InstantCommand.h"

using namespace frc2;

InstantCommand::InstantCommand(std::function<void()> toRun, std::initializer_list<Subsystem*> requirements) : m_toRun{std::move(toRun)} {
  AddRequirements(requirements);
}

InstantCommand::InstantCommand() : m_toRun{[]{}} {}

void InstantCommand::Initialize() {
  m_toRun();
}

bool InstantCommand::IsFinished() {
  return true;
}