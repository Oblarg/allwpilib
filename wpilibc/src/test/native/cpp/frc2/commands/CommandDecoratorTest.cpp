#include "CommandTestBase.h"
#include "frc/frc2/commands/RunCommand.h"
#include "frc/frc2/commands/InstantCommand.h"
#include "frc/frc2/commands/ParallelRaceGroup.h"
#include "frc/frc2/commands/SequentialCommandGroup.h"
#include "frc/frc2/commands/PerpetualCommand.h"

using namespace frc2;
class CommandDecoratorTest : public CommandTestBase {
  
};

TEST_F(CommandDecoratorTest, WithTimeoutTest) {
  CommandScheduler scheduler = GetScheduler();

  auto command = RunCommand([]{}, {}).WithTimeout(.1);

  scheduler.Schedule(&command);

  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command));

  std::this_thread::sleep_for(std::chrono::milliseconds(150));

  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
}

TEST_F(CommandDecoratorTest, InterruptOnTest) {
  CommandScheduler scheduler = GetScheduler();

  bool finished = false;

  auto command = RunCommand([]{}, {}).InterruptOn([&finished]{return finished;});

  scheduler.Schedule(&command);

  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command));

  finished = true;

  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
}

TEST_F(CommandDecoratorTest, BeforeStartingTest) {
  CommandScheduler scheduler = GetScheduler();

  bool finished = false;

  auto command = InstantCommand([]{}, {}).BeforeStarting([&finished]{finished = true;});

  scheduler.Schedule(&command);

  EXPECT_TRUE(finished);

  scheduler.Run();
  scheduler.Run();
  
  EXPECT_FALSE(scheduler.IsScheduled(&command));
}

TEST_F(CommandDecoratorTest, WhenFinishedTest) {
  CommandScheduler scheduler = GetScheduler();

  bool finished = false;

  auto command = InstantCommand([]{}, {}).WhenFinished([&finished]{finished = true;});

  scheduler.Schedule(&command);

  EXPECT_FALSE(finished);

  scheduler.Run();
  scheduler.Run();
  
  EXPECT_FALSE(scheduler.IsScheduled(&command));
  EXPECT_TRUE(finished);
}

TEST_F(CommandDecoratorTest, PerpetuallyTest) {
  CommandScheduler scheduler = GetScheduler();

  auto command = InstantCommand([]{}, {}).Perpetually();

  scheduler.Schedule(&command);

  scheduler.Run();
  scheduler.Run();
  
  EXPECT_TRUE(scheduler.IsScheduled(&command));
}
