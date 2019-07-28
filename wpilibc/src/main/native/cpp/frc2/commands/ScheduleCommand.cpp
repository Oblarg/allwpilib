#include "frc/frc2/commands/ScheduleCommand.h"

using namespace frc2;

ScheduleCommand::ScheduleCommand(wpi::ArrayRef<Command*> toSchedule) {
  SetInsert(m_toSchedule, toSchedule);
}

void ScheduleCommand::Initialize() {
    for (auto command : m_toSchedule) {
      command->Schedule();
    }
  }
  
  bool ScheduleCommand::IsFinished() {
    return true;
  }
  
  bool ScheduleCommand::RunsWhenDisabled() const {
    return true;
  }