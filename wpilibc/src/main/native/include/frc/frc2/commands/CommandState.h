#pragma once

namespace frc {
namespace frc2 {
class CommandState final {
 public:
  CommandState() = default;
  explicit CommandState(bool interruptible) : m_interruptible{interruptible} {
    StartTiming();
    StartRunning();
  }

  void StartTiming();
  void StartRunning();
  bool IsInterruptible() const { return m_interruptible; }
  double TimeSinceInitialized() const;
 private:
  double m_startTime = -1;
  bool m_interruptible;
};
}
}
