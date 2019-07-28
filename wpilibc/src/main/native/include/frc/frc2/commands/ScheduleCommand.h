#pragma once

#include "SendableCommandBase.h"
#include "CommandHelper.h"
#include <wpi/SmallVector.h>
#include "SetUtilities.h"

namespace frc2 { 
/**
 * Schedules the given commands when this command is initialized.  Useful for forking off from
 * CommandGroups.  Note that if run from a CommandGroup, the group will not know about the status
 * of the scheduled commands, and will treat this command as finishing instantly.
 */ 
class ScheduleCommand : public CommandHelper<SendableCommandBase, ScheduleCommand> {
 public:
  /**
   * Creates a new ScheduleCommand that schedules the given commands when initialized.
   *
   * @param toSchedule the commands to schedule
   */
  explicit ScheduleCommand(wpi::ArrayRef<Command*> toSchedule);

  ScheduleCommand(ScheduleCommand&& other) = default;

  ScheduleCommand(const ScheduleCommand& other) = default;
  
  void Initialize() override;
  
  bool IsFinished() override;
  
  bool RunsWhenDisabled() const override;
 private:
  wpi::SmallVector<Command*, 4> m_toSchedule;
};
}
