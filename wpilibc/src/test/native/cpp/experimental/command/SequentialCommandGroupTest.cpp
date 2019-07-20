#include "CommandTestBase.h"
#include "frc/experimental/command/SequentialCommandGroup.h"
#include "frc/experimental/command/InstantCommand.h"
#include "frc/experimental/command/WaitUntilCommand.h"

using namespace frc::experimental;

class SequentialCommandGroupTest : public CommandTestBase {

};

TEST_F(SequentialCommandGroupTest, SequentialGroupScheduleTest){
  CommandScheduler scheduler = GetScheduler();

  std::unique_ptr<MockCommand> command1Holder = std::make_unique<MockCommand>();
  std::unique_ptr<MockCommand> command2Holder = std::make_unique<MockCommand>();
  std::unique_ptr<MockCommand> command3Holder = std::make_unique<MockCommand>();

  MockCommand* command1 = command1Holder.get();
  MockCommand* command2 = command2Holder.get();
  MockCommand* command3 = command3Holder.get();

  SequentialCommandGroup group{
    tcb::make_vector<std::unique_ptr<Command>>(std::move(command1Holder), std::move(command2Holder), std::move(command3Holder))};

  EXPECT_CALL(*command1, Initialize());
  EXPECT_CALL(*command1, Execute()).Times(1);
  EXPECT_CALL(*command1, End(false));

  EXPECT_CALL(*command2, Initialize());
  EXPECT_CALL(*command2, Execute()).Times(1);
  EXPECT_CALL(*command2, End(false));

  EXPECT_CALL(*command3, Initialize());
  EXPECT_CALL(*command3, Execute()).Times(1);
  EXPECT_CALL(*command3, End(false));

  scheduler.Schedule(&group);

  command1->SetFinished(true);
  scheduler.Run();
  command2->SetFinished(true);
  scheduler.Run();
  command3->SetFinished(true);
  scheduler.Run();

  EXPECT_FALSE(scheduler.IsScheduled(&group));
}

TEST_F(SequentialCommandGroupTest, SequentialGroupInterruptTest){
  CommandScheduler scheduler = GetScheduler();

  std::unique_ptr<MockCommand> command1Holder = std::make_unique<MockCommand>();
  std::unique_ptr<MockCommand> command2Holder = std::make_unique<MockCommand>();
  std::unique_ptr<MockCommand> command3Holder = std::make_unique<MockCommand>();

  MockCommand* command1 = command1Holder.get();
  MockCommand* command2 = command2Holder.get();
  MockCommand* command3 = command3Holder.get();

  SequentialCommandGroup group{
    tcb::make_vector<std::unique_ptr<Command>>(std::move(command1Holder), std::move(command2Holder), std::move(command3Holder))};

  EXPECT_CALL(*command1, Initialize());
  EXPECT_CALL(*command1, Execute()).Times(1);
  EXPECT_CALL(*command1, End(false));

  EXPECT_CALL(*command2, Initialize());
  EXPECT_CALL(*command2, Execute()).Times(0);
  EXPECT_CALL(*command2, End(false)).Times(0);
  EXPECT_CALL(*command2, End(true));

  EXPECT_CALL(*command3, Initialize()).Times(0);
  EXPECT_CALL(*command3, Execute()).Times(0);
  EXPECT_CALL(*command3, End(false)).Times(0);
  EXPECT_CALL(*command3, End(true)).Times(0);

  scheduler.Schedule(&group);

  command1->SetFinished(true);
  scheduler.Run();
  scheduler.Cancel(&group);
  scheduler.Run();

  EXPECT_FALSE(scheduler.IsScheduled(&group));
}

TEST_F(SequentialCommandGroupTest, SequentialGroupNotScheduledCancelTest){
  CommandScheduler scheduler = GetScheduler();

  SequentialCommandGroup group{InstantCommand(), InstantCommand()};

  EXPECT_NO_FATAL_FAILURE(scheduler.Cancel(&group));
}

TEST_F(SequentialCommandGroupTest, SequentialGroupCopyTest) {
  CommandScheduler scheduler = GetScheduler();

  bool finished = false;

  WaitUntilCommand command([&finished]{return finished;});

  SequentialCommandGroup group(command);
  scheduler.Schedule(&group);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&group));
  finished = true;
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&group));
}
