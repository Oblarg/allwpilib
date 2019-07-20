#include "frc/experimental/command/CommandGroupBase.h"
#include "frc/experimental/command/SequentialCommandGroup.h"
#include "frc/experimental/command/ParallelCommandGroup.h"
#include "frc/errorbase.h"
#include "frc/WPIErrors.h"
#include "frc/experimental/command/ParallelRaceGroup.h"
#include "frc/experimental/command/ParallelDeadlineGroup.h"
#include <set>

using namespace frc::experimental;

template<typename TMap, typename TKey>
static bool ContainsKey(const TMap& map, TKey keyToCheck) {
return map.find(keyToCheck) != map.end();
}
  bool CommandGroupBase::RequireUngrouped(Command& command) {
    if (command.IsGrouped()) {
      wpi_setGlobalWPIErrorWithContext(CommandIllegalUse,
        "Commands cannot be added to more than one CommandGroup");
      return false;
    }
    else {
      return true;
    }
  }

  bool CommandGroupBase::RequireUngrouped(wpi::ArrayRef<std::unique_ptr<Command>> commands) {
    bool allUngrouped = true;
    for(auto&& command : commands) {
    allUngrouped &= !command.get()->IsGrouped();
    }
    if (!allUngrouped) {
    wpi_setGlobalWPIErrorWithContext(CommandIllegalUse,
        "Commands cannot be added to more than one CommandGroup");
    }
    return allUngrouped;
  }

  bool CommandGroupBase::RequireUngrouped(std::initializer_list<Command*> commands) {
    bool allUngrouped = true;
    for(auto&& command : commands) {
    allUngrouped &= !command->IsGrouped();
    }
    if (!allUngrouped) {
    wpi_setGlobalWPIErrorWithContext(CommandIllegalUse,
        "Commands cannot be added to more than one CommandGroup");
    }
    return allUngrouped;
  }

  CommandGroupBase* CommandGroupBase::Sequence(std::vector<std::unique_ptr<Command>>&& commands) {
      return new SequentialCommandGroup(std::move(commands));
  }
  CommandGroupBase* CommandGroupBase::Parallel(std::vector<std::unique_ptr<Command>>&& commands) {
      return new ParallelCommandGroup(std::move(commands));
  }
  CommandGroupBase* CommandGroupBase::Race(std::vector<std::unique_ptr<Command>>&& commands) {
      return new ParallelRaceGroup(std::move(commands));
  }
  CommandGroupBase* CommandGroupBase::Deadline(std::unique_ptr<Command>&& deadline, std::vector<std::unique_ptr<Command>>&& commands) {
      return new ParallelDeadlineGroup(std::move(deadline), std::move(commands));
  }
