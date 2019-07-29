#pragma once

#include <frc/frc2/commands/SendableCommandBase.h>
#include <frc/frc2/commands/CommandHelper.h>

#include "commandbasednew/include/subsystems/ExampleSubsystem.h"

/**
 * An example command that uses an example subsystem.
 * 
 * <p>Note that this extends CommandHelper, rather extending SendableCommandBase directly; 
 * this is crucially important, or else the decorator functions in Command will *not* work!
 */
class ExampleCommand : public frc2::CommandHelper<frc2::SendableCommandBase, ExampleCommand> {
 public:
  /**
   * Creates a new ExampleCommand.
   *
   * @param subsystem The subsystem used by this command.
   */
  explicit ExampleCommand(ExampleSubsystem* subsystem);

 private:
  ExampleSubsystem* m_subsystem;
};