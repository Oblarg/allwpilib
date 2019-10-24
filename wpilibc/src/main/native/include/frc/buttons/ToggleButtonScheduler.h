/*----------------------------------------------------------------------------*/
/* Copyright (c) 2011-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "frc/buttons/ButtonScheduler.h"

namespace frc {

class Trigger;
class Command;

class ToggleButtonScheduler : public ButtonScheduler {
 public:
  WPI_DEPRECATED(
      "Use the new command-based framework in the frc2 namespace, instead.")
  ToggleButtonScheduler(bool last, Trigger* button, Command* orders);
  virtual ~ToggleButtonScheduler() = default;

  ToggleButtonScheduler(ToggleButtonScheduler&&) = default;
  ToggleButtonScheduler& operator=(ToggleButtonScheduler&&) = default;

  virtual void Execute();
};

}  // namespace frc
