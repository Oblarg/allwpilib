#pragma once

#include "SendableCommandBase.h"
#include "CommandHelper.h"
#include <wpi/SmallVector.h>
#include "SetUtilities.h"

namespace frc2 {  
class ScheduleCommand : public CommandHelper<SendableCommandBase, ScheduleCommand> {
 public:
  explicit ScheduleCommand(wpi::ArrayRef<Command*> toSchedule) {
    SetInsert(m_toSchedule, toSchedule);
  }

  ScheduleCommand(ScheduleCommand&& other) = default;

  ScheduleCommand(const ScheduleCommand& other) = default;
  
  void Initialize() override {
    for (auto command : m_toSchedule) {
      command->Schedule();
    }
  }
  
  bool IsFinished() override {
    return true;
  }
  
  bool RunsWhenDisabled() const override {
    return true;
  }
 private:
  wpi::SmallVector<Command*, 4> m_toSchedule;
};
}
