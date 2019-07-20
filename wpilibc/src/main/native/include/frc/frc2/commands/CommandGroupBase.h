#pragma once

#include "SendableCommandBase.h"
#include <set>
#include <frc/ErrorBase.h>

namespace frc2 {
class CommandGroupBase : public SendableCommandBase {
 public:
  static bool RequireUngrouped(Command& command);
  static bool RequireUngrouped(wpi::ArrayRef<std::unique_ptr<Command>>);
  static bool RequireUngrouped(std::initializer_list<Command*>);

  static CommandGroupBase* Sequence(std::vector<std::unique_ptr<Command>>&& commands);
  static CommandGroupBase* Parallel(std::vector<std::unique_ptr<Command>>&& commands);
  static CommandGroupBase* Race(std::vector<std::unique_ptr<Command>>&& commands);
  static CommandGroupBase* Deadline(std::unique_ptr<Command>&& deadline, std::vector<std::unique_ptr<Command>>&& commands);

  virtual void AddCommands(std::vector<std::unique_ptr<Command>>&& commands) = 0;
};
}
