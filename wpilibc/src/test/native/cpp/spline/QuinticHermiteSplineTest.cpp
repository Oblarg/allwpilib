/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <chrono>
#include <iostream>

#include <units/units.h>

#include "frc/spline/QuinticHermiteSpline.h"
#include "gtest/gtest.h"

using namespace frc;

static constexpr double kEpsilon = 1E-9;

namespace frc {
class QuinticHermiteSplineTest : public ::testing::Test {
 protected:
  static void Run(const Pose2d& a, const Pose2d& b) {
    auto start = std::chrono::high_resolution_clock::now();

    const QuinticHermiteSpline spline{a, b};
    const auto poses = spline.Parameterize();

    auto finish = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(finish - start);

    std::cout << duration.count() << std::endl;
  }
};
}  // namespace frc

TEST_F(QuinticHermiteSplineTest, StraightLine) {
  Run(Pose2d(), Pose2d(3_m, 0_m, Rotation2d()));
}

TEST_F(QuinticHermiteSplineTest, SimpleSCurve) {
  Run(Pose2d(), Pose2d(1_m, 1_m, Rotation2d()));
}

TEST_F(QuinticHermiteSplineTest, SquigglyCurve) {
  Run(Pose2d(0_m, 0_m, Rotation2d(90_deg)),
      Pose2d(-1_m, 0_m, Rotation2d(90_deg)));
}
