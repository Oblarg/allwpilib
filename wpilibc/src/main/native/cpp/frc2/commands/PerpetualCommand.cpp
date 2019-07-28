#include "frc/frc2/commands/PerpetualCommand.h"

using namespace frc2;

PerpetualCommand::PerpetualCommand(std::unique_ptr<Command>&& command) {
    if (!CommandGroupBase::RequireUngrouped(command)) { 
      return; 
    }
    m_command = std::move(command);
    m_command->SetGrouped(true);
    AddRequirements(m_command->GetRequirements());
}

void PerpetualCommand::Initialize() {
  m_command->Initialize();
}

void PerpetualCommand::Execute() {
  m_command->Execute();
}

void PerpetualCommand::End(bool interrupted) {
  m_command->End(interrupted);
}