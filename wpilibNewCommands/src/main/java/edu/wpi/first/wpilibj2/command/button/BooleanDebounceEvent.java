package edu.wpi.first.wpilibj2.command.button;

import static edu.wpi.first.util.ErrorMessages.requireNonNullParam;

import edu.wpi.first.math.filter.Debouncer;
import java.util.function.BooleanSupplier;

// TODO: should this be an anonymous class inside BooleanEvent instead?
class BooleanDebounceEvent extends BooleanEvent {
  private final Debouncer m_debouncer;

  public BooleanDebounceEvent(
      EventLoop loop,
      BooleanSupplier condition,
      double debounceSeconds,
      Debouncer.DebounceType type) {
    super(loop, condition);
    m_debouncer =
        new Debouncer(debounceSeconds, requireNonNullParam(type, "type", "BooleanDebounceEvent"));
  }

  @Override
  public boolean get() {
    return m_debouncer.calculate(m_condition.getAsBoolean());
  }
}
