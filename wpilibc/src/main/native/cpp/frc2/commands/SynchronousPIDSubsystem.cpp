#include "frc/frc2/commands/SynchronousPIDSubsystem.h"

using namespace frc2;

SynchronousPIDSubsystem::SynchronousPIDSubsystem(PIDController controller) : m_controller{controller} {}

void SynchronousPIDSubsystem::Periodic() {
  if (m_enabled) {
    UseOutput(m_controller.Calculate(GetMeasurement(), GetSetpoint()));
  }
}

void SynchronousPIDSubsystem::Enable() {
  m_controller.Reset();
  m_enabled = true;
}

void SynchronousPIDSubsystem::Disable() {
  UseOutput(0);
  m_enabled = false;
}