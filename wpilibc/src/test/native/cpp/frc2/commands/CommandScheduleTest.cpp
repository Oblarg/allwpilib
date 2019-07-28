/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "CommandTestBase.h"
#include "frc/frc2/commands/FunctionalCommand.h"

using namespace frc2;
class CommandScheduleTest : public CommandTestBase {

};

TEST_F(CommandScheduleTest, InstantScheduleTest) {
  CommandScheduler scheduler = GetScheduler();
  MockCommand command;

  EXPECT_CALL(command, Initialize());
  EXPECT_CALL(command, Execute());
  EXPECT_CALL(command, End(false));

  command.SetFinished(true);
  scheduler.Schedule(&command);
  EXPECT_TRUE(scheduler.IsScheduled(&command));
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
}

TEST_F(CommandScheduleTest, SingleIterationScheduleTest) {
  CommandScheduler scheduler = GetScheduler();
  MockCommand command;

  EXPECT_CALL(command, Initialize());
  EXPECT_CALL(command, Execute()).Times(2);
  EXPECT_CALL(command, End(false));

  scheduler.Schedule(&command);
  EXPECT_TRUE(scheduler.IsScheduled(&command));
  scheduler.Run();
  command.SetFinished(true);
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
}

TEST_F(CommandScheduleTest, MultiScheduleTest) {
  CommandScheduler scheduler = GetScheduler();
  MockCommand command1;
  MockCommand command2;
  MockCommand command3;

  EXPECT_CALL(command1, Initialize());
  EXPECT_CALL(command1, Execute()).Times(2);
  EXPECT_CALL(command1, End(false));

  EXPECT_CALL(command2, Initialize());
  EXPECT_CALL(command2, Execute()).Times(3);
  EXPECT_CALL(command2, End(false));

  EXPECT_CALL(command3, Initialize());
  EXPECT_CALL(command3, Execute()).Times(4);
  EXPECT_CALL(command3, End(false));

  scheduler.Schedule(&command1);
  scheduler.Schedule(&command2);
  scheduler.Schedule(&command3);
  EXPECT_TRUE(scheduler.IsScheduled({&command1, &command2, &command3}));
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled({&command1, &command2, &command3}));
  command1.SetFinished(true);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled({&command2, &command3}));
  EXPECT_FALSE(scheduler.IsScheduled(&command1));
  command2.SetFinished(true);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command3));
  EXPECT_FALSE(scheduler.IsScheduled({&command1, &command2}));
  command3.SetFinished(true);
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled({&command1, &command2, &command3}));
}

TEST_F(CommandScheduleTest, SchedulerCancelTest) {
  CommandScheduler scheduler = GetScheduler();
  MockCommand command;

  EXPECT_CALL(command, Initialize());
  EXPECT_CALL(command, Execute());
  EXPECT_CALL(command, End(false)).Times(0);
  EXPECT_CALL(command, End(true));


  scheduler.Schedule(&command);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command));
  scheduler.Cancel(&command);
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
}

TEST_F(CommandScheduleTest, NotScheduledCancelTest) {
  CommandScheduler scheduler = GetScheduler();
  MockCommand command;

  EXPECT_NO_FATAL_FAILURE(scheduler.Cancel(&command));
}
