/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <array>

#include <Eigen/Core>

#include "frc/spline/Spline.h"

namespace frc {
/**
 * Represents a hermite spline of degree 3.
 */
class CubicHermiteSpline : public Spline<3> {
 public:
  /**
   * Constructs a CubicHermiteSpline with the given start and end points and
   * their first derivatives.
   *
   * @param x0 Starting x coordinate.
   * @param dx0 First derivative in the x-dimension for the starting point.
   * @param x1 Ending x coordinate.
   * @param dx1 First derivative in the x-dimension for the ending point.
   * @param y0 Starting y coordinate.
   * @param dy0 First derivative in the y-dimension for the starting point.
   * @param y1 Ending y coordinate.
   * @param dy1 First derivative in the y-dimension for the ending point.
   */
  CubicHermiteSpline(double x0, double dx0, double x1, double dx1, double y0,
                     double dy0, double y1, double dy1);

 protected:
  /**
   * Returns the coefficients matrix.
   * @return The coefficients matrix.
   */
  Eigen::Matrix<double, 6, 3 + 1> Coefficients() const override {
    return m_coefficients;
  }

 private:
  Eigen::Matrix<double, 6, 4> m_coefficients;

  /**
   * Returns the hermite basis matrix for cubic hermite spline interpolation.
   * @return The hermite basis matrix for cubic hermite spline interpolation.
   */
  static Eigen::Matrix<double, 6, 4> MakeHermiteBasis() {
    // clang-format off
    static auto basis = (Eigen::Matrix<double, 6, 4>() <<
     +2.0, +1.0, +1.0, -2.0,
     -3.0, -2.0, -1.0, +3.0,
     +0.0, +1.0, +0.0, +0.0,
     +1.0, +0.0, +0.0, +0.0).finished();
    // clang-format on
    return basis;
  }

  /**
   * Converts the constructor arguments to control vectors to calculate the
   * spline coefficients.
   *
   * @param x0 Starting x coordinate.
   * @param dx0 First derivative in the x-dimension for the starting point.
   * @param x1 Ending x coordinate.
   * @param dx1 First derivative in the x-dimension for the ending point.
   * @param y0 Starting y coordinate.
   * @param dy0 First derivative in the y-dimension for the starting point.
   * @param y1 Ending y coordinate.
   * @param dy1 First derivative in the y-dimension for the ending point.
   *
   * @return Control vectors.
   */
  static std::array<Eigen::Matrix<double, 4, 1>, 2> MakeControlVectors(
      double x0, double dx0, double x1, double dx1, double y0, double dy0,
      double y1, double dy1) {
    Eigen::Matrix<double, 4, 1> xControlMatrix;
    xControlMatrix << x0, dx0, dx1, x1;

    Eigen::Matrix<double, 4, 1> yControlMatrix;
    yControlMatrix << y0, dy0, dy1, y1;

    return {xControlMatrix, yControlMatrix};
  }
};
}  // namespace frc
