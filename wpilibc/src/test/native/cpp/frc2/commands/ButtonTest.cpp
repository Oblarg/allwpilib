/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "CommandTestBase.h"
#include "frc/frc2/buttons/InternalButton.h"
#include "frc/frc2/commands/CommandScheduler.h"
#include "frc/frc2/commands/WaitUntilCommand.h"
#include "gtest/gtest.h"

using namespace frc2;
class ButtonTest : public CommandTestBase {};

TEST_F(ButtonTest, FooTest) {
  auto& scheduler = CommandScheduler::GetInstance();
  bool finished = false;
  bool pressed = false;

  WaitUntilCommand command([&finished] { return finished; });

  Trigger([&pressed]{return pressed;}).WhenActive(&command);

  scheduler.Run();

  EXPECT_FALSE(scheduler.IsScheduled(&command));

  pressed = true;

  scheduler.Run();

  EXPECT_TRUE(scheduler.IsScheduled(&command));
}

TEST_F(ButtonTest, WhenPressedTest) {
  auto& scheduler = CommandScheduler::GetInstance();
  bool finished = false;
  WaitUntilCommand command([&finished] { return finished; });

  InternalButton button;

  button.SetPressed(false);
  button.WhenPressed(&command);
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
  button.SetPressed(true);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command));
  finished = true;
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
}

TEST_F(ButtonTest, WhenReleasedTest) {
  auto& scheduler = CommandScheduler::GetInstance();
  bool finished = false;
  WaitUntilCommand command([&finished] { return finished; });

  InternalButton button;

  button.SetPressed(true);
  button.WhenReleased(&command);
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
  button.SetPressed(false);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command));
  finished = true;
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
}

TEST_F(ButtonTest, WhileHeldTest) {
  auto& scheduler = CommandScheduler::GetInstance();
  bool finished = false;
  WaitUntilCommand command([&finished] { return finished; });

  InternalButton button;

  button.SetPressed(false);
  button.WhileHeld(&command);
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
  button.SetPressed(true);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command));
  finished = true;
  scheduler.Run();
  finished = false;
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command));
  button.SetPressed(false);
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
}

TEST_F(ButtonTest, WhenHeldTest) {
  auto& scheduler = CommandScheduler::GetInstance();
  bool finished = false;
  WaitUntilCommand command([&finished] { return finished; });

  InternalButton button;

  button.SetPressed(false);
  button.WhenHeld(&command);
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
  button.SetPressed(true);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command));
  finished = true;
  scheduler.Run();
  finished = false;
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));

  button.SetPressed(false);
  button.WhenHeld(&command);
  button.SetPressed(true);
  scheduler.Run();
  button.SetPressed(false);
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
}

TEST_F(ButtonTest, ToggleWhenPressedTest) {
  auto& scheduler = CommandScheduler::GetInstance();
  bool finished = false;
  WaitUntilCommand command([&finished] { return finished; });

  InternalButton button;

  button.SetPressed(false);
  button.ToggleWhenPressed(&command);
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
  button.SetPressed(true);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command));
  button.SetPressed(false);
  scheduler.Run();
  button.SetPressed(true);
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
}

TEST_F(ButtonTest, AndTest) {
  auto& scheduler = CommandScheduler::GetInstance();
  bool finished = false;
  WaitUntilCommand command([&finished] { return finished; });

  InternalButton button1;
  InternalButton button2;

  button1.SetPressed(false);
  button2.SetPressed(false);

  auto both = button1 && button2;

  both.WhenActive(&command);
  button1.SetPressed(true);
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
  button2.SetPressed(true);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command));
}

TEST_F(ButtonTest, OrTest) {
  auto& scheduler = CommandScheduler::GetInstance();
  bool finished = false;
  WaitUntilCommand command1([&finished] { return finished; });
  WaitUntilCommand command2([&finished] { return finished; });

  InternalButton button1;
  InternalButton button2;

  button1.SetPressed(false);
  button2.SetPressed(false);

  auto either = button1 || button2;

  either.WhenActive(&command1);
  button1.SetPressed(true);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command1));

  button1.SetPressed(false);

  either.WhenActive(&command2);
  button2.SetPressed(true);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command2));
}

TEST_F(ButtonTest, NegateTest) {
  auto& scheduler = CommandScheduler::GetInstance();
  bool finished = false;
  WaitUntilCommand command([&finished] { return finished; });

  InternalButton button1;

  button1.SetPressed(true);

  auto negated = !button1;

  negated.WhenActive(&command);
  scheduler.Run();
  EXPECT_FALSE(scheduler.IsScheduled(&command));
  button1.SetPressed(false);
  scheduler.Run();
  EXPECT_TRUE(scheduler.IsScheduled(&command));
}
