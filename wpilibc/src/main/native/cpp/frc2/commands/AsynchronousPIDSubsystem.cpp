/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/frc2/commands/AsynchronousPIDSubsystem.h"

using namespace frc2;

AsynchronousPIDSubsystem::AsynchronousPIDSubsystem(PIDController controller)
    : m_controller(controller),
      m_runner(controller, [this] { return GetMeasurement(); },
               [this](double output) { UseOutput(output); }) {}

void AsynchronousPIDSubsystem::Enable() {
  m_controller.Reset();
  m_runner.Enable();
}

void AsynchronousPIDSubsystem::Disable() {
  m_runner.Disable();
  UseOutput(0);
}

PIDController& AsynchronousPIDSubsystem::GetController() {
  return m_controller;
}
