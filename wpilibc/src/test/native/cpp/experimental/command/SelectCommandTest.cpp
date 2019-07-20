#include "CommandTestBase.h"
#include "frc/experimental/command/InstantCommand.h"
#include "frc/experimental/command/SelectCommand.h"
#include "frc/experimental/command/ConditionalCommand.h"

using namespace frc::experimental;

class SelectCommandTest : public CommandTestBase {

};

TEST_F(SelectCommandTest, SelectCommandTest) {
  CommandScheduler scheduler = GetScheduler();

  std::unique_ptr<MockCommand> mock = std::make_unique<MockCommand>();
  MockCommand* mockptr = mock.get();

  EXPECT_CALL(*mock, Initialize());
  EXPECT_CALL(*mock, Execute()).Times(2);
  EXPECT_CALL(*mock, End(false));

  std::vector<std::pair<int, std::unique_ptr<Command>>> temp;

  temp.emplace_back(std::pair(1, std::move(mock)));
  temp.emplace_back(std::pair(2, std::make_unique<InstantCommand>()));
  temp.emplace_back(std::pair(3, std::make_unique<InstantCommand>()));

  SelectCommand<int> select([]{return 1;}, std::move(temp));

  scheduler.Schedule(&select);
  scheduler.Run();
  mockptr->SetFinished(true);
  scheduler.Run();

  EXPECT_FALSE(scheduler.IsScheduled(&select));
}

TEST_F(SelectCommandTest, ConditionalCommandTest) {
  CommandScheduler scheduler = GetScheduler();

  std::unique_ptr<MockCommand> mock = std::make_unique<MockCommand>();
  MockCommand* mockptr = mock.get();

  EXPECT_CALL(*mock, Initialize());
  EXPECT_CALL(*mock, Execute()).Times(2);
  EXPECT_CALL(*mock, End(false));

  ConditionalCommand conditional(std::move(mock), std::make_unique<InstantCommand>(), []{return true;});

  scheduler.Schedule(&conditional);
  scheduler.Run();
  mockptr->SetFinished(true);
  scheduler.Run();

  EXPECT_FALSE(scheduler.IsScheduled(&conditional));
}
