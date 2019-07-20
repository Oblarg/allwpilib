#pragma once

namespace frc {
namespace frc2 {
class Command;
class Subsystem {
 public:
  ~Subsystem();
  virtual void Periodic();
  virtual void SetDefaultCommand(Command* defaultCommand);
  virtual Command* GetDefaultCommand() const;
  virtual Command* GetCurrentCommand() const;
  virtual void Register();
};
}
}
