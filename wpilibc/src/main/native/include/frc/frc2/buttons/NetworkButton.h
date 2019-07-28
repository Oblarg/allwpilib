/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>

#include "Button.h"

namespace frc2 {
class NetworkButton : public Button {
 public:
  NetworkButton(const wpi::Twine& table, const wpi::Twine& field) : NetworkButton{nt::NetworkTableInstance::GetDefault().GetTable(table), field} {}
  NetworkButton(const std::shared_ptr<nt::NetworkTable>& table, const wpi::Twine& field) : m_entry{table->GetEntry(field)} {}
  bool Get() const override { return m_entry.GetInstance().IsConnected() && m_entry.GetBoolean(false); }
 private:
  nt::NetworkTableEntry m_entry;
};
}  // namespace frc2
