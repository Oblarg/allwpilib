// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj2.command.button;

import java.util.Collection;
import java.util.LinkedHashSet;
import java.util.function.BooleanSupplier;

/** The loop polling {@link BooleanEvent} objects and executing the actions bound to them. */
public final class EventLoop {
  private final Collection<Binding> m_bindings = new LinkedHashSet<>();

  /**
   * Bind a new action to run whenever the condition is true.
   *
   * @param condition the condition to listen to.
   * @param action the action to run.
   */
  public void bind(BooleanSupplier condition, Runnable action) {
    m_bindings.add(new Binding(condition, action));
  }

  /** Poll all bindings. */
  public void poll() {
    m_bindings.forEach(Binding::poll);
  }

  /** Clear all bindings. */
  public void clear() {
    m_bindings.clear();
  }

  private static class Binding {
    final BooleanSupplier condition;
    final Runnable action;

    private Binding(BooleanSupplier condition, Runnable action) {
      this.condition = condition;
      this.action = action;
    }

    void poll() {
      if (condition.getAsBoolean()) {
        action.run();
      }
    }
  }
}
