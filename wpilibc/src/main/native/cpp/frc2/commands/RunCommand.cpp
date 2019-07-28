#include "frc/frc2/commands/RunCommand.h"

using namespace frc2;

RunCommand::RunCommand(std::function<void()> toRun, std::initializer_list<Subsystem*> requirements) 
  : m_toRun{std::move(toRun)} {
  AddRequirements(requirements);
}

void RunCommand::Execute() {
  m_toRun();
}