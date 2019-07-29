/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "HatchbotTraditional/include/commands/DriveDistance.h"

DriveDistance::DriveDistance(double inches, double speed,
                             DriveSubsystem* subsystem)
    : m_distance(inches), m_speed(speed), m_drive(subsystem) {
  AddRequirements({subsystem});
}

void DriveDistance::Initialize() {
  m_drive->ResetEncoders();
  m_drive->ArcadeDrive(m_speed, 0);
}

void DriveDistance::End(bool interrupted) { m_drive->ArcadeDrive(0, 0); }

bool DriveDistance::IsFinished() {
  return std::abs(m_drive->GetAverageEncoderDistance()) >= m_distance;
}
