#pragma once

#include "CommandGroupBase.h"
#include "CommandHelper.h"
#include <wpi/ArrayRef.h>
#include "frc/WPIErrors.h"
#include "frc/ErrorBase.h"

namespace frc {
namespace experimental {
class SequentialCommandGroup : public CommandHelper<CommandGroupBase, SequentialCommandGroup> {
 public:
  SequentialCommandGroup(std::vector<std::unique_ptr<Command>>&& commands) {
    AddCommands(std::move(commands));
  }

  template <class... Types, typename = std::enable_if_t<std::conjunction_v<std::is_base_of<Command, std::remove_reference_t<Types>>...>>>
  SequentialCommandGroup(Types&&... commands) {
    AddCommands(std::forward<Types>(commands)...);
  }

  SequentialCommandGroup(SequentialCommandGroup&& other) = default;
  
  //No copy constructors for command groups
  SequentialCommandGroup(const SequentialCommandGroup&) = delete;

  template <class... Types, typename = std::enable_if_t<std::conjunction_v<std::is_base_of<Command, std::remove_reference_t<Types>>...>>>
  void AddCommands(Types&&... commands) {
    std::vector<std::unique_ptr<Command>> foo;
    ((void)foo.emplace_back(std::make_unique<std::remove_reference_t<Types>>(std::forward<Types>(commands))), ...);
    AddCommands(std::move(foo));
  }
  
  void Initialize() override {
    m_currentCommandIndex = 0;
    
    if (!m_commands.empty()) {
      m_commands[0]->Initialize();
    }
  }
  
  void Execute() override {    
    if (m_commands.empty()) return;
    
    auto& currentCommand = m_commands[m_currentCommandIndex];
    
    currentCommand->Execute();
    if (currentCommand->IsFinished()) {
      currentCommand->End(false);
      m_currentCommandIndex++;
      if (m_currentCommandIndex < m_commands.size()) {
        m_commands[m_currentCommandIndex]->Initialize();
      }
    }
  }
  
  void End(bool interrupted) override {
    if (interrupted && !m_commands.empty()) {
      m_commands[m_currentCommandIndex]->End(interrupted);
    }
    m_currentCommandIndex = -1;
  }
  
  bool IsFinished() override {
    return m_currentCommandIndex == m_commands.size();
  }
  
  bool RunsWhenDisabled() const override {
    return m_runWhenDisabled;
  }
 private:
  void AddCommands(std::vector<std::unique_ptr<Command>>&& commands) final {
    if (!RequireUngrouped(commands)) {
      return;
    }
    
    if (m_currentCommandIndex != -1) {
      wpi_setWPIErrorWithContext(CommandIllegalUse, 
          "Commands cannot be added to a CommandGroup while the group is running");
    }
        
    for(auto&& command : commands) {
      command->SetGrouped(true);
      AddRequirements(command->GetRequirements());
      m_runWhenDisabled &= command->RunsWhenDisabled();
      m_commands.emplace_back(std::move(command));
    }
  }

  wpi::SmallVector<std::unique_ptr<Command>, 4> m_commands;
  int m_currentCommandIndex{-1};
  bool m_runWhenDisabled{true};
};
}
}
