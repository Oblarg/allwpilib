// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc2/command/button/BooleanEvent.h"

BooleanEvent::BooleanEvent(std::shared_ptr<EventLoop> loop,
                           std::function<bool()> condition)
    : m_loop(loop), m_condition(condition) {}

bool BooleanEvent::Get() const {
  return m_condition();
}

void BooleanEvent::WhenTrue(std::function<void()> action) {
  m_loop->Bind(m_condition, action);
}

BooleanEvent BooleanEvent::operator!() {
  return BooleanEvent(this->m_loop, [*this] { return !this.Get(); });
}
BooleanEvent BooleanEvent::operator&&(BooleanEvent rhs) {
  return BooleanEvent(this->m_loop,
                      [*this, rhs] { return this.Get() && rhs.Get(); });
}
BooleanEvent BooleanEvent::operator||(BooleanEvent rhs) {
  return BooleanEvent(this->m_loop,
                      [*this, rhs] { return this.Get() || rhs.Get(); });
}
BooleanEvent BooleanEvent::Rising() {
  return BooleanEvent(this->m_loop,
                      [*this, m_previous = this->Get()]() mutable {
                        bool present = this.Get();
                        bool past = m_previous;
                        m_previous = present;
                        return !past && present;
                      });
}
BooleanEvent BooleanEvent::Falling() {
  return BooleanEvent(this->m_loop,
                      [*this, m_previous = this->Get()]() mutable {
                        bool present = this.Get();
                        bool past = m_previous;
                        m_previous = present;
                        return past && !present;
                      });
}

BooleanEvent BooleanEvent::Debounce(units::second_t debounceTime,
                                    frc::Debouncer::DebounceType type) {
  return BooleanEvent(
      this->m_loop,
      [debouncer = frc::Debouncer(debounceTime, type), *this]() mutable {
        return debouncer.Calculate(this.Get());
      });
}
