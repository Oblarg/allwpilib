#include "CommandTestBase.h"
#include "frc/experimental/command/RunCommand.h"

using namespace frc::experimental;

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