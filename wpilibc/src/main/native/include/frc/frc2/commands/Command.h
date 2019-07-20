#pragma once

#include <wpi/Twine.h>
#include <wpi/ArrayRef.h>
#include <wpi/SmallSet.h>
#include <frc/frc2/commands/Subsystem.h>
#include <frc/WPIErrors.h>
#include <frc/ErrorBase.h>

namespace frc2 {
std::string demangle(const char* name);

template <typename T>
std::string GetTypeName(const T& type) {
  return demangle(typeid(type).name());
}

class ParallelCommandGroup;
class ParallelRaceGroup;
class ParallelDeadlineGroup;
class SequentialCommandGroup;
class PerpetualCommand;

class Command : public frc::ErrorBase {
 public:
  Command() = default;
  Command(Command&& other) = default;
  virtual ~Command() = default;
  virtual void Initialize();
  virtual void Execute();
  virtual void End(bool interrupted);
  virtual bool IsFinished() { return false; }
  virtual wpi::SmallSet<Subsystem*, 4> GetRequirements() const = 0;

  ParallelRaceGroup WithTimeout(double seconds)&&;
  ParallelRaceGroup InterruptOn(std::function<bool()> condition)&&;
  SequentialCommandGroup BeforeStarting(std::function<void()> toRun)&&;
  SequentialCommandGroup WhenFinished(std::function<void()> toRun)&&;
  PerpetualCommand Perpetually()&&;
  void Schedule(bool interruptible);
  void Schedule() { Schedule(true); }
  void Cancel();
  bool IsScheduled() const;
  bool HasRequirement(Subsystem* requirement) const;
  bool IsGrouped() const;
  void SetGrouped(bool grouped);
  virtual bool RunsWhenDisabled() const { return false; }
  virtual std::string GetName() const;
 protected:
  virtual std::unique_ptr<Command> TransferOwnership()&& = 0;

  bool m_isGrouped = false;
};

static bool RequirementsDisjoint(Command* first, Command* second) {
  bool disjoint = true;
  auto&& requirements = second->GetRequirements();
  for (auto&& requirement : first->GetRequirements()) {
    disjoint &= requirements.find(requirement) == requirements.end(); 
  }
  return disjoint;
}
}
