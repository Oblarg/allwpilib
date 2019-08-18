/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.first.wpilibj.geometry;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertAll;
import static org.junit.jupiter.api.Assertions.assertEquals;

class Twist2dTest {
  private static final double kEpsilon = 1E-9;

  @Test
  void testStraightLineTwist() {
    var straight = new Twist2d(5.0, 0.0, 0.0);
    var straightPose = new Pose2d().exp(straight);

    assertAll(
        () -> assertEquals(straightPose.getTranslation().getX(), 5.0, kEpsilon),
        () -> assertEquals(straightPose.getTranslation().getY(), 0.0, kEpsilon),
        () -> assertEquals(straightPose.getRotation().getRadians(), 0.0, kEpsilon)
    );
  }

  @Test
  void testQuarterCirleTwist() {
    var quarterCircle = new Twist2d(5.0 / 2.0 * Math.PI, 0, Math.PI / 2.0);
    var quarterCirclePose = new Pose2d().exp(quarterCircle);

    assertAll(
        () -> assertEquals(quarterCirclePose.getTranslation().getX(), 5.0, kEpsilon),
        () -> assertEquals(quarterCirclePose.getTranslation().getY(), 5.0, kEpsilon),
        () -> assertEquals(quarterCirclePose.getRotation().getDegrees(), 90.0, kEpsilon)
    );
  }

  @Test
  void testDiagonalNoDtheta() {
    var diagonal = new Twist2d(2.0, 2.0, 0.0);
    var diagonalPose = new Pose2d().exp(diagonal);

    assertAll(
        () -> assertEquals(diagonalPose.getTranslation().getX(), 2.0, kEpsilon),
        () -> assertEquals(diagonalPose.getTranslation().getY(), 2.0, kEpsilon),
        () -> assertEquals(diagonalPose.getRotation().getDegrees(), 0.0, kEpsilon)
    );
  }

  @Test
  void testPose2dLog() {
    final var start = new Pose2d();
    final var end = new Pose2d(5.0, 5.0, Rotation2d.fromDegrees(90.0));

    final var twist = start.log(end);

    assertAll(
        () -> assertEquals(twist.dx, 5.0 / 2.0 * Math.PI, kEpsilon),
        () -> assertEquals(twist.dy, 0.0, kEpsilon),
        () -> assertEquals(twist.dtheta, Math.PI / 2.0, kEpsilon)
    );
  }
}
