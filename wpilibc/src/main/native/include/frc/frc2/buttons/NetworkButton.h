#pragma once
#include "Button.h"
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>
#include <networktables/NetworkTable.h>

namespace frc {
namespace frc2 {
class NetworkButton : public Button {
 public:
  NetworkButton(const wpi::Twine& table, const wpi::Twine& field) : NetworkButton{nt::NetworkTableInstance::GetDefault().GetTable(table), field} {}
  NetworkButton(const std::shared_ptr<nt::NetworkTable>& table, const wpi::Twine& field) : m_entry{table->GetEntry(field)} {}
  bool Get() const override { return m_entry.GetInstance().IsConnected() && m_entry.GetBoolean(false); }
 private:
  nt::NetworkTableEntry m_entry;
};
}
}
