/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/frc2/commands/AsynchronousPIDCommand.h"

using namespace frc2;

AsynchronousPIDCommand::AsynchronousPIDCommand(
    PIDController controller, std::function<double()> measurementSource,
    std::function<double()> setpointSource,
    std::function<void(double)> useOutput,
    std::initializer_list<Subsystem*> requirements)
    : m_controller(controller),
      m_runner(controller, [this] { return GetMeasurement(); },
               [this](double output) { UseOutput(output); }),
      m_measurement(measurementSource),
      m_setpoint(setpointSource),
      m_useOutput(useOutput) {
  AddRequirements(requirements);
}

void AsynchronousPIDCommand::Initialize() {
  m_controller.Reset();
  m_runner.Enable();
}

void AsynchronousPIDCommand::Execute() {
  m_controller.SetSetpoint(GetSetpoint());
}

void AsynchronousPIDCommand::End(bool interrupted) { m_runner.Disable(); }

double AsynchronousPIDCommand::SetSetpoint(
    std::function<double()> setpointSource) {
  std::scoped_lock lock(m_lock);
  m_setpoint = setpointSource;
}

double AsynchronousPIDCommand::SetSetpoint(double setpoint) {
  SetSetpoint([setpoint] { return setpoint; });
}

double AsynchronousPIDCommand::SetSetpointRelative(double relativeSetpoint) {
  SetSetpoint(GetSetpoint() + relativeSetpoint);
}

double AsynchronousPIDCommand::GetSetpoint() {
  std::scoped_lock lock(m_lock);
  return m_setpoint();
}

double AsynchronousPIDCommand::GetMeasurement() {
  std::scoped_lock lock(m_lock);
  return m_measurement();
}

void AsynchronousPIDCommand::UseOutput(double output) {
  std::scoped_lock lock(m_lock);
  m_useOutput(output);
}

PIDController& AsynchronousPIDCommand::GetController() { return m_controller; }
