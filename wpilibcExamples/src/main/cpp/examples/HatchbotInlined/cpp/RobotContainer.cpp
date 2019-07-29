/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "HatchbotInlined/include/RobotContainer.h"

#include <frc/frc2/buttons/JoystickButton.h>
#include <frc/frc2/commands/InstantCommand.h>
#include <frc/frc2/commands/RunCommand.h>
#include <frc/shuffleboard/Shuffleboard.h>

RobotContainer::RobotContainer() {
  // Initialize all of your commands and subsystems here

  // Add commands to the autonomous command chooser
  m_chooser.AddOption("Simple Auto", &m_simpleAuto);
  m_chooser.AddOption("Complex Auto", &m_complexAuto);

  // Put the chooser on the dashboard
  frc::Shuffleboard::GetTab("Autonomous").Add(m_chooser);

  // Configure the button bindings
  ConfigureButtonBindings();

  // Set up default drive command
  m_drive.SetDefaultCommand(new frc2::RunCommand(
      [this] {
        m_drive.ArcadeDrive(
            m_driverController.GetY(frc::GenericHID::kLeftHand),
            m_driverController.GetX(frc::GenericHID::kRightHand));
      },
      {&m_drive}));
}

void RobotContainer::ConfigureButtonBindings() {
  // Configure your button bindings here

  // NOTE: Using `new` here will leak these commands if they are ever no longer
  // needed. This is usually a non-issue as button-bindings tend to be permanent
  // - however, if you wish to avoid this, the commands should be
  // stack-allocated and declared as members of RobotContainer.

  // Grab the hatch when the 'A' button is pressed.
  m_driverController.GetButton(1).WhenPressed(
      new frc2::InstantCommand([this] { m_hatch.GrabHatch(); }, {&m_hatch}));
  // Release the hatch when the 'B' button is pressed.
  m_driverController.GetButton(2).WhenPressed(
      new frc2::InstantCommand([this] { m_hatch.ReleaseHatch(); }, {&m_hatch}));
  // While holding the shoulder button, drive at half speed
  m_driverController.GetButton(6)
      .WhenPressed(
          new frc2::InstantCommand([this] { m_drive.SetMaxOutput(.5); }, {}))
      .WhenReleased(
          new frc2::InstantCommand([this] { m_drive.SetMaxOutput(1); }, {}));
}

frc2::Command* RobotContainer::GetAutonomousCommand() {
  // Runs the chosen command in autonomous
  return m_chooser.GetSelected();
}
