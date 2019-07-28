/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "frc/controller/PIDController.h"
#include "frc/controller/PIDControllerRunner.h"
#include "frc/frc2/commands/CommandHelper.h"
#include "frc/frc2/commands/SendableSubsystemBase.h"

namespace frc2 {
/**
 * A subsystem uses a PIDController to control an output.  The controller is run
 * asynchronously by a separate thread with the period specified by the
 * controller.
 *
 * <p>Care should be taken to ensure the implementation of this class is
 * thread-safe; if you are not familiar with thread safety, consider using
 * SynchronousPIDSubsystem instead.
 *
 * @see PIDController
 * @see SynchronousPIDSubsystem
 */
class AsynchronousPIDSubsystem : public SendableSubsystemBase {
 public:
  /**
   * Creates a new AsynchronousPIDSubsystem.
   *
   * @param controller the controller to use
   */
  explicit AsynchronousPIDSubsystem(PIDController controller);

  /**
   * Enables the PID control.  Resets the controller.
   */
  virtual void Enable();

  /**
   * Disables the PID control.  Sets the output to zero.
   */
  virtual void Disable();

  PIDController& GetController();

 protected:
  PIDController m_controller;
  frc::PIDControllerRunner m_runner;

  /**
   * Returns the setpoint used by the PIDController.  Should be synchronized to
   * remain threadsafe.
   *
   * @return The setpoint to be used by the controller
   */
  virtual double GetSetpoint() = 0;

  /**
   * Returns the measurement of the process variable used by the PIDController.
   * Should be synchronized to remain threadsafe.
   *
   * @return The measurement of the process variable
   */
  virtual double GetMeasurement() = 0;

  /**
   * Uses the output from the PIDController.  Should be synchronized to remain
   * threadsafe.
   *
   * @param output The output of the PIDController
   */
  virtual void UseOutput(double output) = 0;
};
}  // namespace frc2
