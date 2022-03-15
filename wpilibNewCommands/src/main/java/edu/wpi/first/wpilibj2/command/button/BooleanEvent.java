package edu.wpi.first.wpilibj2.command.button;

import edu.wpi.first.wpilibj2.command.button.BooleanChangeEvent.ChangeType;

import java.util.function.BooleanSupplier;

import static edu.wpi.first.util.ErrorMessages.requireNonNullParam;

public class BooleanEvent implements BooleanSupplier {
  protected final EventLoop m_loop;
  protected final BooleanSupplier m_condition;

  public BooleanEvent(EventLoop loop, BooleanSupplier condition) {
    m_loop = requireNonNullParam(loop, "loop", "BooleanEvent");
    m_condition = requireNonNullParam(condition, "condition", "BooleanEvent");
  }

  public boolean get() {
    return m_condition.getAsBoolean();
  }

  @Override
  public final boolean getAsBoolean() {
    return get();
  }

  public void whenTrue(Runnable action) {
    m_loop.bind(this, action);
  }

  public BooleanEvent rising() {
    return new BooleanChangeEvent(m_loop, this, ChangeType.RISING);
  }

  public BooleanEvent falling() {
    return new BooleanChangeEvent(m_loop, this, ChangeType.FALLING);
  }

  public BooleanEvent negate() {
    return new BooleanEvent(m_loop, () -> !this.get());
  }

  public BooleanEvent and(BooleanEvent other) {
    return new BooleanEvent(m_loop, () -> this.get() && other.get());
  }

  public BooleanEvent or(BooleanEvent other) {
    return new BooleanEvent(m_loop, () -> this.get() || other.get());
  }
}
