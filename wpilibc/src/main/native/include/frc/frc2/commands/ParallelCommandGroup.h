#pragma once

#include "CommandGroupBase.h"
#include "CommandHelper.h"
#include <unordered_map>

namespace frc2 {
/**
 * A CommandGroup that runs a set of commands in parallel, ending when the last command ends.
 *
 * <p>As a rule, CommandGroups require the union of the requirements of their component commands.
 */
class ParallelCommandGroup : public CommandHelper<CommandGroupBase, ParallelCommandGroup> {
 public:
  /**
   * Creates a new ParallelCommandGroup.  The given commands will be executed simultaneously.
   * The command group will finish when the last command finishes.  If the CommandGroup is
   * interrupted, only the commands that are still running will be interrupted.
   *
   * @param commands the commands to include in this group.
   */
  ParallelCommandGroup(std::vector<std::unique_ptr<Command>>&& commands);

  /**
   * Creates a new ParallelCommandGroup.  The given commands will be executed simultaneously.
   * The command group will finish when the last command finishes.  If the CommandGroup is
   * interrupted, only the commands that are still running will be interrupted.
   *
   * @param commands the commands to include in this group.
   */
  template <class... Types, typename = std::enable_if_t<std::conjunction_v<std::is_base_of<Command, std::remove_reference_t<Types>>...>>>
  ParallelCommandGroup(Types&&... commands) {
    AddCommands(std::forward<Types>(commands)...);
  }

  ParallelCommandGroup(ParallelCommandGroup&& other) = default;

  //No copy constructors for commandgroups
  ParallelCommandGroup(const ParallelCommandGroup&) = delete;

  template <class... Types>
  void AddCommands(Types&&... commands) {
    std::vector<std::unique_ptr<Command>> foo;
    ((void)foo.emplace_back(std::make_unique<std::remove_reference_t<Types>>(std::forward<Types>(commands))), ...);
    AddCommands(std::move(foo));
  }
  
  void Initialize() override;
  
  void Execute() override;
  
  void End(bool interrupted) override;
  
  bool IsFinished() override;
  
  bool RunsWhenDisabled() const override;

 private:
  void AddCommands(std::vector<std::unique_ptr<Command>>&& commands) override;

  std::unordered_map<std::unique_ptr<Command>, bool> m_commands;
  bool m_runWhenDisabled{true};
  bool isRunning = false;
};
}
