/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.experimental.command;

import java.util.Set;

/**
 * Schedules the given commands with this command is initialized, and ends when all the commands are
 * no longer scheduled.  Useful for forking off from CommandGroups.  If this command is interrupted,
 * it will cancel all of the commands.
 */
public class BlockingScheduleCommand extends SendableCommandBase {
  private final Set<Command> m_toSchedule;
  private boolean m_finished;

  /**
   * Creates a new BlockingScheduleCommand that schedules the given commands when initialized,
   * and ends when they are all no longer scheduled.
   *
   * @param toSchedule the commands to schedule
   */
  public BlockingScheduleCommand(Command... toSchedule) {
    m_toSchedule = Set.of(toSchedule);
  }

  @Override
  public void initialize() {
    m_finished = false;
    for (Command command : m_toSchedule) {
      command.schedule();
    }
  }

  @Override
  public void end(boolean interrupted) {
    if (interrupted) {
      for (Command command : m_toSchedule) {
        command.cancel();
      }
    }
  }

  @Override
  public void execute() {
    for (Command command : m_toSchedule) {
      m_finished &= !command.isScheduled();
    }
  }

  @Override
  public boolean isFinished() {
    return m_finished;
  }
}
