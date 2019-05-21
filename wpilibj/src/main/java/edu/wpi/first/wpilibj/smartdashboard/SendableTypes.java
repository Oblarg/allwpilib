package edu.wpi.first.wpilibj.smartdashboard;

import java.util.function.BooleanSupplier;
import java.util.function.Consumer;
import java.util.function.DoubleConsumer;
import java.util.function.DoubleSupplier;
import java.util.function.Supplier;

public class SendableTypes {

  private SendableTypes() {
  }

  public final class DoubleProperty {

    DoubleConsumer set;
    DoubleSupplier get;

    public DoubleProperty(DoubleSupplier get, DoubleConsumer set) {
      set = set;
      get = get;
    }
  }

  public final class BooleanProperty {

    SendableBuilder.BooleanConsumer set;
    BooleanSupplier get;

    public BooleanProperty(BooleanSupplier get, SendableBuilder.BooleanConsumer set) {
      set = set;
      get = get;
    }
  }

  public final class StringProperty {

    Consumer<String> set;
    Supplier<String> get;

    public StringProperty(DoubleSupplier get, DoubleConsumer set) {
      set = set;
      get = get;
    }
  }

  public final class DoubleArrayProperty {

    Consumer<double[]> set;
    Supplier<double[]> get;

    public DoubleArrayProperty(Supplier<double[]> get, Consumer<double[]> set) {
      set = set;
      get = get;
    }
  }

  public final class BooleanArrayProperty {

    Consumer<boolean[]> set;
    Supplier<boolean[]> get;

    public BooleanArrayProperty(Supplier<boolean[]> get, Consumer<boolean[]> set) {
      set = set;
      get = get;
    }
  }

  public final class StringArrayProperty {

    Consumer<String[]> set;
    Supplier<String[]> get;

    public StringArrayProperty(Supplier<String[]> get, Consumer<String[]> set) {
      set = set;
      get = get;
    }
  }

  public static void sendAccelerometer(SendableBuilder builder, DoubleProperty value) {
    builder.setSmartDashboardType("Accelerometer");
    builder.addDoubleProperty("Value", value.get, value.set);
  }

  public static void sendAnalogInput(SendableBuilder builder, DoubleProperty value) {
    builder.setSmartDashboardType("AnalogInput");
    builder.addDoubleProperty("Value", value.get, value.set);
  }

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

  public static void sendCommand(SendableBuilder builder,
                                 StringProperty name,
                                 BooleanProperty running,
                                 BooleanProperty isParented) {
    builder.setSmartDashboardType("Command");
    builder.addStringProperty(".name", name.get, name.set);
    builder.addBooleanProperty("running", running.get, running.set);
    builder.addBooleanProperty(".isParented", isParented.get, isParented.set);
  }

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

  public static void sendEncoder(SendableBuilder builder,
                                 DoubleProperty speed,
                                 DoubleProperty distance,
                                 DoubleProperty distancePerTick) {
    builder.setSmartDashboardType("Encoder");
    builder.addDoubleProperty("Speed", speed.get, speed.set);
    builder.addDoubleProperty("Distance", distance.get, distance.set);
    builder.addDoubleProperty("Distance per Tick", distancePerTick.get, distancePerTick.set);
  }

  public static void sendGyro(SendableBuilder builder, DoubleProperty value) {
    builder.setSmartDashboardType("Gyro");
    builder.addDoubleProperty("Value", value.get, value.set);
  }

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

  public static void sendPIDCommand(SendableBuilder builder) {
    builder.setSmartDashboardType("PIDCommand");
  }

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
}
