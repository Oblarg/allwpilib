/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/frc2/commands/SynchronousPIDCommand.h"

using namespace frc2;

SynchronousPIDCommand::SynchronousPIDCommand(
    PIDController controller, std::function<double()> measurementSource,
    std::function<double()> setpointSource,
    std::function<void(double)> useOutput,
    std::initializer_list<Subsystem*> requirements)
    : m_controller{controller},
      m_measurement{measurementSource},
      m_setpoint{setpointSource},
      m_useOutput{useOutput} {}

SynchronousPIDCommand::SynchronousPIDCommand(
    PIDController controller, std::function<double()> measurementSource,
    double setpoint, std::function<void(double)> useOutput,
    std::initializer_list<Subsystem*> requirements)
    : m_controller{controller},
      m_measurement{measurementSource},
      m_setpoint{[setpoint] { return setpoint; }},
      m_useOutput{useOutput} {}

void SynchronousPIDCommand::Initialize() { m_controller.Reset(); }

void SynchronousPIDCommand::Execute() {
  m_useOutput(m_controller.Calculate(m_measurement(), m_setpoint()));
}

void SynchronousPIDCommand::End(bool interrupted) { m_useOutput(0); }

void SynchronousPIDCommand::SetOutput(std::function<void(double)> useOutput) {
  m_useOutput = useOutput;
}

void SynchronousPIDCommand::SetSetpoint(
    std::function<double()> setpointSource) {
  m_setpoint = setpointSource;
}

void SynchronousPIDCommand::SetSetpoint(double setpoint) {
  m_setpoint = [setpoint] { return setpoint; };
}

void SynchronousPIDCommand::SetSetpointRelative(double relativeSetpoint) {
  SetSetpoint(m_setpoint() + relativeSetpoint);
}

PIDController& SynchronousPIDCommand::getController() { return m_controller; }
