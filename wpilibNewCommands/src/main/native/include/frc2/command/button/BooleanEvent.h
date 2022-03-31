// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <functional>
#include <memory>

#include <frc/filter/Debouncer.h>
#include <units/time.h>

#include "EventLoop.h"

class BooleanEvent {
 public:
  BooleanEvent(std::shared_ptr<EventLoop> loop,
               std::function<bool()> condition);
  virtual bool Get() const;
  void WhenTrue(std::function<void()> action);

  /**
   * Creates a new event that is active when this event is inactive, i.e. that
   * acts as the negation of this event.
   *
   * @return the negated event
   */
  BooleanEvent operator!();

  /**
   * Composes this event with another event, returning a new event that is
   * active when both events are active.
   *
   * <p>The new event will use this event's polling loop.
   *
   * @param other the event to compose with
   * @return the event that is active when both events are active
   */
  BooleanEvent operator&&(BooleanEvent rhs);

  /**
   * Composes this event with another event, returning a new event that is
   * active when either event is active.
   *
   * <p>The new event will use this event's polling loop.
   *
   * @param other the event to compose with
   * @return the event that is active when either event is active
   */
  BooleanEvent operator||(BooleanEvent rhs);

  /**
   * Get a new event that events only when this one newly changes to true.
   *
   * @return a new event representing when this one newly changes to true.
   */
  BooleanEvent Rising();

  /**
   * Get a new event that triggers only when this one newly changes to false.
   *
   * @return a new event representing when this one newly changes to false.
   */
  BooleanEvent Falling();

  /**
   * Creates a new debounced event from this event - it will become active when
   * this event has been active for longer than the specified period.
   *
   * @param seconds The debounce period.
   * @param type The debounce type.
   * @return The debounced event.
   */
  BooleanEvent Debounce(units::second_t debounceTime,
                        frc::Debouncer::DebounceType type =
                            frc::Debouncer::DebounceType::kRising);

 private:
  std::shared_ptr<EventLoop> m_loop;
  std::function<bool()> m_condition;
};
