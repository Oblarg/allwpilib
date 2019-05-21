package edu.wpi.first.wpilibj.smartdashboard;

import java.util.function.BooleanSupplier;
import java.util.function.Consumer;
import java.util.function.DoubleConsumer;
import java.util.function.DoubleSupplier;
import java.util.function.Supplier;

import edu.wpi.first.networktables.NetworkTableEntry;

/**
 * Utility class for Sendable widgets.  Each widget type (except those for which it is
 * unfeasible) has a corresponding helper method in this class that serves to formalize and
 * document the properties it needs.  Classes that can be sent as that widget type can simply
 * call the helper method from their
 * {@link edu.wpi.first.wpilibj.Sendable#initSendable(SendableBuilder)} method.
 */
public class SendableTypes {

  /**
   * Utility class, so constructor is private.
   */
  private SendableTypes() {
  }

  /**
   * Wrapper class for double properties.
   */
  public static class DoubleProperty {

    DoubleConsumer set;
    DoubleSupplier get;

    /**
     * Creates a new DoubleProperty.
     *
     * @param get The getter for the double
     * @param set The setter for the double
     */
    public DoubleProperty(DoubleSupplier get, DoubleConsumer set) {
      set = set;
      get = get;
    }
  }

  /**
   * Wrapper class for boolean properties.
   */
  public static class BooleanProperty {

    SendableBuilder.BooleanConsumer set;
    BooleanSupplier get;

    /**
     * Creates a new BooleanProperty
     *
     * @param get The getter for the boolean
     * @param set The setter for the boolean
     */
    public BooleanProperty(BooleanSupplier get, SendableBuilder.BooleanConsumer set) {
      set = set;
      get = get;
    }
  }

  /**
   * Wrapper class for String properties.
   */
  public static class StringProperty {

    Consumer<String> set;
    Supplier<String> get;

    /**
     * Creates a new StringProperty.
     *
     * @param get The getter for the string
     * @param set The setter for the string
     */
    public StringProperty(Supplier<String> get, Consumer<String> set) {
      set = set;
      get = get;
    }
  }

  /**
   * Wrapper class for double array properties.
   */
  public static class DoubleArrayProperty {

    Consumer<double[]> set;
    Supplier<double[]> get;

    /**
     * Creates a new DoubleArrayProperty.
     *
     * @param get The getter for the double array
     * @param set The setter for the double array
     */
    public DoubleArrayProperty(Supplier<double[]> get, Consumer<double[]> set) {
      set = set;
      get = get;
    }
  }

  /**
   * Wrapper class for boolean array properties.
   */
  public static class BooleanArrayProperty {

    Consumer<boolean[]> set;
    Supplier<boolean[]> get;

    /**
     * Creates a new BooleanArrayProperty.
     *
     * @param get The getter for the boolean array
     * @param set The setter for the boolean array
     */
    public BooleanArrayProperty(Supplier<boolean[]> get, Consumer<boolean[]> set) {
      set = set;
      get = get;
    }
  }

  /**
   * Wrapper class for String array properties.
   */
  public static class StringArrayProperty {

    Consumer<String[]> set;
    Supplier<String[]> get;

    /**
     * Creates a new StringArrayProperty.
     *
     * @param get The getter for the String array
     * @param set The setter for the String array
     */
    public StringArrayProperty(Supplier<String[]> get, Consumer<String[]> set) {
      set = set;
      get = get;
    }
  }

  /**
   * Sends an Accelerometer widget to the dashboard.
   *
   * @param builder The SendableBuilder to use
   * @param value The current measured acceleration
   */
  public static void sendAccelerometer(SendableBuilder builder, DoubleProperty value) {
    builder.setSmartDashboardType("Accelerometer");
    builder.addDoubleProperty("Value", value.get, value.set);
  }

  /**
   * Sends an AnalogInput widget to the dashboard.
   *
   * @param builder The SendableBuilder to use
   * @param value The current voltage of the AnalogInput
   */
  public static void sendAnalogInput(SendableBuilder builder, DoubleProperty value) {
    builder.setSmartDashboardType("AnalogInput");
    builder.addDoubleProperty("Value", value.get, value.set);
  }

  /**
   * Sends a Subsystem widget to the dashboard.
   *
   * @param builder The SendableBuilder to use
   * @param hasDefaultCommand Whether the subsystem has a default command
   * @param defaultCommand The name of the default command
   * @param hasCurrentCommand Whether the subsystem is currently running a command
   * @param currentCommand The name of the currently-running command
   */
  public static void sendSubsystem(SendableBuilder builder,
                                   BooleanProperty hasDefaultCommand,
                                   StringProperty defaultCommand,
                                   BooleanProperty hasCurrentCommand,
                                   StringProperty currentCommand) {
    builder.setSmartDashboardType("Subsystem");
    builder.addBooleanProperty(".hasDefault", hasDefaultCommand.get, hasDefaultCommand.set);
    builder.addStringProperty(".default", defaultCommand.get, defaultCommand.set);
    builder.addBooleanProperty(".hasCommand", hasCurrentCommand.get, hasCurrentCommand.set);
    builder.addStringProperty(".command", currentCommand.get, currentCommand.set);
  }

  /**
   * Sends a Command widget to the dashboard.
   *
   * @param builder The SendableBuilder to use
   * @param name The name of the command
   * @param running Whether the command is currently running
   * @param isParented Whether the command is composed in a CommandGroup
   */
  public static void sendCommand(SendableBuilder builder,
                                 StringProperty name,
                                 BooleanProperty running,
                                 BooleanProperty isParented) {
    builder.setSmartDashboardType("Command");
    builder.addStringProperty(".name", name.get, name.set);
    builder.addBooleanProperty("running", running.get, running.set);
    builder.addBooleanProperty(".isParented", isParented.get, isParented.set);
  }

  /**
   * Sends a DifferentialDrive widget to the dashboard.
   *
   * @param builder The SendableBuilder to use
   * @param safeState A method that puts the mechanism into a safe state
   * @param leftMotorSpeed The speed of the left motor
   * @param rightMotorSpeed The speed of the right motor
   */
  public static void sendDifferentialDrive(SendableBuilder builder,
                                           Runnable safeState,
                                           DoubleProperty leftMotorSpeed,
                                           DoubleProperty rightMotorSpeed) {
    builder.setSmartDashboardType("DifferentialDrive");
    builder.setActuator(true);
    builder.setSafeState(safeState);
    builder.addDoubleProperty("Left Motor Speed", leftMotorSpeed.get, leftMotorSpeed.set);
    builder.addDoubleProperty("Right Motor Speed", rightMotorSpeed.get, rightMotorSpeed.set);
  }

  /**
   * Sends an Encoder widget to the dashboard.
   *
   * @param builder The SendableBuilder to use
   * @param speed The current speed of the encoder
   * @param distance The current displacement of the encoder
   * @param distancePerTick The distance-per-tick of the encoder
   */
  public static void sendEncoder(SendableBuilder builder,
                                 DoubleProperty speed,
                                 DoubleProperty distance,
                                 DoubleProperty distancePerTick) {
    builder.setSmartDashboardType("Encoder");
    builder.addDoubleProperty("Speed", speed.get, speed.set);
    builder.addDoubleProperty("Distance", distance.get, distance.set);
    builder.addDoubleProperty("Distance per Tick", distancePerTick.get, distancePerTick.set);
  }

  /**
   * Sends a Gyro widget to the dashboard.
   *
   * @param builder The SendableBuilder to use
   * @param value The measured heading of the gyro
   */
  public static void sendGyro(SendableBuilder builder, DoubleProperty value) {
    builder.setSmartDashboardType("Gyro");
    builder.addDoubleProperty("Value", value.get, value.set);
  }

  /**
   * Sends a MecanumDrive widget to the dashboard.
   *
   * @param builder The SendableBuilder to use
   * @param safeState A method that puts the mechanism into a safe state
   * @param frontLeftMotorSpeed The speed of the front-left motor
   * @param frontRightMotorSpeed The speed of the front-right motor
   * @param rearLeftMotorSpeed The speed of the rear-left motor
   * @param rearRightMotorSpeed The speed of the rear-right motor
   */
  public static void sendMecanumDrive(SendableBuilder builder,
                                      Runnable safeState,
                                      DoubleProperty frontLeftMotorSpeed,
                                      DoubleProperty frontRightMotorSpeed,
                                      DoubleProperty rearLeftMotorSpeed,
                                      DoubleProperty rearRightMotorSpeed) {
    builder.setSmartDashboardType("MecanumDrive");
    builder.setActuator(true);
    builder.setSafeState(safeState);
    builder.addDoubleProperty("Front Left Motor Speed", frontLeftMotorSpeed.get,
        frontLeftMotorSpeed.set);
    builder.addDoubleProperty("Front Right Motor Speed", frontRightMotorSpeed.get,
        frontRightMotorSpeed.set);
    builder.addDoubleProperty("Rear Left Motor Speed", rearLeftMotorSpeed.get,
        rearLeftMotorSpeed.set);
    builder.addDoubleProperty("Rear Right Motor Speed", rearRightMotorSpeed.get,
        rearRightMotorSpeed.set);
  }

  /**
   * Sends a PIDCommand widget to the dashboard.
   *
   * <p>NOTE: The {@link edu.wpi.first.wpilibj.Sendable#initSendable(SendableBuilder)}
   * implementation should send the superclass Command and the enclosed PIDController, first.
   *
   * @param builder The SendableBuilder to use
   */
  public static void sendPIDCommand(SendableBuilder builder) {
    builder.setSmartDashboardType("PIDCommand");
  }

  /**
   * Sends a PIDController widget to the dashboard.
   *
   * @param builder The SendableBuilder to use
   * @param safeState A method that puts the mechanism into a safe state
   * @param p The proportional gain of the controller
   * @param i The integral gain of the controller
   * @param d The derivative gain of the controller
   * @param f The feedforward gain of the controller
   * @param setpoint The current setpoint of the controller
   * @param enabled Whether the controller is enabled
   */
  public static void sendPIDController(SendableBuilder builder,
                                       Runnable safeState,
                                       DoubleProperty p,
                                       DoubleProperty i,
                                       DoubleProperty d,
                                       DoubleProperty f,
                                       DoubleProperty setpoint,
                                       BooleanProperty enabled) {
    builder.setSmartDashboardType("PIDController");
    builder.setSafeState(safeState);
    builder.addDoubleProperty("p", p.get, p.set);
    builder.addDoubleProperty("i", i.get, i.set);
    builder.addDoubleProperty("d", d.get, d.set);
    builder.addDoubleProperty("f", f.get, f.set);
    builder.addDoubleProperty("setpoint", setpoint.get, setpoint.set);
    builder.addBooleanProperty("enabled", enabled.get, enabled.set);
  }

  /**
   * Sends a PowerDistributionPanel widget to the dashboard.
   *
   * @param builder The SendableBuilder to use
   * @param channels The individual channel currents of the PDP
   * @param voltage The total battery voltage
   * @param totalCurrent The total current draw across all channels
   */
  public static void sendPDP(SendableBuilder builder,
                             DoubleProperty[] channels,
                             DoubleProperty voltage,
                             DoubleProperty totalCurrent) {
    builder.setSmartDashboardType("PowerDistributionPanel");
    for (int i = 0; i < channels.length; i++) {
      builder.addDoubleProperty("Chan " + i, channels[i].get, channels[i].set);
    }
    builder.addDoubleProperty("Voltage", voltage.get, voltage.set);
    builder.addDoubleProperty("TotalCurrent", totalCurrent.get, totalCurrent.set);
  }

  /**
   * Sends a Relay widget to the dashboard.
   *
   * @param builder The SendableBuilder to use
   * @param safeState A method that puts the mechanism into a safe state
   * @param value The current state of the relay
   */
  public static void sendRelay(SendableBuilder builder,
                               Runnable safeState,
                               StringProperty value) {
    builder.setSmartDashboardType("Relay");
    builder.setActuator(true);
    builder.setSafeState(safeState);
    builder.addStringProperty("Value", value.get, value.set);
  }

  /**
   * Sends a SpeedController widget to the dashboard.
   *
   * @param builder The SendableBuilder to use
   * @param safeState A method that puts the mechanism into a safe state
   * @param value The current output of the SpeedController
   */
  public static void sendSpeedController(SendableBuilder builder,
                                         Runnable safeState,
                                         DoubleProperty value) {
    builder.setSmartDashboardType("Speed Controller");
    builder.setActuator(true);
    builder.setSafeState(safeState);
    builder.addDoubleProperty("Value", value.get, value.set);
  }

  /**
   * Sends a Three-Axis Accelerometer widget to the dashboard.
   *
   * @param builder The SendableBuilder to use
   * @param x The acceleration in the x-direction
   * @param y The acceleration in the y-direction
   * @param z The acceleration in the z-direction
   */
  public static void sendThreeAxisAccelerometer(SendableBuilder builder,
                                                DoubleProperty x,
                                                DoubleProperty y,
                                                DoubleProperty z) {
    builder.setSmartDashboardType("3AxisAccelerometer");
    builder.addDoubleProperty("X", x.get, x.set);
    builder.addDoubleProperty("Y", y.get, y.set);
    builder.addDoubleProperty("Z", z.get, z.set);
  }

  /**
   * Sends a Three-Axis Accelerometer widget to the dashboard.
   *
   * @param builder The SendableBuilder to use
   * @param values An array containing the accelerations in the x, y, and z directions
   */
  public static void sendThreeAxisAccelerometer(SendableBuilder builder,
                                                DoubleArrayProperty values) {
    builder.setSmartDashboardType("3AxisAccelerometer");
    NetworkTableEntry entryX = builder.getEntry("X");
    NetworkTableEntry entryY = builder.getEntry("Y");
    NetworkTableEntry entryZ = builder.getEntry("Z");
    builder.setUpdateTable(() -> {
      double[] data = values.get.get();
      entryX.setDouble(data[0]);
      entryY.setDouble(data[1]);
      entryZ.setDouble(data[2]);});
  }
}
