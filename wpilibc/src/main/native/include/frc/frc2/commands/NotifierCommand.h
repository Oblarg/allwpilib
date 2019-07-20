#pragma once

#include "SendableCommandBase.h"
#include "CommandHelper.h"
#include <frc/Notifier.h>

namespace frc {
namespace frc2 {
class NotifierCommand : public CommandHelper<SendableCommandBase, NotifierCommand> {
 public:
  NotifierCommand(std::function<void()> toRun, double period, std::initializer_list<Subsystem*> requirements) :
    m_toRun(toRun), m_notifier{std::move(toRun)}, m_period{period} {
      AddRequirements(requirements);
  }

  NotifierCommand(NotifierCommand&& other) 
    : CommandHelper(other), m_toRun(other.m_toRun), m_notifier(other.m_toRun), m_period(other.m_period) {};

  NotifierCommand(const NotifierCommand& other) = default;

  void Initialize() override {
    m_notifier.StartPeriodic(m_period);
  }
  
  void End(bool interrupted) override {
    m_notifier.Stop();
  }
 private:
  std::function<void()> m_toRun;
  Notifier m_notifier;
  double m_period;
};
}
}
