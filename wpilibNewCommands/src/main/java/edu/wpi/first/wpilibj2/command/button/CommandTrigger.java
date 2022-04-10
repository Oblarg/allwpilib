// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj2.command.button;

import edu.wpi.first.math.filter.Debouncer;
import edu.wpi.first.wpilibj2.command.Command;
import java.util.function.BooleanSupplier;

/**
 * This class provides an easy way to link commands to inputs.
 *
 * <p>It is very easy to link a button to a command. For instance, you could link the trigger button
 * of a joystick to a "score" command.
 *
 * <p>It is encouraged that teams write a subclass of Trigger if they want to have something unusual
 * (for instance, if they want to react to the user holding a button while the robot is reading a
 * certain sensor input). For this, they only have to write the {@link CommandTrigger#getAsBoolean()} method to get
 * the full functionality of the Trigger class.
 *
 * <p>This class is provided by the NewCommands VendorDep
 */
public class CommandTrigger extends UnitEvent {

  public CommandTrigger(EventLoop loop, BooleanSupplier trigger) {
    super(loop, trigger);
  }

  public CommandTrigger onTrue(final Command command) {
    rising().whenTrue(command::schedule);
    return this;
  }

  public CommandTrigger whileTrue(final Command command) {
    rising().whenTrue(command::schedule);
    falling().whenTrue(command::cancel);
    return this;
  }

  public CommandTrigger onFalse(final Command command) {
    negate().onTrue(command);
    return this;
  }

  public CommandTrigger whileFalse(final Command command) {
    negate().whileTrue(command);
    return this;
  }


  public CommandTrigger toggleOnTrue(final Command command) {
    rising().and(() -> !command.isScheduled()).whenTrue(command::schedule);
    rising().and(command::isScheduled).whenTrue(command::cancel);
    return this;
  }

  public CommandTrigger toggleOnFalse(final Command command) {
    return negate().toggleOnTrue(command);
  }

  /**
   * Composes this trigger with another trigger, returning a new trigger that is active when both
   * triggers are active.
   *
   * @param other the trigger to compose with
   * @return the trigger that is active when both triggers are active
   */
  public CommandTrigger and(BooleanSupplier other) {
    return new CommandTrigger(m_loop, super.and(other));
  }

  /**
   * Composes this trigger with another trigger, returning a new trigger that is active when either
   * trigger is active.
   *
   * @param other the trigger to compose with
   * @return the trigger that is active when either trigger is active
   */
  public CommandTrigger or(BooleanSupplier other) {
    return new CommandTrigger(m_loop, super.or(other));
  }

  /**
   * Creates a new trigger that is active when this trigger is inactive, i.e. that acts as the
   * negation of this trigger.
   *
   * @return the negated trigger
   */
  public CommandTrigger negate() {
    return new CommandTrigger(m_loop, super.negate());
  }

  /**
   * Creates a new debounced trigger from this trigger - it will become active when this trigger has
   * been active for longer than the specified period.
   *
   * @param seconds The debounce period.
   * @return The debounced trigger (rising edges debounced only)
   */
  public CommandTrigger debounce(double seconds) {
    return debounce(seconds, Debouncer.DebounceType.kRising);
  }

  /**
   * Creates a new debounced trigger from this trigger - it will become active when this trigger has
   * been active for longer than the specified period.
   *
   * @param seconds The debounce period.
   * @param type The debounce type.
   * @return The debounced trigger.
   */
  public CommandTrigger debounce(double seconds, Debouncer.DebounceType type) {
    return new CommandTrigger(m_loop, super.debounce(seconds, type));
  }
}
