#pragma once

#include "SendableCommandBase.h"
#include "CommandHelper.h"
#include <wpi/SmallVector.h>
#include "SetUtilities.h"

namespace frc {
namespace experimental {
class ProxyScheduleCommand : public CommandHelper<SendableCommandBase, ProxyScheduleCommand> {
 public:
  ProxyScheduleCommand(wpi::ArrayRef<Command*> toSchedule) {
    SetInsert(m_toSchedule, toSchedule);
  }

  ProxyScheduleCommand(ProxyScheduleCommand&& other) = default;

  ProxyScheduleCommand(const ProxyScheduleCommand& other) = default;

  void Initialize() override {
    m_finished = false;
    for (auto* command : m_toSchedule) {
      command->Schedule();
    }
  }

  void End(bool interrupted) override {
    if (interrupted) {
      for (auto* command : m_toSchedule) {
        command->Cancel();
      }
    }
  }

  void Execute() override {
    for (auto* command : m_toSchedule) {
      m_finished &= !command->IsScheduled();
    }
  }

  bool IsFinished() override {
    return m_finished;
  }
 private:
  wpi::SmallVector<Command*,4> m_toSchedule;
  bool m_finished{false};
};
}
}
