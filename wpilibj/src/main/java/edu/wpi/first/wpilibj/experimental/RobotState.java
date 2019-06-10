/*----------------------------------------------------------------------------*/
/* Copyright (c) 2008-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.experimental;

import edu.wpi.first.wpilibj.DriverStation;

/**
 * Provide access to the current state of the robot.
 */
public class RobotState {

    /**
     * Provides access to the network communication data to / from the Driver Station
     */
    private final DriverStation driverStation;

    /**
     * The Singleton Instance.
     */
    private static RobotState instance = null;

    /**
     * Returns the singleton RobotState instance.
     *
     * @return the instance
     */
    public static synchronized RobotState getInstance() {
        if (instance == null) {
            instance = new RobotState();
        }
        return instance;
    }

    RobotState() {
        this(DriverStation.getInstance());
    }

    protected RobotState(DriverStation driverStation) {
        this.driverStation = driverStation;
    }

    /**
     * @return True if the robot is currently enabled, false otherwise.
     */
    public boolean isEnabled() {
        return driverStation.isEnabled();
    }

    /**
     * @return True if the robot is currently disabled, false otherwise.
     */
    public boolean isDisabled() {
        return driverStation.isDisabled();
    }

    /**
     * @return True if the robot is currently enabled in teleoperated mode, false
     *         otherwise.
     */
    public boolean isOperatorControl() {
        return driverStation.isOperatorControl();
    }

    /**
     * @return True if the robot is currently enabled in autonomous mode, false
     *         otherwise.
     */
    public boolean isAutonomous() {
        return driverStation.isAutonomous();
    }

    /**
     * @return True if the robot is currently enabled in test mode, false otherwise.
     */
    public boolean isTest() {
        return driverStation.isTest();
    }

}
