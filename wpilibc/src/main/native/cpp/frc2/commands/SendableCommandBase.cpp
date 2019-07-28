#include <frc/frc2/commands/SendableCommandBase.h>
#include <frc/smartdashboard/SendableBuilder.h>
#include <frc/frc2/commands/SetUtilities.h>
#include <frc/frc2/commands/CommandScheduler.h>

using namespace frc2;

SendableCommandBase::SendableCommandBase() {
  m_name = Command::GetName();
  m_subsystem = "Unknown";
}

SendableCommandBase::SendableCommandBase(const SendableCommandBase& other) {
  m_name = other.m_name;
  m_subsystem = other.m_subsystem;
  m_requirements = other.m_requirements;
}

void SendableCommandBase::AddRequirements(std::initializer_list<Subsystem*> requirements) {
  m_requirements.insert(requirements.begin(), requirements.end());
}

void SendableCommandBase::AddRequirements(wpi::SmallSet<Subsystem*, 4> requirements) {
  m_requirements.insert(requirements.begin(), requirements.end());
}

wpi::SmallSet<Subsystem*, 4> SendableCommandBase::GetRequirements() const {
  return m_requirements;
}

void SendableCommandBase::InitSendable(frc::SendableBuilder& builder) {
  builder.SetSmartDashboardType("Command");
  builder.AddStringProperty(".name", [this]{return GetName();}, nullptr);
  builder.AddBooleanProperty("running", [this] {return IsScheduled();},
  [this](bool value){
    bool isScheduled = IsScheduled();
    if (value && !isScheduled) {
      CommandScheduler::GetInstance().QueueSchedule(this);
    } else if (!value && isScheduled) {
      CommandScheduler::GetInstance().QueueCancel(this);
    }
  });
}
