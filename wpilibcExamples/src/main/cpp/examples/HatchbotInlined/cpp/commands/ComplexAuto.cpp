/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "HatchbotInlined/include/commands/ComplexAuto.h"

#include <frc/frc2/commands/InstantCommand.h>
#include <frc/frc2/commands/ParallelRaceGroup.h>
#include <frc/frc2/commands/StartEndCommand.h>

using namespace AutoConstants;

ComplexAuto::ComplexAuto(DriveSubsystem* drive, HatchSubsystem* hatch) {
  AddCommands(
      // Drive forward the specified distance
      frc2::StartEndCommand([drive] { drive->ArcadeDrive(kAutoDriveSpeed, 0); },
                            [drive] { drive->ArcadeDrive(0, 0); }, {drive})
          .BeforeStarting([drive] { drive->ResetEncoders(); })
          .InterruptOn([drive] {
            return drive->GetAverageEncoderDistance() >=
                   kAutoDriveDistanceInches;
          }),
      // Release the hatch
      frc2::InstantCommand([hatch] { hatch->ReleaseHatch(); }, {hatch}),
      // Drive backward the specified distance
      frc2::StartEndCommand(
          [drive] { drive->ArcadeDrive(-kAutoDriveSpeed, 0); },
          [drive] { drive->ArcadeDrive(0, 0); }, {drive})
          .BeforeStarting([drive] { drive->ResetEncoders(); })
          .InterruptOn([drive] {
            return drive->GetAverageEncoderDistance() <=
                   kAutoBackupDistanceInches;
          }));
}
