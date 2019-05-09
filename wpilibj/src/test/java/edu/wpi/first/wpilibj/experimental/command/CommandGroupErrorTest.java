package edu.wpi.first.wpilibj.experimental.command;

import org.junit.jupiter.api.Test;

import edu.wpi.first.wpilibj.command.IllegalUseOfCommandException;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class CommandGroupErrorTest extends CommandTestBase {
  @Test
  void commandInMultipleGroupsTest() {

    MockCommandHolder command1Holder = new MockCommandHolder(true);
    Command command1 = command1Holder.getMock();
    MockCommandHolder command2Holder = new MockCommandHolder(true);
    Command command2 = command2Holder.getMock();

    @SuppressWarnings("PMD.UnusedLocalVariable")
    Command group = new ParallelCommandGroup(command1, command2);
    assertThrows(IllegalUseOfCommandException.class,
        () -> new ParallelCommandGroup(command1, command2));
  }

  @Test
  void commandInGroupExternallyScheduledTest() {
    CommandScheduler scheduler = new CommandScheduler();

    MockCommandHolder command1Holder = new MockCommandHolder(true);
    Command command1 = command1Holder.getMock();
    MockCommandHolder command2Holder = new MockCommandHolder(true);
    Command command2 = command2Holder.getMock();

    @SuppressWarnings("PMD.UnusedLocalVariable")
    Command group = new ParallelCommandGroup(command1, command2);

    assertThrows(IllegalUseOfCommandException.class,
        () -> scheduler.schedule(command1));
  }

  @Test
  void redecoratedCommandErrorTest() {
    Command command = new InstantCommand();

    assertDoesNotThrow(() -> command.withTimeout(10).interruptOn(() -> false));
    assertThrows(IllegalUseOfCommandException.class, () -> command.withTimeout(10));
    CommandGroupBase.clearGroupedCommand(command);
    assertDoesNotThrow(() -> command.withTimeout(10));
  }
}
