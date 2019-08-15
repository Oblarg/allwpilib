/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <limits>

#include <units/units.h>

#include "frc/geometry/Pose2d.h"

namespace frc {
class TrajectoryConstraint {
 public:
  struct MinMax {
    units::meters_per_second_squared_t minAcceleration =
        units::meters_per_second_squared_t(std::numeric_limits<double>::min());
    units::meters_per_second_squared_t maxAcceleration =
        units::meters_per_second_squared_t(std::numeric_limits<double>::max());
  };

  virtual units::meters_per_second_t MaxVelocity(const Pose2d& pose,
                                                 double curvature) = 0;
  virtual MinMax MinMaxAcceleration(const Pose2d& pose, double curvature,
                                    units::meters_per_second_t speed) = 0;
};
}  // namespace frc
