/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "CommandTestBase.h"
#include "frc/frc2/commands/RunCommand.h"

using namespace frc2;
class DefaultCommandTest : public CommandTestBase {

};

TEST_F(DefaultCommandTest, DefaultCommandScheduleTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem subsystem;

  RunCommand command1([]{}, {&subsystem});

  scheduler.SetDefaultCommand(&subsystem, &command1);
  scheduler.Run();

  EXPECT_TRUE(scheduler.IsScheduled(&command1));
}

TEST_F(DefaultCommandTest, DefaultCommandInterruptResumeTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem subsystem;

  RunCommand command1([]{}, {&subsystem});
  RunCommand command2([]{}, {&subsystem});

  scheduler.SetDefaultCommand(&subsystem, &command1);
  scheduler.Run();
  scheduler.Schedule(&command2);

  EXPECT_TRUE(scheduler.IsScheduled(&command2));
  EXPECT_FALSE(scheduler.IsScheduled(&command1));

  scheduler.Cancel(&command2);
  scheduler.Run();

  EXPECT_TRUE(scheduler.IsScheduled(&command1));
}
