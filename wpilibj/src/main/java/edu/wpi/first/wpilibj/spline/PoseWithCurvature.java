package edu.wpi.first.wpilibj.spline;

import edu.wpi.first.wpilibj.geometry.Pose2d;

@SuppressWarnings("MemberName")
public class PoseWithCurvature {
  public Pose2d pose;
  public double curvature;

  public PoseWithCurvature(Pose2d pose, double curvature) {
    this.pose = pose;
    this.curvature = curvature;
  }
}
