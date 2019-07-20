#include <frc/frc2/commands/Subsystem.h>
#include <frc/frc2/commands/CommandScheduler.h>

using namespace frc::frc2;

  Subsystem::~Subsystem() {
      CommandScheduler::GetInstance().UnregisterSubsystem(this);
  }

  void Subsystem::Periodic() {

  }
  void Subsystem::SetDefaultCommand(Command* defaultCommand) {
      CommandScheduler::GetInstance().SetDefaultCommand(this, defaultCommand);
  }
  Command* Subsystem::GetDefaultCommand() const {
      return CommandScheduler::GetInstance().GetDefaultCommand(this);
  }
  Command* Subsystem::GetCurrentCommand() const {
      return CommandScheduler::GetInstance().Requiring(this);
  }
  void Subsystem::Register() {
      return CommandScheduler::GetInstance().RegisterSubsystem(this);
  }
