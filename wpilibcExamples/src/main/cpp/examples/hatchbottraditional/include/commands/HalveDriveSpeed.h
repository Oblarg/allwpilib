/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc/frc2/commands/CommandHelper.h>
#include <frc/frc2/commands/SendableCommandBase.h>

#include "HatchbotTraditional/include/subsystems/DriveSubsystem.h"

class HalveDriveSpeed
    : public frc2::CommandHelper<frc2::SendableCommandBase, HalveDriveSpeed> {
 public:
  explicit HalveDriveSpeed(DriveSubsystem* subsystem);

  void Initialize() override;

  void End(bool interrupted) override;

 private:
  DriveSubsystem* m_drive;
};
