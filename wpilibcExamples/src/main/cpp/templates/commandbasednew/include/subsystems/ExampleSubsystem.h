#pragma once

#include <frc/frc2/commands/SendableSubsystemBase.h>

class ExampleSubsystem : public frc2::SendableSubsystemBase {
 public:
  ExampleSubsystem();

  /**
   * Will be called periodically whenever the CommandScheduler runs.
   */
  void Periodic() override;

  //Subsystem methods go here.
  
 private:
  // Components (e.g. motor controllers and sensors) should generally be declared private and exposed only through public methods.
};