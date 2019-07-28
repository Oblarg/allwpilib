#include "frc/frc2/commands/WaitUntilCommand.h"

using namespace frc2;

WaitUntilCommand::WaitUntilCommand(std::function<bool()> condition) 
  : m_condition{std::move(condition)} {}

WaitUntilCommand::WaitUntilCommand(double time) 
  : m_condition{[=]{ return frc::Timer::GetMatchTime() - time > 0; }} {}

bool WaitUntilCommand::IsFinished() {
  return m_condition();
}

bool WaitUntilCommand::RunsWhenDisabled() const {
  return true;
}