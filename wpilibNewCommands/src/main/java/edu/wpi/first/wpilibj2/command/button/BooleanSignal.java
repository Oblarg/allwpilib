package edu.wpi.first.wpilibj2.command.button;

import edu.wpi.first.math.filter.Debouncer;

import java.util.function.BooleanSupplier;

public interface BooleanSignal extends BooleanSupplier {
  default BooleanSignal rising() {
    return new BooleanSignal() {
      boolean m_previous;
      @Override
      public boolean getAsBoolean() {
        boolean present = m_previous;
        boolean ret = !m_previous && BooleanSignal.this.getAsBoolean();
        m_previous = present;
        return ret;
      }
    };
  }

  default BooleanSignal falling() {
    return new BooleanSignal() {
      boolean m_previous;
      @Override
      public boolean getAsBoolean() {
        boolean present = m_previous;
        boolean ret = m_previous && !BooleanSignal.this.getAsBoolean();
        m_previous = present;
        return ret;
      }
    };
  }

  default BooleanSignal changed() {
    return rising().or(falling());
  }

  default BooleanSignal debounce(double seconds, Debouncer.DebounceType type) {
    return new BooleanSignal() {
      Debouncer m_debouncer = new Debouncer(seconds, type);
      @Override
      public boolean getAsBoolean() {
        return m_debouncer.calculate(BooleanSignal.this.getAsBoolean());
      }
    };
  }

  default BooleanSignal debounce(double seconds) {
    return debounce(seconds, Debouncer.DebounceType.kRising);
  }

  default BooleanSignal and(BooleanSupplier other) {
    return () -> getAsBoolean() && other.getAsBoolean();
  }

  default BooleanSignal or(BooleanSupplier other) {
    return () -> getAsBoolean() || other.getAsBoolean();
  }

  default BooleanSignal negate() {
    return () -> !getAsBoolean();
  }
}
