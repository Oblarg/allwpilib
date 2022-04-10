// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj2.command.button;

import static edu.wpi.first.util.ErrorMessages.requireNonNullParam;

import edu.wpi.first.math.filter.Debouncer;

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
 */
public class UnitEvent implements BooleanSupplier {
  /** Poller loop. */
  protected final EventLoop m_loop;
  /** Condition. */
  protected final BooleanSignal m_trigger;

  /**
   * Creates a new event with the given condition determining whether it is active.
   *
   * @param loop the loop that polls this event
   * @param condition returns whether or not the event should be active
   */
  public UnitEvent(EventLoop loop, BooleanSupplier condition) {
    m_loop = requireNonNullParam(loop, "loop", "BooleanEvent");
    m_trigger = requireNonNullParam(condition, "condition", "BooleanEvent")::getAsBoolean;
  }

  @Override
  public final boolean getAsBoolean() {
    return m_trigger.getAsBoolean();
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
  public UnitEvent rising() {
    return new UnitEvent(m_loop, m_trigger.rising());
  }

  /**
   * Get a new event that triggers only when this one newly changes to false.
   *
   * @return a new event representing when this one newly changes to false.
   */
  public UnitEvent falling() {
    return new UnitEvent(m_loop, m_trigger.falling());
  }

  public UnitEvent changed() {
    return new UnitEvent(m_loop, m_trigger.changed());
  }

  /**
   * Creates a new debounced event from this event - it will become active when this event has been
   * active for longer than the specified period.
   *
   * @param seconds The debounce period.
   * @return The debounced event (rising edges debounced only)
   */
  public UnitEvent debounce(double seconds) {
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
  public UnitEvent debounce(double seconds, Debouncer.DebounceType type) {
    return new UnitEvent(m_loop, m_trigger.debounce(seconds, type));
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
  public UnitEvent and(BooleanSupplier other) {
    return new UnitEvent(m_loop, m_trigger.and(other));
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
  public UnitEvent or(BooleanSupplier other) {
    return new UnitEvent(m_loop, m_trigger.or(other));
  }

  /**
   * Creates a new event that is active when this event is inactive, i.e. that acts as the negation
   * of this event.
   *
   * @return the negated event
   */
  public UnitEvent negate() {
    return new UnitEvent(m_loop, m_trigger.negate());
  }
}
