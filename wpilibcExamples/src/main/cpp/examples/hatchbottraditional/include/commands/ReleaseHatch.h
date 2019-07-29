/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc/frc2/commands/CommandHelper.h>
#include <frc/frc2/commands/SendableCommandBase.h>

#include "HatchbotTraditional/include/subsystems/HatchSubsystem.h"

/**
 * A simple command that releases a hatch with the HatchSubsystem.  Written
 * explicitly for pedagogical purposes.  Actual code should inline a command
 * this simple with InstantCommand.
 *
 * @see InstantCommand
 */
class ReleaseHatch
    : public frc2::CommandHelper<frc2::SendableCommandBase, GrabHatch> {
 public:
  explicit ReleaseHatch(HatchSubsystem* subsystem);

  void Initialize() override;

  bool IsFinished() override;

 private:
  HatchSubsystem* m_hatch;
};
