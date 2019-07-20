#pragma once

#include "SendableCommandBase.h"
#include "CommandHelper.h"
#include <frc/Notifier.h>

namespace frc {
namespace experimental {
class NotifierCommand : public CommandHelper<SendableCommandBase, NotifierCommand> {
 public:
  NotifierCommand(std::function<void()> toRun, double period, std::initializer_list<Subsystem*> requirements) :
    m_notifier{std::move(toRun)}, m_period{period} {
      AddRequirements(requirements);
  }

  NotifierCommand(NotifierCommand&& other) = default;

  NotifierCommand(const NotifierCommand& other) = default;
    
  void Initialize() override {
    m_notifier.StartPeriodic(m_period);
  }
  
  void End(bool interrupted) override {
    m_notifier.Stop();
  }
 private:
  Notifier m_notifier;
  double m_period;
};
}
}
