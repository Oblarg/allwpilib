#include "frc/experimental/buttons/Button.h"

using namespace frc::experimental;

Button* Button::WhenPressed(Command* command, bool interruptible) {
  WhenActive(command, interruptible);
  return this;
}

Button* Button::WhenPressed(Command* command) {
  WhenActive(command);
  return this;
}

Button* Button::WhenPressed(std::function<void()> toRun) {
  WhenActive(std::move(toRun));
  return this;
}

Button* Button::WhileHeld(Command* command, bool interruptible) {
  WhileActiveContinous(command, interruptible);
  return this;
}

Button* Button::WhileHeld(Command* command) {
  WhileActiveContinous(command);
  return this;
}

Button* Button::WhileHeld(std::function<void()> toRun) {
  WhileActiveContinous(std::move(toRun));
  return this;
}

Button* Button::WhenHeld(Command* command, bool interruptible) {
  WhileActiveOnce(command, interruptible);
  return this;
}

Button* Button::WhenHeld(Command* command) {
  WhileActiveOnce(command);
  return this;
}

Button* Button::WhenReleased(Command* command, bool interruptible) {
  WhenInactive(command, interruptible);
  return this;
}

Button* Button::WhenReleased(Command* command) {
  WhenInactive(command);
  return this;
}

Button* Button::WhenReleased(std::function<void()> toRun) {
  WhenInactive(std::move(toRun));
  return this;
}

Button* Button::ToggleWhenPressed(Command* command, bool interruptible) {
  ToggleWhenActive(command, interruptible);
  return this;
}

Button* Button::ToggleWhenPressed(Command* command) {
  ToggleWhenActive(command);
  return this;
}

Button* Button::CancelWhenPressed(Command* command) {
  CancelWhenActive(command);
  return this;
}
