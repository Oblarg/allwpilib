#include "CommandTestBase.h"
#include "frc/frc2/commands/WaitCommand.h"
#include "frc/frc2/commands/WaitUntilCommand.h"

using namespace frc2;
class WaitCommandTest : public CommandTestBase {

};

TEST_F(WaitCommandTest, WaitCommandTest) {
  CommandScheduler scheduler = GetScheduler();

  WaitCommand command(.1);

  scheduler.Schedule(&command);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command));
  std::this_thread::sleep_for(std::chrono::milliseconds(110));
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
}

TEST_F(WaitCommandTest, WaitUntilCommandTest) {
  CommandScheduler scheduler = GetScheduler();

  bool finished = false;

  WaitUntilCommand command([&finished]{return finished;});

  scheduler.Schedule(&command);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command));
  finished = true;
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
}
