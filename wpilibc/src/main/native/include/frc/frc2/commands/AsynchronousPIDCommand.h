// /*----------------------------------------------------------------------------*/
// /* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
// /* Open Source Software - may be modified and shared by FRC teams. The code   */
// /* must be accompanied by the FIRST BSD license file in the root directory of */
// /* the project.                                                               */
// /*----------------------------------------------------------------------------*/

// #pragma once

// #include "frc/controller/PIDController.h"
// #include "frc/controller/PIDControllerRunner.h"
// #include "frc/frc2/commands/CommandHelper.h"
// #include "frc/frc2/commands/SendableCommandBase.h"

// namespace frc2 {
// /**
//  * A command that controls an output with a PIDController.  Runs forever by
//  * default - to add exit conditions and/or other behavior, subclass this class.
//  * The controller calculation and output are performed asynchronously by a
//  * separate thread with the period specified by the controller.
//  *
//  * <p>While this class does more than AsynchronousPIDSubsystem to ensure
//  * thread-safety (as it is a fully usable implementation rather than a base for
//  * a user implementation), care should still be taken when using this class to
//  * ensure code remains thread-safe.  If you are unfamiliar with thread-safety,
//  * consider using SynchronousPIDCommand.
//  *
//  * @see PIDController
//  * @see SynchronousPIDCommand
//  */
// class AsynchronousPIDCommand
//     : public CommandHelper<SendableCommandBase, AsynchronousPIDCommand> {
//  public:
//   /**
//    * Creates a new AsynchronousPIDCommand, which controls the given output with
//    * a PIDController.
//    *
//    * @param controller        the controller that controls the output
//    * @param measurementSource the measurement of the process variable
//    * @param setpointSource   the controller's reference (aka setpoint)
//    * @param useOutput         the controller's output
//    * @param requirements      the subsystems required by this command
//    */
//   AsynchronousPIDCommand(PIDController controller,
//                          std::function<double()> measurementSource,
//                          std::function<double()> setpointSource,
//                          std::function<void(double)> useOutput,
//                          std::initializer_list<Subsystem*> requirements);

//   AsynchronousPIDCommand(AsynchronousPIDCommand&& other);

//   AsynchronousPIDCommand(const AsynchronousPIDCommand& other) = delete;

//   void Initialize() override;

//   void Execute() override;

//   void End(bool interrupted) override;

//   /**
//    * Sets the setpoint of the PIDController to track a given source.
//    *
//    * @param setpointSource The setpoint source for the PIDController
//    */
//   double SetSetpoint(std::function<double()> setpointSource);

//   /**
//    * Sets the setpoint of the PIDController to a constant value.
//    *
//    * @param setpoint The setpoint for the PIDController
//    */
//   double SetSetpoint(double setpoint);

//   /**
//    * Sets the setpoint of the PIDController to the current value plus a
//    * specified value.  The set reference will be constant.
//    *
//    * @param relativeSetpoint The amount by which to increase the setpoint for
//    * the PIDController
//    */
//   double SetSetpointRelative(double relativeSetpoint);

//   /**
//    * Returns the PIDController.
//    *
//    * @return The controller.
//    */
//   PIDController& GetController();

//  protected:
//   PIDController m_controller;
//   frc::PIDControllerRunner m_runner;
//   std::function<double()> m_measurement;
//   std::function<double()> m_setpoint;
//   std::function<void(double)> m_useOutput;
//   std::mutex m_lock;

//   /**
//    * Gets the setpoint for the controller.  Wraps the passed-in function so that
//    * changes to the function by a subclass are seen by the runner.
//    *
//    * @return The setpoint for the controller
//    */
//   double GetSetpoint();

//   /**
//    * Gets the measurement of the process variable.  Wraps the passed-in function
//    * so that changes to the function by a subclass are seen by the runner.
//    *
//    * @return the measurement of the process variable
//    */
//   double GetMeasurement();

//   /**
//    * Uses the output of the controller.  Wraps the passed-in function so that
//    * changes to the function by a subclass are seen by the runner.
//    *
//    * @param output the output to use
//    */
//   void UseOutput(double output);
// };
// }  // namespace frc2
