/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/frc2/commands/Command.h"

#include <iostream>

#include "frc/frc2/commands/CommandScheduler.h"
#include "frc/frc2/commands/InstantCommand.h"
#include "frc/frc2/commands/ParallelCommandGroup.h"
#include "frc/frc2/commands/ParallelDeadlineGroup.h"
#include "frc/frc2/commands/ParallelRaceGroup.h"
#include "frc/frc2/commands/PerpetualCommand.h"
#include "frc/frc2/commands/ProxyScheduleCommand.h"
#include "frc/frc2/commands/SequentialCommandGroup.h"
#include "frc/frc2/commands/WaitCommand.h"
#include "frc/frc2/commands/WaitUntilCommand.h"

#ifdef __GNUG__
#include <cxxabi.h>

#include <cstdlib>
#include <memory>
namespace frc {
namespace frc2 {
std::string demangle(const char* name) {
  int status = -1;
  std::unique_ptr<char, void (*)(void*)> res{
      abi::__cxa_demangle(name, NULL, NULL, &status), std::free};
  return (status == 0) ? res.get() : name;
}
}  // namespace frc2
}  // namespace frc
#else
namespace frc2 {
std::string demangle(const char* name) { return name; }
}  // namespace frc2
#endif

using namespace frc2;

Command::~Command() { CommandScheduler::GetInstance().Cancel(this); }

void Command::Initialize() {}
void Command::Execute() {}
void Command::End(bool interrupted) {}

ParallelRaceGroup Command::WithTimeout(double seconds) && {
  std::vector<std::unique_ptr<Command>> temp;
  temp.emplace_back(std::make_unique<WaitCommand>(seconds));
  temp.emplace_back(std::move(*this).TransferOwnership());
  return ParallelRaceGroup(std::move(temp));
}

ParallelRaceGroup Command::InterruptOn(std::function<bool()> condition) && {
  std::vector<std::unique_ptr<Command>> temp;
  temp.emplace_back(std::make_unique<WaitUntilCommand>(std::move(condition)));
  temp.emplace_back(std::move(*this).TransferOwnership());
  return ParallelRaceGroup(std::move(temp));
}

SequentialCommandGroup Command::BeforeStarting(std::function<void()> toRun) && {
  std::vector<std::unique_ptr<Command>> temp;
  temp.emplace_back(std::make_unique<InstantCommand>(
      std::move(toRun), std::initializer_list<Subsystem*>{}));
  temp.emplace_back(std::move(*this).TransferOwnership());
  return SequentialCommandGroup(std::move(temp));
}

SequentialCommandGroup Command::WhenFinished(std::function<void()> toRun) && {
  std::vector<std::unique_ptr<Command>> temp;
  temp.emplace_back(std::move(*this).TransferOwnership());
  temp.emplace_back(std::make_unique<InstantCommand>(
      std::move(toRun), std::initializer_list<Subsystem*>{}));
  return SequentialCommandGroup(std::move(temp));
}

PerpetualCommand Command::Perpetually() && {
  return PerpetualCommand(std::move(*this).TransferOwnership());
}

ProxyScheduleCommand Command::AsProxy() { return ProxyScheduleCommand(this); }

void Command::Schedule(bool interruptible) {
  CommandScheduler::GetInstance().Schedule(interruptible, this);
}

void Command::Cancel() { CommandScheduler::GetInstance().Cancel(this); }

bool Command::IsScheduled() const {
  return CommandScheduler::GetInstance().IsScheduled(this);
}

bool Command::HasRequirement(Subsystem* requirement) const {
  bool hasRequirement = false;
  for (auto&& subsystem : GetRequirements()) {
    hasRequirement |= requirement == subsystem;
  }
  return hasRequirement;
}

std::string Command::GetName() const { return GetTypeName(*this); }

bool Command::IsGrouped() const { return m_isGrouped; }

void Command::SetGrouped(bool grouped) { m_isGrouped = grouped; }
