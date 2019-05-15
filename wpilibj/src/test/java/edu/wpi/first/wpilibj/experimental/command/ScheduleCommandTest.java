/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.experimental.command;

import org.junit.jupiter.api.Test;

import static org.mockito.Mockito.verify;

public class ScheduleCommandTest extends CommandTestBase {
  @Test
  void scheduleCommandTest() {
    CommandScheduler scheduler = new CommandScheduler();

    MockCommandHolder command1Holder = new MockCommandHolder(true);
    Command command1 = command1Holder.getMock();
    MockCommandHolder command2Holder = new MockCommandHolder(true);
    Command command2 = command2Holder.getMock();

    ScheduleCommand scheduleCommand = new ScheduleCommand(command1, command2);

    scheduler.schedule(scheduleCommand);

    verify(command1).schedule();
    verify(command2).schedule();
  }
}
