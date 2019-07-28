#include "frc/frc2/commands/NotifierCommand.h"

using namespace frc2;

NotifierCommand::NotifierCommand(std::function<void()> toRun, double period, std::initializer_list<Subsystem*> requirements) :
  m_toRun(toRun), m_notifier{std::move(toRun)}, m_period{period} {
  AddRequirements(requirements);
}

NotifierCommand::NotifierCommand(NotifierCommand&& other) 
  : CommandHelper(other), m_toRun(other.m_toRun), m_notifier(other.m_toRun), m_period(other.m_period) {}

void NotifierCommand::Initialize() {
  m_notifier.StartPeriodic(m_period);
}

void NotifierCommand::End(bool interrupted) {
  m_notifier.Stop();
}
