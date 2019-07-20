#include "CommandTestBase.h"
#include "frc/frc2/commands/InstantCommand.h"
#include "frc/frc2/commands/RunCommand.h"

using namespace frc::frc2;

class SchedulerTest : public CommandTestBase {

};

TEST_F(SchedulerTest, SchedulerLambdaTestNoInterrupt) {
  CommandScheduler scheduler = GetScheduler();

  InstantCommand command;

  int counter = 0;

  scheduler.OnCommandInitialize([&counter](const Command&){counter++;});
  scheduler.OnCommandExecute([&counter](const Command&){counter++;});
  scheduler.OnCommandFinish([&counter](const Command&){counter++;});

  scheduler.Schedule(&command);
  scheduler.Run();

  EXPECT_EQ(counter, 3);
}

TEST_F(SchedulerTest, SchedulerLambdaInterruptTest) {
  CommandScheduler scheduler = GetScheduler();

  RunCommand command([]{}, {});

  int counter = 0;

  scheduler.OnCommandInterrupt([&counter](const Command&){counter++;});

  scheduler.Schedule(&command);
  scheduler.Run();
  scheduler.Cancel(&command);

  EXPECT_EQ(counter, 1);
}

TEST_F(SchedulerTest, UnregisterSubsystemTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem system;

  scheduler.RegisterSubsystem(&system);
  
  EXPECT_NO_FATAL_FAILURE(scheduler.UnregisterSubsystem(&system));
}
