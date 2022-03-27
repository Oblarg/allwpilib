// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj2.command.button;

import static edu.wpi.first.wpilibj.util.ErrorMessages.requireNonNullParam;

import edu.wpi.first.math.filter.Debouncer;
import edu.wpi.first.wpilibj2.command.Command;
import edu.wpi.first.wpilibj2.command.CommandScheduler;
import edu.wpi.first.wpilibj2.command.InstantCommand;
import edu.wpi.first.wpilibj2.command.Subsystem;
import java.util.function.BooleanSupplier;

/**
 * This class is a command-based wrapper around {@link BooleanEvent}, providing an easy way to link
 * commands to inputs.
 *
 * <p>It is very easy to link a button to a command. For instance, you could link the trigger button
 * of a joystick to a "score" command.
 *
 * <p>It is encouraged that teams write a subclass of Trigger if they want to have something unusual
 * (for instance, if they want to react to the user holding a button while the robot is reading a
 * certain sensor input). For this, they only have to write the {@link Trigger#get()} method to get
 * the full functionality of the Trigger class.
 *
 * <p>This class is provided by the NewCommands VendorDep
 */
public class Trigger extends BooleanEvent {
  /**
   * Creates a new trigger with the given condition determining whether it is active.
   *
   * @param loop the loop that polls this trigger
   * @param isActive returns whether or not the trigger should be active
   */
  public Trigger(EventLoop loop, BooleanSupplier isActive) {
    super(loop, isActive);
  }

  /**
   * Copies the BooleanEvent into a Trigger object.
   *
   * @param toCast the BooleanEvent
   */
  public static Trigger cast(BooleanEvent toCast) {
    // use the event itself as a condition (instead of copying the condition) so that debounce and
    // similar are kept.
    return new Trigger(toCast.m_loop, toCast);
  }

  /**
   * Creates a new trigger with the given condition determining whether it is active.
   *
   * <p>Polled by the current scheduler button loop.
   *
   * @param isActive returns whether or not the trigger should be active
   */
  public Trigger(BooleanSupplier isActive) {
    this(CommandScheduler.getInstance().getButtonLoop(), isActive);
  }

  /**
   * Creates a new trigger that is always inactive. Useful only as a no-arg constructor for
   * subclasses that will be overriding {@link Trigger#get()} anyway.
   */
  public Trigger() {
    this(() -> false);
  }

  /**
   * Starts the given command whenever the trigger just becomes active.
   *
   * @param command the command to start
   * @param interruptible whether the command is interruptible
   * @return this trigger, so calls can be chained
   */
  public Trigger whenActive(final Command command, boolean interruptible) {
    requireNonNullParam(command, "command", "whenActive");

    this.rising().whenTrue(() -> command.schedule(interruptible));
    return this;
  }

  /**
   * Starts the given command whenever the trigger just becomes active. The command is set to be
   * interruptible.
   *
   * @param command the command to start
   * @return this trigger, so calls can be chained
   */
  public Trigger whenActive(final Command command) {
    return whenActive(command, true);
  }

  /**
   * Runs the given runnable whenever the trigger just becomes active.
   *
   * @param toRun the runnable to run
   * @param requirements the required subsystems
   * @return this trigger, so calls can be chained
   */
  public Trigger whenActive(final Runnable toRun, Subsystem... requirements) {
    // TODO perhaps replace with directly binding it if no requirements are given?
    return whenActive(new InstantCommand(toRun, requirements));
  }

  /**
   * Constantly starts the given command while the button is held.
   *
   * <p>{@link Command#schedule(boolean)} will be called repeatedly while the trigger is active, and
   * will be canceled when the trigger becomes inactive.
   *
   * @param command the command to start
   * @param interruptible whether the command is interruptible
   * @return this trigger, so calls can be chained
   */
  public Trigger whileActiveContinuous(final Command command, boolean interruptible) {
    requireNonNullParam(command, "command", "whileActiveContinuous");

    this.whenTrue(() -> command.schedule(interruptible));
    this.falling().whenTrue(command::cancel);

    return this;
  }

  /**
   * Constantly starts the given command while the button is held.
   *
   * <p>{@link Command#schedule(boolean)} will be called repeatedly while the trigger is active, and
   * will be canceled when the trigger becomes inactive. The command is set to be interruptible.
   *
   * @param command the command to start
   * @return this trigger, so calls can be chained
   */
  public Trigger whileActiveContinuous(final Command command) {
    return whileActiveContinuous(command, true);
  }

  /**
   * Constantly runs the given runnable while the button is held.
   *
   * @param toRun the runnable to run
   * @param requirements the required subsystems
   * @return this trigger, so calls can be chained
   */
  public Trigger whileActiveContinuous(final Runnable toRun, Subsystem... requirements) {
    // TODO perhaps replace with directly binding it if no requirements are given?
    return whileActiveContinuous(new InstantCommand(toRun, requirements));
  }

  /**
   * Starts the given command when the trigger initially becomes active, and ends it when it becomes
   * inactive, but does not re-start it in-between.
   *
   * @param command the command to start
   * @param interruptible whether the command is interruptible
   * @return this trigger, so calls can be chained
   */
  public Trigger whileActiveOnce(final Command command, boolean interruptible) {
    requireNonNullParam(command, "command", "whileActiveOnce");

    this.rising().whenTrue(() -> command.schedule(interruptible));
    this.falling().whenTrue(command::cancel);

    return this;
  }

  /**
   * Starts the given command when the trigger initially becomes active, and ends it when it becomes
   * inactive, but does not re-start it in-between. The command is set to be interruptible.
   *
   * @param command the command to start
   * @return this trigger, so calls can be chained
   */
  public Trigger whileActiveOnce(final Command command) {
    return whileActiveOnce(command, true);
  }

  /**
   * Starts the command when the trigger becomes inactive.
   *
   * @param command the command to start
   * @param interruptible whether the command is interruptible
   * @return this trigger, so calls can be chained
   */
  public Trigger whenInactive(final Command command, boolean interruptible) {
    requireNonNullParam(command, "command", "whenInactive");

    this.falling().whenTrue(() -> command.schedule(interruptible));

    return this;
  }

  /**
   * Starts the command when the trigger becomes inactive. The command is set to be interruptible.
   *
   * @param command the command to start
   * @return this trigger, so calls can be chained
   */
  public Trigger whenInactive(final Command command) {
    return whenInactive(command, true);
  }

  /**
   * Runs the given runnable when the trigger becomes inactive.
   *
   * @param toRun the runnable to run
   * @param requirements the required subsystems
   * @return this trigger, so calls can be chained
   */
  public Trigger whenInactive(final Runnable toRun, Subsystem... requirements) {
    // TODO perhaps replace with directly binding it if no requirements are given?
    return whenInactive(new InstantCommand(toRun, requirements));
  }

  /**
   * Toggles a command when the trigger becomes active.
   *
   * @param command the command to toggle
   * @param interruptible whether the command is interruptible
   * @return this trigger, so calls can be chained
   */
  public Trigger toggleWhenActive(final Command command, boolean interruptible) {
    requireNonNullParam(command, "command", "toggleWhenActive");

    this.rising()
        .whenTrue(
            () -> {
              if (command.isScheduled()) {
                command.cancel();
              } else {
                command.schedule(interruptible);
              }
            });

    return this;
  }

  /**
   * Toggles a command when the trigger becomes active. The command is set to be interruptible.
   *
   * @param command the command to toggle
   * @return this trigger, so calls can be chained
   */
  public Trigger toggleWhenActive(final Command command) {
    return toggleWhenActive(command, true);
  }

  /**
   * Cancels a command when the trigger becomes active.
   *
   * @param command the command to cancel
   * @return this trigger, so calls can be chained
   */
  public Trigger cancelWhenActive(final Command command) {
    requireNonNullParam(command, "command", "cancelWhenActive");

    this.rising().whenTrue(command::cancel);

    return this;
  }

  /* ----------- Super method type redeclarations ----------------- */

  @Override
  public Trigger and(BooleanEvent trigger) {
    return cast(super.and(trigger));
  }

  @Override
  public Trigger or(BooleanEvent trigger) {
    return cast(super.or(trigger));
  }

  @Override
  public Trigger negate() {
    return cast(super.negate());
  }

  @Override
  public Trigger debounce(double seconds) {
    return debounce(seconds, Debouncer.DebounceType.kRising);
  }

  @Override
  public Trigger debounce(double seconds, Debouncer.DebounceType type) {
    return cast(super.debounce(seconds, type));
  }

  @Override
  public Trigger rising() {
    return cast(super.rising());
  }

  @Override
  public Trigger falling() {
    return cast(super.falling());
  }
}
