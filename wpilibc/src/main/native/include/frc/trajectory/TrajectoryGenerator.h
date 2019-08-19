/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "constraints/TrajectoryConstraint.h"
#include "frc/spline/CubicHermiteSpline.h"
#include "frc/spline/QuinticHermiteSpline.h"
#include "frc/trajectory/Trajectory.h"

namespace frc {
/**
 * Helper class used to generate trajectories with various constraints.
 */
class TrajectoryGenerator {
 public:
  using PoseWithCurvature = std::pair<Pose2d, double>;

  /**
   * Generates a trajectory with the given waypoints and constraints.
   *
   * @param waypoints A vector of points that the trajectory must go through.
   * @param constraints A vector of various velocity and acceleration
   * constraints.
   * @param startVelocity The start velocity for the trajectory.
   * @param endVelocity The end velocity for the trajectory.
   * @param maxVelocity The max velocity for the trajectory.
   * @param maxAcceleration The max acceleration for the trajectory.
   * @param reversed Whether the robot should move backwards. Note that the
   * robot will still move from a -> b -> ... -> z as defined in the waypoints.
   *
   * @return The trajectory.
   */
  static Trajectory GenerateTrajectory(
      std::vector<Pose2d> waypoints,
      std::vector<std::unique_ptr<TrajectoryConstraint>>&& constraints,
      units::meters_per_second_t startVelocity,
      units::meters_per_second_t endVelocity,
      units::meters_per_second_t maxVelocity,
      units::meters_per_second_squared_t maxAcceleration, bool reversed);

  /**
   * Creates a clamped cubic spline from the waypoints.  Headings are only 
   * specified at the start and end of the spline; waypoints in-between are 
   * position-only.
   */
  static std::vector<PoseWithCurvature> SplinePointsFromWaypoints(
      Pose2d start, std::vector<Translation2d> waypoints, Pose2d end);

 private:
  constexpr static double kEpsilon = 1E-6;

  struct ConstrainedState {
    PoseWithCurvature pose = {Pose2d(), 0.0};
    units::meter_t distance;
    units::meters_per_second_t maxVelocity;
    units::meters_per_second_squared_t minAcceleration;
    units::meters_per_second_squared_t maxAcceleration;
  };

  /**
   * Thomas algorithm for solving tridiagonal systems Af = d.
   * 
   * @param a the values of A above the diagonal
   * @param b the values of A on the diagonal
   * @param c the values of A below the diagonal
   * @param d the vector on the rhs
   * @param f the unknown (solution) vector, modified in-place
   */
  static void thomas_algorithm(const std::vector<double>& a,
                      const std::vector<double>& b,
                      const std::vector<double>& c,
                      const std::vector<double>& d,
                      std::vector<double>& f);

  /**
   * Creates splines from the waypoints and parameterizes the splines to get a
   * vector of poses and curvatures.
   */
  static std::vector<PoseWithCurvature> SplinePointsFromWaypoints(
      std::vector<Pose2d> poses);

  /**
   * Parameterize the trajectory by time. This is where the velocity profile is
   * generated.
   */
  static Trajectory TimeParameterizeTrajectory(
      std::vector<PoseWithCurvature> points,
      std::vector<std::unique_ptr<TrajectoryConstraint>>&& constraints,
      units::meters_per_second_t startVelocity,
      units::meters_per_second_t endVelocity,
      units::meters_per_second_t maxVelocity,
      units::meters_per_second_squared_t maxAcceleration, bool reversed);

  /**
   * Enforces acceleration limits as defined by the constraints. This function
   * is used when time parameterizing a trajectory.
   */
  static void EnforceAccelerationLimits(
      bool reverse,
      const std::vector<std::unique_ptr<TrajectoryConstraint>>& constraints,
      ConstrainedState* state);
};
}  // namespace frc
