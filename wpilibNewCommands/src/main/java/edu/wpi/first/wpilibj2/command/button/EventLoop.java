package edu.wpi.first.wpilibj2.command.button;

import java.util.ArrayList;
import java.util.List;
import java.util.function.BooleanSupplier;

public class EventLoop {
  private final List<Binding> m_bindings = new ArrayList<>();

  public void bind(BooleanSupplier condition, Runnable action) {
    m_bindings.add(new Binding(condition, action));
  }

  public void poll() {
    m_bindings.forEach(Binding::poll);
  }

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
