#pragma once

namespace frc2 {
/**
 * Class that holds scheduling state for a command.  Used internally by the
 * CommandScheduler
 */
class CommandState final {
 public:
  CommandState() = default;

  explicit CommandState(bool interruptible);

  bool IsInterruptible() const { return m_interruptible; }

  //The time since this command was initialized.
  double TimeSinceInitialized() const;
 private:
  double m_startTime = -1;
  bool m_interruptible;

  void StartTiming();
  void StartRunning();
};
}
