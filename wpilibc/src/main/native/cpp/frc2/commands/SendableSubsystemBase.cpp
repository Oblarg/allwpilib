/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/frc2/commands/SendableSubsystemBase.h"

#include <frc/frc2/commands/Command.h>
#include <frc/frc2/commands/CommandScheduler.h>
#include <frc/smartdashboard/SendableBuilder.h>

using namespace frc2;

SendableSubsystemBase::SendableSubsystemBase() {
  m_name = GetTypeName(*this);
  CommandScheduler::GetInstance().RegisterSubsystem({this});
}

void SendableSubsystemBase::InitSendable(frc::SendableBuilder& builder) {
  builder.SetSmartDashboardType("Subsystem");
  builder.AddBooleanProperty(".hasDefault", [this] { return GetDefaultCommand() != nullptr; }, nullptr);
  builder.AddStringProperty(".default", [this]()-> std::string {
    auto command = GetDefaultCommand();
    if (command == nullptr) return "none";
    return command->GetName();
   }, nullptr);
  builder.AddBooleanProperty(".hasCommand", [this] { return GetCurrentCommand() != nullptr; }, nullptr);
  builder.AddStringProperty(".command", [this]()-> std::string {
    auto command = GetCurrentCommand();
    if (command == nullptr) return "none";
    return command->GetName();
   }, nullptr);
}

std::string SendableSubsystemBase::GetName() const { return m_name; }

void SendableSubsystemBase::SetName(const wpi::Twine& name) { m_name = name.str(); }

std::string SendableSubsystemBase::GetSubsystem() const { return GetName(); }

void SendableSubsystemBase::SetSubsystem(const wpi::Twine& name) { SetName(name); }
