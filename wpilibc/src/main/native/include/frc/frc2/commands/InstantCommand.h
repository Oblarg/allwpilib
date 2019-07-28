#pragma once

#include "SendableCommandBase.h"
#include "CommandHelper.h"

namespace frc2 {
/**
 * A Command that runs instantly; it will initialize, execute once, and end on the same
 * iteration of the scheduler.  Users can either pass in a Runnable and a set of requirements,
 * or else subclass this command if desired.
 */
class InstantCommand : public CommandHelper<SendableCommandBase, InstantCommand> {
 public:
  /**
   * Creates a new InstantCommand that runs the given Runnable with the given requirements.
   *
   * @param toRun        the Runnable to run
   * @param requirements the subsystems required by this command
   */
  InstantCommand(std::function<void()> toRun, std::initializer_list<Subsystem*> requirements);

  InstantCommand(InstantCommand&& other) = default;

  InstantCommand(const InstantCommand& other) = default;

  /**
   * Creates a new InstantCommand with a Runnable that does nothing.  Useful only as a no-arg
   * constructor to call implicitly from subclass constructors.
   */
  InstantCommand();

  void Initialize() override;

  bool IsFinished() final override;

 private:
  std::function<void()> m_toRun;
};
}
