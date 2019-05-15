/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.experimental.command;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class DefaultCommandTest extends CommandTestBase {
  @Test
  void defaultCommandScheduleTest() {
    CommandScheduler scheduler = new CommandScheduler();

    TestSubsystem hasDefaultCommand = new TestSubsystem();

    MockCommandHolder defaultHolder = new MockCommandHolder(true, hasDefaultCommand);
    Command defaultCommand = defaultHolder.getMock();

    scheduler.setDefaultCommand(hasDefaultCommand, defaultCommand);
    scheduler.run();

    assertTrue(scheduler.isScheduled(defaultCommand));
  }

  @Test
  void defaultCommandInterruptResumeTest() {
    CommandScheduler scheduler = new CommandScheduler();

    TestSubsystem hasDefaultCommand = new TestSubsystem();

    MockCommandHolder defaultHolder = new MockCommandHolder(true, hasDefaultCommand);
    Command defaultCommand = defaultHolder.getMock();
    MockCommandHolder interrupterHolder = new MockCommandHolder(true, hasDefaultCommand);
    Command interrupter = interrupterHolder.getMock();

    scheduler.setDefaultCommand(hasDefaultCommand, defaultCommand);
    scheduler.run();
    scheduler.schedule(interrupter);

    assertFalse(scheduler.isScheduled(defaultCommand));
    assertTrue(scheduler.isScheduled(interrupter));

    scheduler.cancel(interrupter);
    scheduler.run();

    assertTrue(scheduler.isScheduled(defaultCommand));
    assertFalse(scheduler.isScheduled(interrupter));
  }
}
