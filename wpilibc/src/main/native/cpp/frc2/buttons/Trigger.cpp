/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/frc2/buttons/Trigger.h"

#include <frc/frc2/commands/Command.h>
#include <frc/frc2/commands/CommandScheduler.h>
#include <frc/frc2/commands/InstantCommand.h>
#include <frc/smartdashboard/SendableBuilder.h>

using namespace frc2;

Trigger::Trigger(const Trigger& other) : m_isActive(other.m_isActive) {}

Trigger Trigger::WhenActive(Command* command, bool interruptible) {
  CommandScheduler::GetInstance().AddButton(
      [pressedLast = Get(), *this, command, interruptible]() mutable {
        bool pressed = Get();

        if (!pressedLast && pressed) {
          command->Schedule(interruptible);
        }

        pressedLast = pressed;
      });

  return *this;
}

Trigger Trigger::WhenActive(std::function<void()> toRun) {
  return WhenActive(new InstantCommand(std::move(toRun), {}));
}

Trigger Trigger::WhileActiveContinous(Command* command, bool interruptible) {
  CommandScheduler::GetInstance().AddButton(
      [pressedLast = Get(), *this, command, interruptible]() mutable {
        bool pressed = Get();

        if (pressed) {
          command->Schedule(interruptible);
        } else if (pressedLast && !pressed) {
          command->Cancel();
        }

        pressedLast = pressed;
      });
  return *this;
}

Trigger Trigger::WhileActiveContinous(std::function<void()> toRun) {
  return WhileActiveContinous(new InstantCommand(std::move(toRun), {}));
}

Trigger Trigger::WhileActiveOnce(Command* command, bool interruptible) {
  CommandScheduler::GetInstance().AddButton(
      [pressedLast = Get(), *this, command, interruptible]() mutable {
        bool pressed = Get();

        if (!pressedLast && pressed) {
          command->Schedule(interruptible);
        } else if (pressedLast && !pressed) {
          command->Cancel();
        }

        pressedLast = pressed;
      });
  return *this;
}

Trigger Trigger::WhenInactive(Command* command, bool interruptible) {
  CommandScheduler::GetInstance().AddButton(
      [pressedLast = Get(), *this, command, interruptible]() mutable {
        bool pressed = Get();

        if (pressedLast && !pressed) {
          command->Schedule(interruptible);
        }

        pressedLast = pressed;
      });
  return *this;
}

Trigger Trigger::WhenInactive(std::function<void()> toRun) {
  return WhenInactive(new InstantCommand(std::move(toRun), {}));
}

Trigger Trigger::ToggleWhenActive(Command* command, bool interruptible) {
  CommandScheduler::GetInstance().AddButton(
      [pressedLast = Get(), *this, command, interruptible]() mutable {
        bool pressed = Get();

        if (!pressedLast && pressed) {
          if (command->IsScheduled()) {
            command->Cancel();
          } else {
            command->Schedule(interruptible);
          }
        }

        pressedLast = pressed;
      });
  return *this;
}

Trigger Trigger::CancelWhenActive(Command* command) {
  CommandScheduler::GetInstance().AddButton(
      [pressedLast = Get(), *this, command]() mutable {
        bool pressed = Get();

        if (!pressedLast && pressed) {
          command->Cancel();
        }

        pressedLast = pressed;
      });
  return *this;
}
