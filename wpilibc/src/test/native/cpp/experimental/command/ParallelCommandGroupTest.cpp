#include "CommandTestBase.h"
#include "frc/experimental/command/ParallelCommandGroup.h"
#include "frc/experimental/command/InstantCommand.h"
#include "frc/experimental/command/WaitUntilCommand.h"

using namespace frc::experimental;

class ParallelCommandGroupTest : public CommandTestBase {

};

TEST_F(ParallelCommandGroupTest, ParallelGroupScheduleTest){
  CommandScheduler scheduler = GetScheduler();

  std::unique_ptr<MockCommand> command1Holder = std::make_unique<MockCommand>();
  std::unique_ptr<MockCommand> command2Holder = std::make_unique<MockCommand>();

  MockCommand* command1 = command1Holder.get();
  MockCommand* command2 = command2Holder.get();

  ParallelCommandGroup group(tcb::make_vector<std::unique_ptr<Command>>(std::move(command1Holder), std::move(command2Holder)));

  EXPECT_CALL(*command1, Initialize());
  EXPECT_CALL(*command1, Execute()).Times(1);
  EXPECT_CALL(*command1, End(false));

  EXPECT_CALL(*command2, Initialize());
  EXPECT_CALL(*command2, Execute()).Times(2);
  EXPECT_CALL(*command2, End(false));

  scheduler.Schedule(&group);

  command1->SetFinished(true);
  scheduler.Run();
  command2->SetFinished(true);
  scheduler.Run();

  EXPECT_FALSE(scheduler.IsScheduled(&group));
}

TEST_F(ParallelCommandGroupTest, ParallelGroupInterruptTest){
  CommandScheduler scheduler = GetScheduler();

  std::unique_ptr<MockCommand> command1Holder = std::make_unique<MockCommand>();
  std::unique_ptr<MockCommand> command2Holder = std::make_unique<MockCommand>();

  MockCommand* command1 = command1Holder.get();
  MockCommand* command2 = command2Holder.get();

  ParallelCommandGroup group(tcb::make_vector<std::unique_ptr<Command>>(std::move(command1Holder), std::move(command2Holder)));

  EXPECT_CALL(*command1, Initialize());
  EXPECT_CALL(*command1, Execute()).Times(1);
  EXPECT_CALL(*command1, End(false));

  EXPECT_CALL(*command2, Initialize());
  EXPECT_CALL(*command2, Execute()).Times(2);
  EXPECT_CALL(*command2, End(false)).Times(0);
  EXPECT_CALL(*command2, End(true));

  scheduler.Schedule(&group);

  command1->SetFinished(true);
  scheduler.Run();
  scheduler.Run();
  scheduler.Cancel(&group);

  EXPECT_FALSE(scheduler.IsScheduled(&group));
}

TEST_F(ParallelCommandGroupTest, ParallelGroupNotScheduledCancelTest){
  CommandScheduler scheduler = GetScheduler();

  ParallelCommandGroup group((InstantCommand(), InstantCommand()));

  EXPECT_NO_FATAL_FAILURE(scheduler.Cancel(&group));
}

TEST_F(ParallelCommandGroupTest, ParallelGroupCopyTest) {
  CommandScheduler scheduler = GetScheduler();

  bool finished = false;

  WaitUntilCommand command([&finished]{return finished;});

  ParallelCommandGroup group(command);
  scheduler.Schedule(&group);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&group));
  finished = true;
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&group));
}
