/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <array>

#include <Eigen/Core>

#include "frc/geometry/Pose2d.h"
#include "frc/geometry/Rotation2d.h"
#include "frc/spline/Spline.h"

namespace frc {
/**
 * Represents a hermite spline of degree 5.
 */
class QuinticHermiteSpline : public Spline<5> {
 public:
  /**
   * Constructs a QuinticHermiteSpline between the two specified poses. The
   * curvature at the endpoints is zero.
   *
   * @param start The initial position on the spline.
   * @param end The final position on the spline.
   */
  QuinticHermiteSpline(const Pose2d& start, const Pose2d& end);

 protected:
  /**
   * Returns the coefficients matrix.
   * @return The coefficients matrix.
   */
  Eigen::Matrix<double, 6, 6> Coefficients() const override {
    return m_coefficients;
  }

 private:
  Eigen::Matrix<double, 6, 6> m_coefficients;

  /**
   * Returns the hermite basis matrix for quintic hermite spline interpolation.
   * @return The hermite basis matrix for quintic hermite spline interpolation.
   */
  static Eigen::Matrix<double, 6, 6> MakeHermiteBasis() {
    // clang-format off
    static const auto basis = (Eigen::Matrix<double, 6, 6>() <<
      -06.0, -03.0, -00.5, +00.5, -03.0, +06.0,
      +15.0, +08.0, +01.5, -01.0, +07.0, -15.0,
      -10.0, -06.0, -01.5, +00.5, -04.0, +10.0,
      +00.0, +00.0, +00.5, +00.0, +00.0, +00.0,
      +00.0, +01.0, +00.0, +00.0, +00.0, +00.0,
      +01.0, +00.0, +00.0, +00.0, +00.0, +00.0).finished();
    // clang-format on
    return basis;
  }

  /**
   * Converts the constructor arguments to the control vectors to calculate the
   * spline coefficients.
   *
   * @param start The starting position for this spline
   * @param end The end position for this spline
   *
   * @return Control vectors.
   */
  static std::array<Eigen::Matrix<double, 6, 1>, 2> MakeControlVectors(
      const Pose2d& start, const Pose2d& end) {
    // This just makes the splines look better.
    const auto scalar =
        1.2 * start.Translation().Distance(end.Translation()).to<double>();

    Eigen::Matrix<double, 6, 1> xControlVector;
    xControlVector << start.Translation().X().to<double>(),
        start.Rotation().Cos() * scalar, 0.0, 0.0,
        end.Rotation().Cos() * scalar, end.Translation().X().to<double>();

    Eigen::Matrix<double, 6, 1> yControlVector;
    yControlVector << start.Translation().Y().to<double>(),
        start.Rotation().Sin() * scalar, 0.0, 0.0,
        end.Rotation().Sin() * scalar, end.Translation().Y().to<double>();

    return {xControlVector, yControlVector};
  }
};
}  // namespace frc
