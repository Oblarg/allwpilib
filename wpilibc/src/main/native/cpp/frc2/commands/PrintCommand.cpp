#include "frc/frc2/commands/PrintCommand.h"

using namespace frc2;

PrintCommand::PrintCommand(const wpi::Twine& message) 
  : CommandHelper{[str = message.str()]{wpi::outs() << str << "\n";}, {}} {}

bool PrintCommand::RunsWhenDisabled() const {
  return true;
}