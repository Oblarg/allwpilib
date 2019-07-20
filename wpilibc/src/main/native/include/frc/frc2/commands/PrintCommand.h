#pragma once

#include "InstantCommand.h"
#include "CommandHelper.h"
#include "wpi/Twine.h"
#include "wpi/raw_ostream.h"

namespace frc2 {
class PrintCommand : public CommandHelper<InstantCommand, PrintCommand>  {
 public:
  PrintCommand(const wpi::Twine& message) 
    : CommandHelper{[str = message.str()]{
      wpi::outs() << str << "\n";
    }, {}} {
    }

  PrintCommand(PrintCommand&& other) = default;

  PrintCommand(const PrintCommand& other) = default;
    
  bool RunsWhenDisabled() const override {
    return true;
  }
};
}
