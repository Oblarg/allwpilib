#include "frc/frc2/commands/CommandState.h"
#include "frc/Timer.h"

using namespace frc2;
  CommandState::CommandState(bool interruptible) : m_interruptible{interruptible} {
    StartTiming();
    StartRunning();
  }

  void CommandState::StartTiming() {
      m_startTime = frc::Timer::GetFPGATimestamp();
  }
  void CommandState::StartRunning() {
      m_startTime = -1;
  }
  double CommandState::TimeSinceInitialized() const {
      return m_startTime != -1 ? frc::Timer::GetFPGATimestamp() - m_startTime : -1;
  }
