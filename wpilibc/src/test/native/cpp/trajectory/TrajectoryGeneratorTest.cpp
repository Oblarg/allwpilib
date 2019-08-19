/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <chrono>

#include "frc/trajectory/Trajectory.h"
#include "frc/trajectory/TrajectoryGenerator.h"
#include "frc/trajectory/constraints/TrajectoryConstraint.h"
#include "gtest/gtest.h"

using namespace frc;

class CentripetalAccelerationConstraint : public TrajectoryConstraint {
 public:
  explicit CentripetalAccelerationConstraint(
      units::meters_per_second_squared_t maxCentripetalAcceleration)
      : m_maxCentripetalAcceleration(maxCentripetalAcceleration) {}

  units::meters_per_second_t MaxVelocity(const Pose2d& pose,
                                         double curvature) override {
    // a_c = v^2 / r
    // v^2 = a_c * r
    // v^2 = a_c / curvature (1 / r = curvature)
    // v = std::sqrt(a_c / curvature)

    // We need to multiply by 1 meter here because curvature has units of 1/m.
    return units::math::sqrt(m_maxCentripetalAcceleration / curvature * 1_m);
  }

  struct MinMax MinMaxAcceleration(const Pose2d& pose, double curvature,
                                   units::meters_per_second_t speed) override {
    return MinMax();
  }

 private:
  units::meters_per_second_squared_t m_maxCentripetalAcceleration;
};

TEST(TrajectoryGenerationTest, ObeysConstraints) {
  units::meters_per_second_t startVelocity = 0_mps, endVelocity = 0_mps;
  units::meters_per_second_t maxVelocity = 12_fps;
  units::meters_per_second_squared_t maxAcceleration = 7_fps_sq;
  units::meters_per_second_squared_t maxCentripetalAcceleration = 7_fps_sq;

  std::vector<std::unique_ptr<TrajectoryConstraint>> constraints;

  constraints.emplace_back(std::make_unique<CentripetalAccelerationConstraint>(
      maxCentripetalAcceleration));

  // 2018 cross scale auto waypoints
  Pose2d sideStart{1.54_ft, 23.23_ft, Rotation2d(180_deg)};
  Pose2d crossScale{23.7_ft, 6.8_ft, Rotation2d(-160_deg)};

  auto start = std::chrono::high_resolution_clock::now();
  Trajectory trajectory = TrajectoryGenerator::GenerateTrajectory(
      std::vector<Pose2d>{
          sideStart,
          sideStart + Transform2d{Translation2d(-13_ft, 0_ft), Rotation2d()},
          sideStart +
              Transform2d{Translation2d(-19.5_ft, 5.0_ft), Rotation2d(-90_deg)},
          sideStart +
              Transform2d{Translation2d(-19.5_ft, 14_ft), Rotation2d(-90_deg)},
          crossScale},
      std::move(constraints), startVelocity, endVelocity, maxVelocity,
      maxAcceleration, true);
  auto finish = std::chrono::high_resolution_clock::now();

  auto timetaken =
      std::chrono::duration_cast<std::chrono::microseconds>(finish - start);

  std::cout << timetaken.count() << std::endl;

  units::second_t time = 0_s;
  units::second_t dt = 20_ms;
  units::second_t duration = trajectory.TotalTime();

  while (time < duration) {
    const Trajectory::State point = trajectory.Sample(time);
    double a_c = std::pow(point.velocity.to<double>(), 2) * point.curvature;

    EXPECT_TRUE(a_c <= maxCentripetalAcceleration.to<double>() + 0.01);
    EXPECT_TRUE(units::math::abs(point.velocity) <= maxVelocity + 0.01_fps);
    EXPECT_TRUE(units::math::abs(point.acceleration) <=
                maxAcceleration + 0.01_fps_sq);

    time += dt;
  }
}

TEST(ClampedSplineTest, ClampedSplineGenTest) {
  Pose2d start{0_m, 0_m, Rotation2d(90_deg)};
  std::vector<Translation2d> waypoints{
    Translation2d(1_m, 1_m),
    Translation2d(2_m, -1_m)
  };
  Pose2d end{3_m, 0_m, Rotation2d{90_deg}};

  auto spline = TrajectoryGenerator::SplinePointsFromWaypoints(start, waypoints, end);

  for (auto&& pose : spline) {
    std::cout << pose.first.Translation().X().to<double>() 
      << "," << pose.first.Translation().Y().to<double> () << std::endl;
  }
}
