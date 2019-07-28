#include "frc/frc2/commands/SynchronousPIDCommand.h"

using namespace frc2;

SynchronousPIDCommand::SynchronousPIDCommand(PIDController controller, 
  std::function<double()> measurementSource, 
  std::function<double()> setpointSource, 
  std::function<void(double)> useOutput, 
  std::initializer_list<Subsystem*> requirements) : m_controller{controller}, m_measurement{measurementSource}, m_setpoint{setpointSource}, m_useOutput{useOutput} {}

SynchronousPIDCommand::SynchronousPIDCommand(PIDController controller, 
  std::function<double()> measurementSource, 
  double setpoint,
  std::function<void(double)> useOutput, 
  std::initializer_list<Subsystem*> requirements) : m_controller{controller}, m_measurement{measurementSource}, m_setpoint{[setpoint]{return setpoint;}}, m_useOutput{useOutput} {}

void SynchronousPIDCommand::Initialize() {
  m_controller.Reset();
}

void SynchronousPIDCommand::Execute() {
  m_useOutput(m_controller.Calculate(m_measurement(), m_setpoint()));
}

void SynchronousPIDCommand::End(bool interrupted) {
  m_useOutput(0);
}

void SynchronousPIDCommand::SetOutput(std::function<void(double)> useOutput) {m_useOutput = useOutput;}

void SynchronousPIDCommand::SetSetpoint(std::function<double()> setpointSource) {m_setpoint = setpointSource;}

void SynchronousPIDCommand::SetSetpoint(double setpoint) {m_setpoint = [setpoint]{return setpoint;};}

void SynchronousPIDCommand::SetSetpointRelative(double relativeSetpoint) {SetSetpoint(m_setpoint() + relativeSetpoint);}

PIDController& SynchronousPIDCommand::getController() {return m_controller;}