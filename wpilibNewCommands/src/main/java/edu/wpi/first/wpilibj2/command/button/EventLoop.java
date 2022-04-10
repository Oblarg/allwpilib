// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj2.command.button;

import java.util.Collection;
import java.util.LinkedHashSet;
import java.util.Optional;
import java.util.function.BooleanSupplier;
import java.util.function.Consumer;
import java.util.function.Supplier;

/** The loop polling {@link UnitEvent} objects and executing the actions bound to them. */
public final class EventLoop {
  private final Collection<Binding> m_bindings = new LinkedHashSet<>();

  /**
   * Bind a new action to run whenever the condition is true.
   *
   * @param trigger the condition to listen to.
   * @param action the action to run.
   */
  public void bind(BooleanSupplier trigger, Runnable action) {
    m_bindings.add(new Binding<Unit>(
        () -> trigger.getAsBoolean() ? Optional.of(Unit.INSTANCE) : Optional.empty(),
        __ -> action.run()));
  }

  public <Payload> void bind(Supplier<Optional<Payload>> trigger, Consumer<Payload> action) {
    m_bindings.add(new Binding<Payload>(trigger, action));
  }

  /** Poll all bindings. */
  public void poll() {
    m_bindings.forEach(Binding::poll);
  }

  /** Clear all bindings. */
  public void clear() {
    m_bindings.clear();
  }

  private static class Binding<Payload> {
    private final Supplier<Optional<Payload>> m_trigger;
    private final Consumer<Payload> m_action;

    private Binding(Supplier<Optional<Payload>> trigger, Consumer<Payload> action) {
      m_trigger = trigger;
      m_action = action;
    }

    void poll() {
      if (m_trigger.get().isPresent()) {
        m_action.accept(m_trigger.get().get());
      }
    }
  }

  private static enum Unit {
    INSTANCE
  }
}
