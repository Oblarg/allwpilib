/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/spline/CubicHermiteSpline.h"

using namespace frc;

CubicHermiteSpline::CubicHermiteSpline(double x0, double dx0, double x1,
                                       double dx1, double y0, double dy0,
                                       double y1, double dy1) {
  // Make control vectors and hermite basis.
  auto [x, y] = MakeControlVectors(x0, dx0, x1, dx1, y0, dy0, y1, dy1);
  auto hermite = MakeHermiteBasis();

  // Populate first two rows with coefficients.
  m_coefficients.template block<1, 4>(0, 0) = hermite * x;
  m_coefficients.template block<1, 4>(1, 0) = hermite * y;

  // Populate Row 2 and Row 3 with the derivatives of the equations above.
  // Then populate row 4 and 5 with the second derivatives.
  for (int i = 0; i < 4; i++) {
    m_coefficients.template block<2, 1>(2, i) =
        m_coefficients.template block<2, 1>(0, i) * (3 - i);

    m_coefficients.template block<2, 1>(4, i) =
        m_coefficients.template block<2, 1>(2, i) * (3 - i);
  }
}
