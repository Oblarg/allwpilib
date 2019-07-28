#pragma once

#include "frc/frc2/commands/SendableSubsystemBase.h"
#include "frc/controller/PIDController.h"

namespace frc2{
/**
 * A subsystem that uses a PIDController to control an output.  The controller is run
 * synchronously from the subsystem's periodic() method.
 * 
 * @see PIDController
 */
class SynchronousPIDSubsystem : public SendableSubsystemBase {
 public:
  /**
   * Creates a new SynchronousPIDSubsystem.
   *
   * @param controller the PIDController to use
   */
  explicit SynchronousPIDSubsystem(PIDController controller);

  void Periodic() override;

  /**
   * Uses the output from the PIDController.
   *
   * @param output the output of the PIDController
   */
  virtual void UseOutput(double output) = 0;

  /**
   * Returns the reference (setpoint) used by the PIDController.
   *
   * @return the reference (setpoint) to be used by the controller
   */
  virtual double GetSetpoint() = 0;

  /**
   * Returns the measurement of the process variable used by the PIDController.
   *
   * @return the measurement of the process variable
   */
  virtual double GetMeasurement() = 0;

  /**
   * Enables the PID control.  Resets the controller.
   */
  void Enable();

  /**
   * Disables the PID control.  Sets output to zero.
   */
  void Disable();

 protected:
  PIDController m_controller;
  bool m_enabled;
};
}