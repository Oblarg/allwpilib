#pragma once

#include "SendableCommandBase.h"
#include "CommandGroupBase.h"
#include "CommandHelper.h"

namespace frc {
namespace frc2 {
class PerpetualCommand : public CommandHelper<SendableCommandBase, PerpetualCommand> {
 public:
  PerpetualCommand(std::unique_ptr<Command>&& command) {
      if (!CommandGroupBase::RequireUngrouped(command)) { 
        return; 
      }
      m_command = std::move(command);
      m_command->SetGrouped(true);
      AddRequirements(m_command->GetRequirements());
  }

  template<class T, 
    typename = std::enable_if_t<std::is_base_of<Command, std::remove_reference_t<T>>::value>>
  PerpetualCommand(T&& command) 
    : PerpetualCommand(std::make_unique<std::remove_reference_t<T>>(std::forward<T>(command))) {};

  PerpetualCommand(PerpetualCommand&& other) = default;

  //No copy constructors for command groups
  PerpetualCommand(const PerpetualCommand& other) = delete;
    
  void Initialize() override {
    m_command->Initialize();
  }
  
  void Execute() override {
    m_command->Execute();
  }
  
  void End(bool interrupted) override {
    m_command->End(interrupted);
  }
 private:
  std::unique_ptr<Command> m_command;
};
}
}
