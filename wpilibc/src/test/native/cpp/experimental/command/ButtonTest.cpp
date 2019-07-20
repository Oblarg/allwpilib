// #include "gtest/gtest.h"
// #include "frc/experimental/command/CommandScheduler.h"
// #include "frc/experimental/buttons/InternalButton.h"
// #include "CommandTestBase.h"

// using namespace frc::experimental;

// class ButtonTest : public CommandTestBase {
  
// };

// TEST_F(ButtonTest, WhenPressedTest) {
//   auto& scheduler = CommandScheduler::GetInstance();
//   MockCommandHolder command1Holder{true, {}};
//   std::unique_ptr<MockCommandHolder::MockCommand>& command1 = command1Holder.GetMock();

//   InternalButton button;

//   EXPECT_CALL(*command1, Initialize());
//   EXPECT_CALL(*command1, Execute()).Times(2);

//   button.SetPressed(false);
//   button.WhenPressed(command1);
//   scheduler.Run();
//   EXPECT_FALSE(scheduler.IsScheduled(command1));
//   button.SetPressed(true);
//   scheduler.Run();
//   scheduler.Run();
//   EXPECT_TRUE(scheduler.IsScheduled(command1));
// }
