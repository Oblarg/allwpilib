#pragma once

#include "frc/frc2/commands/SendableCommandBase.h"
#include "frc/frc2/commands/CommandHelper.h"
#include "frc/controller/PIDController.h"

namespace frc2{
/**
 * A command that controls an output with a PIDController.  Runs forever by default - to add
 * exit conditions and/or other behavior, subclass this class.  The controller calculation and
 * output are performed synchronously in the command's execute() method.
 * 
 * @see PIDController
 */
class SynchronousPIDCommand : public CommandHelper<SendableCommandBase, SynchronousPIDCommand> {
 public:
  /**
   * Creates a new SynchronousPIDCommand, which controls the given output with a PIDController.
   *
   * @param controller        the controller that controls the output.
   * @param measurementSource the measurement of the process variable
   * @param setpointSource   the controller's reference (aka setpoint)
   * @param useOutput         the controller's output
   * @param requirements      the subsystems required by this command
   */
  SynchronousPIDCommand(PIDController controller, 
    std::function<double()> measurementSource, 
    std::function<double()> setpointSource, 
    std::function<void(double)> useOutput, 
    std::initializer_list<Subsystem*> requirements);

  /**
   * Creates a new SynchronousPIDCommand, which controls the given output with a PIDController with a constant setpoint.
   *
   * @param controller        the controller that controls the output.
   * @param measurementSource the measurement of the process variable
   * @param setpoint         the controller's setpoint (aka setpoint)
   * @param useOutput         the controller's output
   * @param requirements      the subsystems required by this command
   */
  SynchronousPIDCommand(PIDController controller, 
    std::function<double()> measurementSource, 
    double setpoint, 
    std::function<void(double)> useOutput, 
    std::initializer_list<Subsystem*> requirements);

  SynchronousPIDCommand(SynchronousPIDCommand&& other) = default;

  SynchronousPIDCommand(const SynchronousPIDCommand& other) = default;

  void Initialize() override;

  void Execute() override;

  void End(bool interrupted) override;

  /**
   * Sets the function that uses the output of the PIDController.
   * 
   * @param useOutput The function that uses the output.
   */
  void SetOutput(std::function<void(double)> useOutput);

  /**
   * Sets the setpoint for the controller to track the given source.
   *
   * @param setpointSource The setpoint source
   */
  void SetSetpoint(std::function<double()> setpointSource);

  /**
   * Sets the setpoint for the controller to a constant value.
   *
   * @param setpoint The setpoint
   */
  void SetSetpoint(double setpoint);

  /**
   * Sets the setpoint for the controller to a constant value relative (i.e. added to) the current setpoint.
   *
   * @param relativeReference The change in setpoint
   */
  void SetSetpointRelative(double relativeSetpoint);

  /**
   * Returns the PIDController used by the command.
   *
   * @return The PIDController
   */
  PIDController& getController();
  
 protected:
  PIDController m_controller;
  std::function<double()> m_measurement;
  std::function<double()> m_setpoint;
  std::function<void(double)> m_useOutput;
};
}