#pragma once

#include "SendableCommandBase.h"
#include "CommandHelper.h"
#include <wpi/SmallVector.h>
#include "SetUtilities.h"

namespace frc2 {
/**
 * Schedules the given commands when this command is initialized, and ends when all the commands are
 * no longer scheduled.  Useful for forking off from CommandGroups.  If this command is interrupted,
 * it will cancel all of the commands.
 */
class ProxyScheduleCommand : public CommandHelper<SendableCommandBase, ProxyScheduleCommand> {
 public:
  /**
   * Creates a new ProxyScheduleCommand that schedules the given commands when initialized,
   * and ends when they are all no longer scheduled.
   *
   * @param toSchedule the commands to schedule
   */
  explicit ProxyScheduleCommand(wpi::ArrayRef<Command*> toSchedule);

  ProxyScheduleCommand(ProxyScheduleCommand&& other) = default;

  ProxyScheduleCommand(const ProxyScheduleCommand& other) = default;

  void Initialize() override;

  void End(bool interrupted) override;

  void Execute() override;

  bool IsFinished() override;
 private:
  wpi::SmallVector<Command*,4> m_toSchedule;
  bool m_finished{false};
};
}
