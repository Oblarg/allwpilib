#pragma once

#include <unordered_map>

#include "SendableCommandBase.h"
#include "CommandGroupBase.h"
#include "PrintCommand.h"

namespace frc {
namespace frc2 {
template <typename Key>

class SelectCommand : public CommandHelper<SendableCommandBase, SelectCommand<Key>> {
 public:
  template <class... Types, typename = std::enable_if_t<std::conjunction_v<std::is_base_of<Command, std::remove_reference_t<Types>>...>>>
  SelectCommand(std::function<Key()> selector, std::pair<Key, Types>... commands) 
    : m_selector{std::move(selector)} {
    std::vector<std::pair<Key, std::unique_ptr<Command>>> foo;

    ((void)foo.emplace_back(commands.first, std::make_unique<std::remove_reference_t<Types>>(std::move(commands.second))), ...);

    for(auto&& command : foo) {
      if (!CommandGroupBase::RequireUngrouped(command.second)) {
        return;
      }
    }

    for (auto&& command : foo) {
      this->AddRequirements(command.second->GetRequirements());
      m_runsWhenDisabled &= command.second->RunsWhenDisabled();
      m_commands.emplace(std::move(command.first), std::move(command.second));
    }
  }

  SelectCommand(std::function<Key()> selector, std::vector<std::pair<Key, std::unique_ptr<Command>>>&& commands) 
    : m_selector{std::move(selector)} {
    for (auto&& command : commands) {
      if (!CommandGroupBase::RequireUngrouped(command.second)) {
        return;
      }
    }

    for (auto&& command : commands) {
      this->AddRequirements(command.second->GetRequirements());
      m_runsWhenDisabled &= command.second->RunsWhenDisabled();
      m_commands.emplace(std::move(command.first), std::move(command.second));
    }
  }

  //No copy constructors for command groups
  SelectCommand(const SelectCommand& other) = delete;
  
  SelectCommand(std::function<Command*()> toRun) 
    : m_toRun{toRun} {
  }

  SelectCommand(SelectCommand&& other) = default;
  
  void Initialize() override;
  
  void Execute() override {
    m_selectedCommand->Execute();
  }
  
  void End(bool interrupted) override {
    return m_selectedCommand->End(interrupted);
  }
  
  bool IsFinished() override {
    return m_selectedCommand->IsFinished();
  }
  
  bool RunsWhenDisabled() const override {
    return m_runsWhenDisabled;
  }
 protected:
  std::unique_ptr<Command> TransferOwnership()&& override {
    return std::make_unique<SelectCommand>(std::move(*this));
  }
 private:
  std::unordered_map<Key, std::unique_ptr<Command>> m_commands;
  std::function<Key()> m_selector;
  std::function<std::unique_ptr<Command>()> m_toRun;
  Command* m_selectedCommand;
  bool m_runsWhenDisabled = true;
};

template <typename T>
void SelectCommand<T>::Initialize() {
  if (m_selector) {
    auto find = m_commands.find(m_selector());
    if (find == m_commands.end()) {
      m_selectedCommand = new PrintCommand("SelectCommand selector value does not correspond to any command!");
      return;
    }
    m_selectedCommand = find->second.get();
  } else {
    m_selectedCommand = m_toRun().get();
  }
  m_selectedCommand->Initialize();
}

}
}
