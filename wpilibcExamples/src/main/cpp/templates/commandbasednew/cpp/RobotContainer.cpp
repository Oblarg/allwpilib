#include "commandbasednew/include/RobotContainer.h"

RobotContainer::RobotContainer() : m_autonomousCommand(&m_subsystem) {
  //Initialize all of your commands and subsystems here

  //Configure the button bindings
  ConfigureButtonBindings();
}

void RobotContainer::ConfigureButtonBindings() {
  //Configure your button bindings here
}

frc2::Command* RobotContainer::GetAutonomousCommand() {
  //An example command will be run in autonomous
  return &m_autonomousCommand;
}