/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/frc2/commands/ProxyScheduleCommand.h"

using namespace frc2;

ProxyScheduleCommand::ProxyScheduleCommand(wpi::ArrayRef<Command*> toSchedule) {
  SetInsert(m_toSchedule, toSchedule);
}

void ProxyScheduleCommand::Initialize() {
  m_finished = false;
  for (auto* command : m_toSchedule) {
    command->Schedule();
  }
}

void ProxyScheduleCommand::End(bool interrupted) {
  if (interrupted) {
    for (auto* command : m_toSchedule) {
      command->Cancel();
    }
  }
}

void ProxyScheduleCommand::Execute() {
  for (auto* command : m_toSchedule) {
    m_finished &= !command->IsScheduled();
  }
}

bool ProxyScheduleCommand::IsFinished() {
  return m_finished;
}
