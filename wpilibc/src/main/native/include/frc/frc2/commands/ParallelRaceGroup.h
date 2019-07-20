#pragma once

#include "CommandGroupBase.h"
#include "CommandHelper.h"
#include <unordered_map>

namespace frc2 {
class ParallelRaceGroup : public CommandHelper<CommandGroupBase, ParallelRaceGroup> {
 public:
  ParallelRaceGroup(std::vector<std::unique_ptr<Command>>&& commands) {
    AddCommands(std::move(commands));
  }

  template <class... Types, typename = std::enable_if_t<std::conjunction_v<std::is_base_of<Command, std::remove_reference_t<Types>>...>>>
  ParallelRaceGroup(Types&&... commands) {
    AddCommands(std::forward<Types>(commands)...);
  }

  ParallelRaceGroup(ParallelRaceGroup&& other) = default;

  //No copy constructors for command groups
  ParallelRaceGroup(const ParallelRaceGroup&) = delete;
  
  template <class... Types>
  void AddCommands(Types&&... commands) {
    std::vector<std::unique_ptr<Command>> foo;
    ((void)foo.emplace_back(std::make_unique<std::remove_reference_t<Types>>(std::forward<Types>(commands))), ...);
    AddCommands(std::move(foo));
  }

  void Initialize() override {
    for (auto& commandRunning : m_commands) {
      commandRunning->Initialize();
    }
    isRunning = true;
  }
  
  void Execute() override {
    for (auto& commandRunning : m_commands) {
      commandRunning->Execute();
      if (commandRunning->IsFinished()) {
        m_finished = true;
        commandRunning->End(false);
      }
    }
  }

  void End(bool interrupted) override {
    for (auto& commandRunning : m_commands) {
      if (!commandRunning->IsFinished()) {
        commandRunning->End(true);
      }
    }
    isRunning = false;
  }
  
  bool IsFinished() override {
    return m_finished;
  }
  
  bool RunsWhenDisabled() const override {
    return m_runWhenDisabled;
  }
 private:
  void AddCommands(std::vector<std::unique_ptr<Command>>&& commands) override {
    if (!RequireUngrouped(commands)) {
      return;
    }

    if (isRunning) {
      wpi_setWPIErrorWithContext(CommandIllegalUse,
        "Commands cannot be added to a CommandGroup while the group is running");
    }
    
    for (auto&& command : commands) {
      if(RequirementsDisjoint(this, command.get())) {
        command->SetGrouped(true);
        AddRequirements(command->GetRequirements());
        m_runWhenDisabled &= command->RunsWhenDisabled();
        m_commands.emplace(std::move(command));
      }
      else {
        wpi_setWPIErrorWithContext(CommandIllegalUse, 
          "Multiple commands in a parallel group cannot require the same subsystems");
        return;
      }
    }
  }

  std::set<std::unique_ptr<Command>> m_commands;
  bool m_runWhenDisabled{true};
  bool m_finished{false};
  bool isRunning = false;
};
}
