/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc/XboxController.h>
#include <frc/frc2/commands/Command.h>
#include <frc/smartdashboard/SendableChooser.h>

#include "hatchbottraditional/include/Constants.h"
#include "hatchbottraditional/include/commands/ComplexAuto.h"
#include "hatchbottraditional/include/commands/DefaultDrive.h"
#include "hatchbottraditional/include/commands/DriveDistance.h"
#include "hatchbottraditional/include/subsystems/DriveSubsystem.h"
#include "hatchbottraditional/include/subsystems/HatchSubsystem.h"

/**
 * This class is where the bulk of the robot should be declared.  Since
 * Command-based is a "declarative" paradigm, very little robot logic should
 * actually be handled in the {@link Robot} periodic methods (other than the
 * scheduler calls).  Instead, the structure of the robot (including subsystems,
 * commands, and button mappings) should be declared here.
 */
class RobotContainer {
 public:
  RobotContainer();

  frc2::Command* GetAutonomousCommand();

 private:
  // The robot's subsystems and commands are defined here...

  // The robot's subsystems
  DriveSubsystem m_drive;
  HatchSubsystem m_hatch;

  // The autonomous routines
  DriveDistance m_simpleAuto{AutoConstants::kAutoDriveDistanceInches, AutoConstants::kAutoDriveSpeed, &m_drive};
  ComplexAuto m_complexAuto{&m_drive, &m_hatch};

  // The chooser for the autonomous routines
  frc::SendableChooser<frc2::Command*> m_chooser;

  // The driver's controller
  frc::XboxController m_driverController{OIConstants::kDriverControllerPort};

  void ConfigureButtonBindings();
};
