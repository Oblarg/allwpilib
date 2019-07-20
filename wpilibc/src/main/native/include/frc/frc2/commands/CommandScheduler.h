#pragma once

#include <frc/smartdashboard/SendableBase.h>
#include <wpi/DenseMap.h>
#include <wpi/SmallSet.h>
#include <networktables/NetworkTableEntry.h>
#include <frc/RobotState.h>
#include <frc/ErrorBase.h>
#include <unordered_map>
#include "CommandState.h"

namespace frc2 {
class Command;
class Subsystem;
class CommandScheduler final : public frc::SendableBase, frc::ErrorBase {
 public:
  static CommandScheduler& GetInstance();
  using Action = std::function<void(const Command&)>;

  void AddButton(std::function<void()> button) {
    m_buttons.emplace_back(std::move(button));
  }

  void ClearButtons() {
    m_buttons.clear();
  }

  void Schedule(bool interruptible, Command* command);
  
  void Schedule(bool interruptible, wpi::ArrayRef<Command*> commands) {
    for (auto command : commands) {
      Schedule(interruptible, command);
    }
  }

    void Schedule(wpi::ArrayRef<Command*> commands) {
    for (auto command : commands) {
      Schedule(true, command);
    }
  }

  void Run();

  void RegisterSubsystem(Subsystem* subsystem);

  void UnregisterSubsystem(Subsystem* subsystem);

  void RegisterSubsystem(std::initializer_list<Subsystem*> subsystems);

  void UnregisterSubsystem(std::initializer_list<Subsystem*> subsystems);

  void SetDefaultCommand(Subsystem* subsystem, Command* defaultCommand);

  Command* GetDefaultCommand(const Subsystem* subsystem) const;

  void Cancel(Command* command);
  void Cancel(wpi::ArrayRef<Command*> commands) {
    for (auto command : commands) {
      Cancel(command);
    }
  }
  void CancelAll();

  double TimeSinceScheduled(const Command* command) const;
  bool IsScheduled(std::initializer_list<const Command*> commands) const;
  bool IsScheduled(const Command* command) const;

  Command* Requiring(const Subsystem* subsystem) const;

  void Disable() { m_disabled = true; }
  void Enable() { m_disabled = false; }

  void OnCommandInitialize(Action action) {
    m_initActions.emplace_back(std::move(action));
  }
  void OnCommandExecute(Action action) {
    m_executeActions.emplace_back(std::move(action));
  }
  void OnCommandInterrupt(Action action) {
    m_interruptActions.emplace_back(std::move(action));
  }
  void OnCommandFinish(Action action) {
    m_finishActions.emplace_back(std::move(action));
  }

  void InitSendable(frc::SendableBuilder& builder) override;

 private:
  CommandScheduler();

  wpi::DenseMap<Command*, CommandState> m_scheduledCommands;
  wpi::DenseMap<Subsystem*, Command*> m_requirements;
  wpi::DenseMap<Subsystem*, Command*> m_subsystems;
  wpi::SmallVector<std::function<void()>, 4> m_buttons;

  bool m_disabled{false};

  nt::NetworkTableEntry m_namesEntry;
  nt::NetworkTableEntry m_idsEntry;
  nt::NetworkTableEntry m_cancelEntry;

  wpi::SmallVector<Action, 4> m_initActions;
  wpi::SmallVector<Action, 4> m_executeActions;
  wpi::SmallVector<Action, 4> m_interruptActions;
  wpi::SmallVector<Action, 4> m_finishActions;

  friend class CommandTestBase;
};
}
