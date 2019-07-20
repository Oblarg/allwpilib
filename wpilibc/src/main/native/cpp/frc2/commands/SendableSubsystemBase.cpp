#include <frc/frc2/commands/SendableSubsystemBase.h>
#include <frc/frc2/commands/Command.h>
#include <frc/frc2/commands/CommandScheduler.h>
#include <frc/smartdashboard/SendableBuilder.h>

using namespace frc::frc2;

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
