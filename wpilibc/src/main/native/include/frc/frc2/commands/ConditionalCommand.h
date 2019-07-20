#pragma once

#include "SendableCommandBase.h"
#include "CommandGroupBase.h"
#include "CommandHelper.h"
#include <iostream>

namespace frc {
namespace frc2 {
class ConditionalCommand : public CommandHelper<SendableCommandBase, ConditionalCommand> {
 public:
  template <class T1, class T2, 
    typename = std::enable_if_t<std::is_base_of<Command, T1>::value>,
    typename = std::enable_if_t<std::is_base_of<Command, T2>::value>>
  ConditionalCommand(T1&& onTrue, T2&& onFalse, std::function<bool()> condition)
    : ConditionalCommand(std::make_unique<T1>(std::forward<T1>(onTrue)),
      std::make_unique<T2>(std::forward<T2>(onFalse)),
      condition) {}

  ConditionalCommand(std::unique_ptr<Command>&& onTrue, std::unique_ptr<Command>&& onFalse, std::function<bool()> condition)
    : m_condition{std::move(condition)} {
    if (!CommandGroupBase::RequireUngrouped({onTrue.get(), onFalse.get()})) {
      return;
    }

    m_onTrue = std::move(onTrue);
    m_onFalse = std::move(onFalse);

    m_onTrue->SetGrouped(true);
    m_onFalse->SetGrouped(true);

    m_runsWhenDisabled &= m_onTrue->RunsWhenDisabled();
    m_runsWhenDisabled &= m_onFalse->RunsWhenDisabled();

    AddRequirements(m_onTrue->GetRequirements());
    AddRequirements(m_onFalse->GetRequirements());
  }

    ConditionalCommand(ConditionalCommand&& other) = default;

    //No copy constructors for command groups
    ConditionalCommand(const ConditionalCommand& other) = delete;
    
    void Initialize() override {
      if (m_condition()) {
        m_selectedCommand = m_onTrue.get();
      } else {
        m_selectedCommand = m_onFalse.get();
      }
      m_selectedCommand->Initialize();
    }
    
    void Execute() override {
      m_selectedCommand->Execute();
    }
    
    void End(bool interrupted) override {
      m_selectedCommand->End(interrupted);
    }
    
    bool IsFinished() override {
      return m_selectedCommand->IsFinished();
    }
    
    bool RunsWhenDisabled() const override {
      return m_runsWhenDisabled;
    }
 private:
  std::unique_ptr<Command> m_onTrue;
  std::unique_ptr<Command> m_onFalse;
  std::function<bool()> m_condition;
  Command* m_selectedCommand{nullptr};
  bool m_runsWhenDisabled = true;
};
}
}
