/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/trajectory/TrajectoryGenerator.h"

#include <utility>
#include <iostream>

using namespace frc;

Trajectory TrajectoryGenerator::GenerateTrajectory(
    std::vector<Pose2d> waypoints,
    std::vector<std::unique_ptr<TrajectoryConstraint>>&& constraints,
    units::meters_per_second_t startVelocity,
    units::meters_per_second_t endVelocity,
    units::meters_per_second_t maxVelocity,
    units::meters_per_second_squared_t maxAcceleration, bool reversed) {
  const Transform2d flip{Translation2d(), Rotation2d(180_deg)};

  // Make theta normal for trajectory generation if path is reversed.
  std::vector<Pose2d> newWaypoints;
  for (auto&& point : waypoints) {
    newWaypoints.emplace_back(reversed ? point + flip : point);
  }

  auto points = SplinePointsFromWaypoints(newWaypoints);

  // After trajectory generation, flip theta back so it's relative to the
  // field. Also fix curvature.
  if (reversed) {
    for (auto& point : points) {
      point = {point.first + flip, -point.second};
    }
  }

  return TimeParameterizeTrajectory(points, std::move(constraints),
                                    startVelocity, endVelocity, maxVelocity,
                                    maxAcceleration, reversed);
}

std::vector<TrajectoryGenerator::PoseWithCurvature>
TrajectoryGenerator::SplinePointsFromWaypoints(std::vector<Pose2d> poses) {
  // Create the vector of spline points.
  std::vector<PoseWithCurvature> splinePoints;

  // Add the first point.
  splinePoints.emplace_back(poses.front(), 0.0);

  for (int i = 0; i < poses.size() - 1; i++) {
    // Create the spline.
    const QuinticHermiteSpline spline{poses[i], poses[i + 1]};
    // Get the array of poses.
    auto points = spline.Parameterize();
    // Append the array of poses to the vector. We are removing the first point
    // because it's a duplicate of the last point from the previous spline.
    splinePoints.insert(std::end(splinePoints), std::begin(points) + 1,
                        std::end(points));
  }
  return splinePoints;
}

std::vector<TrajectoryGenerator::PoseWithCurvature>
TrajectoryGenerator::SplinePointsFromWaypoints(Pose2d start,
  std::vector<Translation2d> waypoints, Pose2d end) {
  // Create the vector of spline points.
  std::vector<PoseWithCurvature> splinePoints;

  // Add the first point.
  splinePoints.emplace_back(start, 0.0);

  double scalar; 
  // This just makes the splines look better.
  if (waypoints.empty()) {
    scalar =
        1.2 * start.Translation().Distance(end.Translation()).to<double>();
  }
  else {
    scalar = 1.2 * start.Translation().Distance(waypoints.front()).to<double>();
  }

  std::vector<double> startXControlVector{start.Translation().X().to<double>(),
      start.Rotation().Cos() * scalar, 0.0};

  std::vector<double> startYControlVector{start.Translation().Y().to<double>(),
      start.Rotation().Sin() * scalar, 0.0};

  // This just makes the splines look better.
  if (!waypoints.empty()) {
    scalar = 1.2 * end.Translation().Distance(waypoints.back()).to<double>();
  }

  std::vector<double> endXControlVector{end.Translation().X().to<double>(),
      end.Rotation().Cos() * scalar, 0.0};

  std::vector<double> endYControlVector{end.Translation().Y().to<double>(),
      end.Rotation().Sin() * scalar, 0.0};

  if (!waypoints.empty()) {
    waypoints.emplace(waypoints.begin(), start.Translation());
    waypoints.emplace_back(end.Translation());

    std::vector<double> a;
    std::vector<double> b(waypoints.size() - 2, 4.0);
    std::vector<double> c;
    std::vector<double> dx, dy;
    std::vector<double> fx(waypoints.size() - 2, 0.0), fy(waypoints.size() - 2, 0.0);
    
    a.emplace_back(0);
    for (int i = 0; i < waypoints.size() - 3; i++) {
      a.emplace_back(1);
      c.emplace_back(1);
    }
    c.emplace_back(0);

    dx.emplace_back(3 *
     (waypoints[2].X().to<double>() - waypoints[0].X().to<double>()) - startXControlVector[1]);
    dy.emplace_back(3 *
     (waypoints[2].Y().to<double>() - waypoints[0].Y().to<double>()) - startYControlVector[1]);
    if (waypoints.size() > 4) {
      for (int i = 1; i <= waypoints.size() - 4; i++) {
        dx.emplace_back(3 * (waypoints[i+1].X().to<double>() - waypoints[i-1].X().to<double>()));
        dy.emplace_back(3 * (waypoints[i+1].Y().to<double>() - waypoints[i-1].Y().to<double>()));
      }
    }
    dx.emplace_back(3 *
     (waypoints[waypoints.size() - 1].X().to<double>() - waypoints[waypoints.size() - 3].X().to<double>()) - endXControlVector[1]);
    dy.emplace_back(3 *
     (waypoints[waypoints.size() - 1].Y().to<double>() - waypoints[waypoints.size() - 3].Y().to<double>()) - endYControlVector[1]);

    thomas_algorithm(a, b, c, dx, fx);
    thomas_algorithm(a, b, c, dy, fy);

    fx.emplace(fx.begin(), startXControlVector[1]);
    fx.emplace_back(endXControlVector[1]);
    fy.emplace(fy.begin(), startYControlVector[1]);
    fy.emplace_back(endYControlVector[1]);

    for (int i = 0; i < fx.size() - 1; i++) {
      // Create the spline.
      std::cout << "FX " << fx[i] << " " << fx[i+1] << std::endl;
      std::cout << "FY " << fy[i] << " " << fy[i+1] << std::endl;
      const CubicHermiteSpline spline{waypoints[i].X().to<double>(), 
                                      fx[i],
                                      waypoints[i+1].X().to<double>(),
                                      fx[i+1],
                                      waypoints[i].Y().to<double>(),
                                      fy[i],
                                      waypoints[i+1].Y().to<double>(),
                                      fy[i+1]};

      // Get the array of poses.
      std::vector<PoseWithCurvature> points;

      for (double t = 0; t <= 1; t+=.01) {
        points.emplace_back(spline.GetPoint(t));
      }

      // Append the array of poses to the vector. We are removing the first point
      // because it's a duplicate of the last point from the previous spline.
      splinePoints.insert(std::end(splinePoints), std::begin(points) + 1,
                          std::end(points));
    }
  }
  else {
    // Create the spline.
    const CubicHermiteSpline spline{startXControlVector[0], 
                                    startXControlVector[1],
                                    endXControlVector[0],
                                    endXControlVector[1],
                                    startYControlVector[0],
                                    startYControlVector[1],
                                    endYControlVector[0],
                                    endYControlVector[1]};
    // Get the array of poses.
    auto points = spline.Parameterize();
    // Append the array of poses to the vector. We are removing the first point
    // because it's a duplicate of the last point from the previous spline.
    splinePoints.insert(std::end(splinePoints), std::begin(points) + 1,
                        std::end(points));
  }

  return splinePoints;
}

// Vectors a, b, c and d are const. They will not be modified                                                                                                                                                        
// by the function. Vector f (the solution vector) is non-const                                                                                                                                                      
// and thus will be calculated and updated by the function.                                                                                                                                                          
void TrajectoryGenerator::thomas_algorithm(const std::vector<double>& a,
                      const std::vector<double>& b,
                      const std::vector<double>& c,
                      const std::vector<double>& d,
                      std::vector<double>& f) {
  size_t N = d.size();

  // Create the temporary vectors                                                                                                                                                                                    
  // Note that this is inefficient as it is possible to call                                                                                                                                                         
  // this function many times. A better implementation would                                                                                                                                                         
  // pass these temporary matrices by non-const reference to                                                                                                                                                         
  // save excess allocation and deallocation                                                                                                                                                                         
  std::vector<double> c_star(N, 0.0);
  std::vector<double> d_star(N, 0.0);

  // This updates the coefficients in the first row                                                                                                                                                                  
  // Note that we should be checking for division by zero here                                                                                                                                                       
  c_star[0] = c[0] / b[0];
  d_star[0] = d[0] / b[0];

  // Create the c_star and d_star coefficients in the forward sweep                                                                                                                                                  
  for (int i=1; i<N; i++) {
    double m = 1.0 / (b[i] - a[i] * c_star[i-1]);
    std::cout << "IN_FOR:" << a[i] << " " << b[i] << " " << c[i] << std::endl;
    c_star[i] = c[i] * m;
    d_star[i] = (d[i] - a[i] * d_star[i-1]) * m;
  }

  f[N-1] = d_star[N-1];
  // This is the reverse sweep, used to update the solution vector f                                                                                                                                                 
  for (int i=N-2; i >= 0; i--) {
    std::cout << "IN_FOR2:" << d_star[i] << " " << d[i+1] << " " << c_star[i] << std::endl;
    f[i] = d_star[i] - c_star[i] * f[i+1];
  }
}



Trajectory TrajectoryGenerator::TimeParameterizeTrajectory(
    std::vector<PoseWithCurvature> points,
    std::vector<std::unique_ptr<TrajectoryConstraint>>&& constraints,
    units::meters_per_second_t startVelocity,
    units::meters_per_second_t endVelocity,
    units::meters_per_second_t maxVelocity,
    units::meters_per_second_squared_t maxAcceleration, bool reversed) {
  std::vector<ConstrainedState> constrainedStates(points.size());

  ConstrainedState predecessor{points.front(), 0_m, startVelocity,
                               -maxAcceleration, maxAcceleration};

  constrainedStates[0] = predecessor;

  // Forward pass
  for (int i = 0; i < points.size(); i++) {
    auto& constrainedState = constrainedStates[i];
    constrainedState.pose = points[i];

    // Begin constraining based on predecessor
    units::meter_t ds = constrainedState.pose.first.Translation().Distance(
        predecessor.pose.first.Translation());
    constrainedState.distance = ds + predecessor.distance;

    // We may need to iterate to find the maximum end velocity and common
    // acceleration, since acceleration limits may be a function of velocity.
    while (true) {
      // Enforce global max velocity and max reachable velocity by global
      // acceleration limit. vf = std::sqrt(vi^2 + 2*a*d).

      constrainedState.maxVelocity = units::math::min(
          maxVelocity,
          units::math::sqrt(predecessor.maxVelocity * predecessor.maxVelocity -
                            predecessor.maxAcceleration * ds * 2.0));

      constrainedState.minAcceleration = -maxAcceleration;
      constrainedState.maxAcceleration = maxAcceleration;

      // At this point, the constrained state is fully constructed apart from
      // all the custom-defined user constraints.
      for (const auto& constraint : constraints) {
        constrainedState.maxVelocity = units::math::min(
            constrainedState.maxVelocity,
            constraint->MaxVelocity(constrainedState.pose.first,
                                    constrainedState.pose.second));
      }

      // Now enforce all acceleration limits.
      EnforceAccelerationLimits(reversed, constraints, &constrainedState);

      if (ds.to<double>() < kEpsilon) break;

      // If the actual acceleration for this state is higher than the max
      // acceleration that we applied, then we need to reduce the max
      // acceleration of the predecessor and try again.
      units::meters_per_second_squared_t actualAcceleration =
          (constrainedState.maxVelocity * constrainedState.maxVelocity -
           predecessor.maxVelocity * predecessor.maxVelocity) /
          (ds * 2.0);

      // If we violate the max acceleration constraint, let's modify the
      // predecessor.
      if (constrainedState.maxAcceleration < actualAcceleration - 1E-6_mps_sq) {
        predecessor.maxAcceleration = constrainedState.maxAcceleration;
      } else {
        // Constrain the predecessor's max acceleration to the current
        // acceleration.
        if (actualAcceleration > predecessor.minAcceleration + 1E-6_mps_sq) {
          predecessor.maxAcceleration = actualAcceleration;
        }
        // If the actual acceleration is less than the predecessor's min
        // acceleration, it will be repaired in the backward pass.
        break;
      }
    }
    predecessor = constrainedState;
  }

  ConstrainedState successor{points.back(), constrainedStates.back().distance,
                             endVelocity, -maxAcceleration, maxAcceleration};

  // Backward pass
  for (int i = points.size() - 1; i >= 0; i--) {
    auto& constrainedState = constrainedStates[i];
    units::meter_t ds =
        constrainedState.distance - successor.distance;  // negative

    while (true) {
      // Enforce max velocity limit (reverse)
      // vf = std::sqrt(vi^2 + 2*a*d), where vi = successor.
      units::meters_per_second_t newMaxVelocity =
          units::math::sqrt(successor.maxVelocity * successor.maxVelocity +
                            successor.minAcceleration * ds * 2.0);

      // No more limits to impose! This state can be finalized.
      if (newMaxVelocity >= constrainedState.maxVelocity) break;

      constrainedState.maxVelocity = newMaxVelocity;

      // Check all acceleration constraints with the new max velocity.
      EnforceAccelerationLimits(true, constraints, &constrainedState);

      if (ds.to<double>() > -kEpsilon) break;

      // If the actual acceleration for this state is lower than the min
      // acceleration, then we need to lower the min acceleration of the
      // successor and try again.
      units::meters_per_second_squared_t actualAcceleration =
          (constrainedState.maxVelocity * constrainedState.maxVelocity -
           successor.maxVelocity * successor.maxVelocity) /
          (ds * 2.0);
      if (constrainedState.minAcceleration > actualAcceleration + 1E-6_mps_sq) {
        successor.minAcceleration = constrainedState.minAcceleration;
      } else {
        successor.minAcceleration = actualAcceleration;
        break;
      }
    }
    successor = constrainedState;
  }

  // Now we can integrate the constrained states forward in time to obtain our
  // trajectory states.

  std::vector<Trajectory::State> states(points.size());
  units::second_t t = 0_s;
  units::meter_t s = 0_m;
  units::meters_per_second_t v = 0_mps;

  for (int i = 0; i < constrainedStates.size(); i++) {
    auto state = constrainedStates[i];

    // Calculate the change in position between the current state and the
    // previous state.
    units::meter_t ds = state.distance - s;

    // Calculate the acceleration between the current state and the previous
    // state.
    units::meters_per_second_squared_t accel =
        (state.maxVelocity * state.maxVelocity - v * v) / (ds * 2);

    // Calculate dt.
    units::second_t dt = 0_s;
    if (i > 0) {
      states.at(i - 1).acceleration = reversed ? -accel : accel;
      if (units::math::abs(accel) > 1E-6_mps_sq) {
        // v_f = v_0 + a * t
        dt = (state.maxVelocity - v) / accel;
      } else {
        // delta_x = v * t
        dt = ds / v;
      }
    }

    v = state.maxVelocity;
    s = state.distance;

    states[i] = {t, reversed ? -v : v, reversed ? -accel : accel,
                 state.pose.first, state.pose.second};
  }

  return Trajectory(states);
}

void TrajectoryGenerator::EnforceAccelerationLimits(
    bool reverse,
    const std::vector<std::unique_ptr<TrajectoryConstraint>>& constraints,
    ConstrainedState* state) {
  for (auto&& constraint : constraints) {
    double factor = reverse ? -1.0 : 1.0;

    auto minMaxAccel = constraint->MinMaxAcceleration(
        state->pose.first, state->pose.second, state->maxVelocity * factor);

    state->minAcceleration = units::math::max(
        state->minAcceleration,
        reverse ? -minMaxAccel.maxAcceleration : minMaxAccel.minAcceleration);

    state->maxAcceleration = units::math::min(
        state->maxAcceleration,
        reverse ? -minMaxAccel.minAcceleration : minMaxAccel.maxAcceleration);
  }
}
