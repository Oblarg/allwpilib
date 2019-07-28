#pragma once

#include "InstantCommand.h"
#include "CommandHelper.h"
#include "wpi/Twine.h"
#include "wpi/raw_ostream.h"

namespace frc2 {
/**
 * A command that prints a string when initialized.
 */
class PrintCommand : public CommandHelper<InstantCommand, PrintCommand>  {
 public:
  /**
   * Creates a new a PrintCommand.
   *
   * @param message the message to print
   */
  explicit PrintCommand(const wpi::Twine& message);

  PrintCommand(PrintCommand&& other) = default;

  PrintCommand(const PrintCommand& other) = default;
    
  bool RunsWhenDisabled() const override;
};
}
