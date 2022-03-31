// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc2/command/button/EventLoop.h"

EventLoop::EventLoop() {}

struct EventLoop::Binding {
  std::function<bool()> condition;
  std::function<void()> action;

  void Poll() const {
    if (condition()) {
      action();
    }
  }
};

void EventLoop::Bind(std::function<bool()> condition,
                     std::function<void()> action) {
  m_bindings.push_back(Binding{condition, action});
}

void EventLoop::Poll() const {
  for (const Binding& binding : m_bindings) {
    binding.Poll();
  }
}

void EventLoop::Clear() {
  m_bindings.clear();
}
