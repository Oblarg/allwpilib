#pragma once
#include "Trigger.h"

namespace frc2 {
class Command;
class Button : public Trigger {
 public:
  Button* WhenPressed(Command* command, bool interruptible);
  Button* WhenPressed(Command* command);
  Button* WhenPressed(std::function<void()> toRun);

  Button* WhileHeld(Command* command, bool interruptible);
  Button* WhileHeld(Command* command);
  Button* WhileHeld(std::function<void()> toRun);

  Button* WhenHeld(Command* command, bool interruptible);
  Button* WhenHeld(Command* command);

  Button* WhenReleased(Command* command, bool interruptible);
  Button* WhenReleased(Command* command);
  Button* WhenReleased(std::function<void()> toRun);

  Button* ToggleWhenPressed(Command* command, bool interruptible);
  Button* ToggleWhenPressed(Command* command);

  Button* CancelWhenPressed(Command* command);
};
}
