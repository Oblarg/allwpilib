// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj2.command.button;

import static edu.wpi.first.util.ErrorMessages.requireNonNullParam;

import edu.wpi.first.math.filter.Debouncer;
import edu.wpi.first.wpilibj2.command.button.BooleanChangeEvent.ChangeType;
import java.util.function.BooleanSupplier;

/**
 * This class provides an easy way to link actions to inputs. Each object represents a boolean
 * condition to which callback actions can be bound using {@link #whenTrue(Runnable)}.
 *
 * <p>These events can easily be composed using factories such as {@link #and}, {@link #or}, {@link
 * #negate} etc.
 *
 * <p>To get an event that activates only when this one changes, see {@link #falling()} and {@link
 * #rising()}.
 *
 * <p>If custom functionality is needed, overriding
 */
public class BooleanEvent implements BooleanSupplier {
  /** Poller loop */
  protected final EventLoop m_loop;
  /** Condition */
  protected final BooleanSupplier m_condition;

  /**
   * Creates a new event with the given condition determining whether it is active.
   *
   * @param loop the loop that polls this event
   * @param condition returns whether or not the event should be active
   */
  public BooleanEvent(EventLoop loop, BooleanSupplier condition) {
    m_loop = requireNonNullParam(loop, "loop", "BooleanEvent");
    m_condition = requireNonNullParam(condition, "condition", "BooleanEvent");
  }

  /**
   * Check whether this event is active or not.
   *
   * @return true if active.
   */
  public boolean get() {
    return m_condition.getAsBoolean();
  }

  @Override
  public final boolean getAsBoolean() {
    return get();
  }

  /**
   * Bind an action to this event.
   *
   * @param action the action to run if this event is active.
   */
  public final void whenTrue(Runnable action) {
    m_loop.bind(this, action);
  }

  /**
   * Get a new event that events only when this one newly changes to true.
   *
   * @return a new event representing when this one newly changes to true.
   */
  public BooleanEvent rising() {
    return new BooleanChangeEvent(m_loop, this, ChangeType.RISING);
  }

  /**
   * Get a new event that triggers only when this one newly changes to false.
   *
   * @return a new event representing when this one newly changes to false.
   */
  public BooleanEvent falling() {
    return new BooleanChangeEvent(m_loop, this, ChangeType.FALLING);
  }

  /**
   * Creates a new debounced event from this event - it will become active when this event has been
   * active for longer than the specified period.
   *
   * @param seconds The debounce period.
   * @return The debounced event (rising edges debounced only)
   */
  public BooleanEvent debounce(double seconds) {
    return debounce(seconds, Debouncer.DebounceType.kRising);
  }

  /**
   * Creates a new debounced event from this event - it will become active when this event has been
   * active for longer than the specified period.
   *
   * @param seconds The debounce period.
   * @param type The debounce type.
   * @return The debounced event.
   */
  public BooleanEvent debounce(double seconds, Debouncer.DebounceType type) {
    return new BooleanDebounceEvent(m_loop, this, seconds, type);
  }

  /**
   * Creates a new event that is active when this event is inactive, i.e. that acts as the negation
   * of this event.
   *
   * @return the negated event
   */
  public BooleanEvent negate() {
    return new BooleanEvent(m_loop, () -> !this.get());
  }

  /**
   * Composes this event with another event, returning a new event that is active when both events
   * are active.
   *
   * <p>The new event will use this event's polling loop.
   *
   * @param other the event to compose with
   * @return the event that is active when both events are active
   */
  public BooleanEvent and(BooleanEvent other) {
    return new BooleanEvent(m_loop, () -> this.get() && other.get());
  }

  /**
   * Composes this event with another event, returning a new event that is active when either event
   * is active.
   *
   * <p>The new event will use this event's polling loop.
   *
   * @param other the event to compose with
   * @return the event that is active when either event is active
   */
  public BooleanEvent or(BooleanEvent other) {
    return new BooleanEvent(m_loop, () -> this.get() || other.get());
  }
}
