#include "frc/frc2/commands/CommandScheduler.h"
#include "frc/frc2/buttons/InternalButton.h"
#include "CommandTestBase.h"
#include "frc/frc2/commands/InstantCommand.h"
#include "frc/frc2/commands/ParallelCommandGroup.h"
#include "frc/frc2/commands/ParallelRaceGroup.h"
#include "frc/frc2/commands/ParallelDeadlineGroup.h"
#include "frc/frc2/commands/ConditionalCommand.h"
#include "frc/frc2/commands/SelectCommand.h"
#include "frc/frc2/commands/SequentialCommandGroup.h"

using namespace frc2;
class CommandRequirementsTest : public CommandTestBase {
  
};

TEST_F(CommandRequirementsTest, RequirementInterruptTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem requirement;

  MockCommand command1({&requirement});
  MockCommand command2({&requirement});

  EXPECT_CALL(command1, Initialize());
  EXPECT_CALL(command1, Execute());
  EXPECT_CALL(command1, End(true));
  EXPECT_CALL(command1, End(false)).Times(0);

  EXPECT_CALL(command2, Initialize());
  EXPECT_CALL(command2, Execute());
  EXPECT_CALL(command2, End(true)).Times(0);
  EXPECT_CALL(command2, End(false)).Times(0);

  scheduler.Schedule(&command1);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command1));
  scheduler.Schedule(&command2);
  EXPECT_FALSE(scheduler.IsScheduled(&command1));
  EXPECT_TRUE(scheduler.IsScheduled(&command2));
  scheduler.Run();
}

TEST_F(CommandRequirementsTest, RequirementUninterruptibleTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem requirement;

  MockCommand command1({&requirement});
  MockCommand command2({&requirement});

  EXPECT_CALL(command1, Initialize());
  EXPECT_CALL(command1, Execute()).Times(2);
  EXPECT_CALL(command1, End(true)).Times(0);
  EXPECT_CALL(command1, End(false)).Times(0);

  EXPECT_CALL(command2, Initialize()).Times(0);
  EXPECT_CALL(command2, Execute()).Times(0);
  EXPECT_CALL(command2, End(true)).Times(0);
  EXPECT_CALL(command2, End(false)).Times(0);

  scheduler.Schedule(false, &command1);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command1));
  scheduler.Schedule(&command2);
  EXPECT_TRUE(scheduler.IsScheduled(&command1));
  EXPECT_FALSE(scheduler.IsScheduled(&command2));
  scheduler.Run();
}

TEST_F(CommandRequirementsTest, ParallelGroupRequirementTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem requirement1;
  TestSubsystem requirement2;
  TestSubsystem requirement3;
  TestSubsystem requirement4;

  InstantCommand command1([]{},{&requirement1, &requirement2});
  InstantCommand command2([]{},{&requirement3});
  InstantCommand command3([]{},{&requirement3, &requirement4});

  ParallelCommandGroup group(std::move(command1), std::move(command2));

  scheduler.Schedule(&group);
  scheduler.Schedule(&command3);

  EXPECT_TRUE(scheduler.IsScheduled(&command3));
  EXPECT_FALSE(scheduler.IsScheduled(&group));
}

TEST_F(CommandRequirementsTest, SequentialGroupRequirementTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem requirement1;
  TestSubsystem requirement2;
  TestSubsystem requirement3;
  TestSubsystem requirement4;

  InstantCommand command1([]{},{&requirement1, &requirement2});
  InstantCommand command2([]{},{&requirement3});
  InstantCommand command3([]{},{&requirement3, &requirement4});

  SequentialCommandGroup group(std::move(command1), std::move(command2));

  scheduler.Schedule(&group);
  scheduler.Schedule(&command3);

  EXPECT_TRUE(scheduler.IsScheduled(&command3));
  EXPECT_FALSE(scheduler.IsScheduled(&group));
}

TEST_F(CommandRequirementsTest, RaceGroupRequirementTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem requirement1;
  TestSubsystem requirement2;
  TestSubsystem requirement3;
  TestSubsystem requirement4;

  InstantCommand command1([]{},{&requirement1, &requirement2});
  InstantCommand command2([]{},{&requirement3});
  InstantCommand command3([]{},{&requirement3, &requirement4});

  ParallelRaceGroup group(std::move(command1), std::move(command2));

  scheduler.Schedule(&group);
  scheduler.Schedule(&command3);

  EXPECT_TRUE(scheduler.IsScheduled(&command3));
  EXPECT_FALSE(scheduler.IsScheduled(&group));
}

TEST_F(CommandRequirementsTest, ParallelDeadlineRequirementTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem requirement1;
  TestSubsystem requirement2;
  TestSubsystem requirement3;
  TestSubsystem requirement4;

  InstantCommand command1([]{},{&requirement1, &requirement2});
  InstantCommand command2([]{},{&requirement3});
  InstantCommand command3([]{},{&requirement3, &requirement4});

  ParallelDeadlineGroup group(std::move(command1), std::move(command2));

  scheduler.Schedule(&group);
  scheduler.Schedule(&command3);

  EXPECT_TRUE(scheduler.IsScheduled(&command3));
  EXPECT_FALSE(scheduler.IsScheduled(&group));
}

TEST_F(CommandRequirementsTest, ConditionalCommandRequirementTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem requirement1;
  TestSubsystem requirement2;
  TestSubsystem requirement3;
  TestSubsystem requirement4;

  InstantCommand command1([]{},{&requirement1, &requirement2});
  InstantCommand command2([]{},{&requirement3});
  InstantCommand command3([]{},{&requirement3, &requirement4});

  ConditionalCommand conditional(std::move(command1), std::move(command2), []{return true;});
  scheduler.Schedule(&conditional);
  scheduler.Schedule(&command3);

  EXPECT_TRUE(scheduler.IsScheduled(&command3));
  EXPECT_FALSE(scheduler.IsScheduled(&conditional));
}

TEST_F(CommandRequirementsTest, SelectCommandRequirementTest) {
  CommandScheduler scheduler = GetScheduler();

  TestSubsystem requirement1;
  TestSubsystem requirement2;
  TestSubsystem requirement3;
  TestSubsystem requirement4;

  InstantCommand command1([]{},{&requirement1, &requirement2});
  InstantCommand command2([]{},{&requirement3});
  InstantCommand command3([]{},{&requirement3, &requirement4});

  SelectCommand<int> select(
    []{return 1;},
    std::pair(1, std::move(command1)), 
    std::pair(2, std::move(command2)));

  scheduler.Schedule(&select);
  scheduler.Schedule(&command3);

  EXPECT_TRUE(scheduler.IsScheduled(&command3));
  EXPECT_FALSE(scheduler.IsScheduled(&select));
}

TEST_F(CommandRequirementsTest, DefaultCommandRequirementErrorTest) {
  TestSubsystem requirement1;
  ErrorConfirmer confirmer("require");

  MockCommand command1;
  
  //TODO: actually test for correct error triggering
  requirement1.SetDefaultCommand(&command1);

  EXPECT_TRUE(requirement1.GetDefaultCommand() == NULL);
}
