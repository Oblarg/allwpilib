// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package edu.wpi.first.wpilibj2.command.button;

import static edu.wpi.first.util.ErrorMessages.requireNonNullParam;

import java.util.function.BooleanSupplier;

// TODO: should this be an anonymous class inside BooleanEvent instead?
class BooleanChangeEvent extends BooleanEvent {
  private boolean m_previous;
  private final ChangeType m_type;

  public BooleanChangeEvent(EventLoop loop, BooleanSupplier condition, ChangeType type) {
    super(loop, condition);
    m_previous = condition.getAsBoolean();
    m_type = requireNonNullParam(type, "type", "BooleanChangeEvent");
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
    FALLING,
    RISING
  }
}
