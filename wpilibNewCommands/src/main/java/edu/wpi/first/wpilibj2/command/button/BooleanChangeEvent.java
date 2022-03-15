package edu.wpi.first.wpilibj2.command.button;

import java.util.function.BooleanSupplier;

import static edu.wpi.first.util.ErrorMessages.requireNonNullParam;

class BooleanChangeEvent extends BooleanEvent {
  private boolean m_previous;
  private final ChangeType m_type;

  public BooleanChangeEvent(EventLoop loop, BooleanSupplier condition, ChangeType type) {
    super(loop, condition);
    m_previous = condition.getAsBoolean();
    m_type = requireNonNullParam(type, "type", "BooleanChangeType");
  }

  @Override
  public boolean get() {
    boolean present = m_condition.getAsBoolean();

    boolean ret;
    switch (m_type) {
      case FALLING:
        ret = m_previous && !present;
        break;
      case RISING:
        ret = !m_previous && present;
        break;
      default:
        throw new NullPointerException("Null change type!");
    }

    m_previous = present;
    return ret;
  }

  enum ChangeType {
    FALLING, RISING
  }
}
