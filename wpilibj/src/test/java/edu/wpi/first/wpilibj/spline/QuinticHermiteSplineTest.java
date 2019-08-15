package edu.wpi.first.wpilibj.spline;

import edu.wpi.first.wpilibj.geometry.Pose2d;
import edu.wpi.first.wpilibj.geometry.Rotation2d;
import org.junit.jupiter.api.Test;

class QuinticHermiteSplineTest {
  private void run(Pose2d a, Pose2d b) {
    var start = System.nanoTime();
    var spline = new QuinticHermiteSpline(a, b);
    spline.parameterize();
    var end = System.nanoTime();

    System.out.println((end - start) / 1000.0);
  }

  @Test
  void testStraightLine() {
    for (int i = 0; i < 100; i++) {
      run(new Pose2d(), new Pose2d(3, 0, new Rotation2d()));
    }
  }

  @Test
  void testSimpleSCurve() {
    for (int i = 0; i < 100; i++) {
      run(new Pose2d(), new Pose2d(1, 1, new Rotation2d()));
    }
  }

  @Test
  void testSquiggly() {
    for (int i = 0; i < 100; i++) {
      run(new Pose2d(0, 0, Rotation2d.fromDegrees(90)), new Pose2d(-1, 0, Rotation2d.fromDegrees(90)));
    }
  }
}
