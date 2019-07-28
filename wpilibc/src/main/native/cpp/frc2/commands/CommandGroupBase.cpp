#include "frc/frc2/commands/CommandGroupBase.h"
#include "frc/frc2/commands/SequentialCommandGroup.h"
#include "frc/frc2/commands/ParallelCommandGroup.h"
#include "frc/errorbase.h"
#include "frc/WPIErrors.h"
#include "frc/frc2/commands/ParallelRaceGroup.h"
#include "frc/frc2/commands/ParallelDeadlineGroup.h"
#include <set>

using namespace frc2;
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
