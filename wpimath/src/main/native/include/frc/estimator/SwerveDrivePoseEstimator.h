// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <limits>

#include <wpi/array.h>
#include <wpi/sendable/Sendable.h>
#include <wpi/sendable/SendableBuilder.h>
#include <wpi/sendable/SendableHelper.h>
#include <wpi/timestamp.h>

#include "Eigen/Core"
#include "frc/StateSpaceUtil.h"
#include "frc/estimator/AngleStatistics.h"
#include "frc/estimator/KalmanFilterLatencyCompensator.h"
#include "frc/estimator/UnscentedKalmanFilter.h"
#include "frc/geometry/Pose2d.h"
#include "frc/geometry/Rotation2d.h"
#include "frc/kinematics/SwerveDriveKinematics.h"
#include "units/time.h"

namespace frc {
/**
 * This class wraps an Unscented Kalman Filter to fuse latency-compensated
 * vision measurements with swerve drive encoder velocity measurements. It will
 * correct for noisy measurements and encoder drift. It is intended to be an
 * easy but more accurate drop-in for SwerveDriveOdometry.
 *
 * Update() should be called every robot loop. If your loops are faster or
 * slower than the default of 0.02s, then you should change the nominal delta
 * time by specifying it in the constructor.
 *
 * AddVisionMeasurement() can be called as infrequently as you want; if you
 * never call it, then this class will behave mostly like regular encoder
 * odometry.
 *
 * The state-space system used internally has the following states (x), inputs
 * (u), and outputs (y):
 *
 * <strong> x = [x, y, theta]ᵀ </strong> in the field coordinate system
 * containing x position, y position, and heading.
 *
 * <strong> u = [v_l, v_r, dtheta]ᵀ </strong> containing left wheel velocity,
 * right wheel velocity, and change in gyro heading.
 *
 * <strong> y = [x, y, theta]ᵀ </strong> from vision containing x position, y
 * position, and heading; or <strong> y = [theta]ᵀ </strong> containing gyro
 * heading.
 */
template <size_t NumModules>
class SwerveDrivePoseEstimator
    : public wpi::Sendable,
      public wpi::SendableHelper<SwerveDrivePoseEstimator<NumModules>> {
 public:
  /**
   * Constructs a SwerveDrivePoseEstimator.
   *
   * @param gyroAngle                The current gyro angle.
   * @param initialPose              The starting pose estimate.
   * @param kinematics               A correctly-configured kinematics object
   *                                 for your drivetrain.
   * @param stateStdDevs             Standard deviations of model states.
   *                                 Increase these numbers to trust your
   *                                 model's state estimates less. This matrix
   *                                 is in the form [x, y, theta]ᵀ, with units
   *                                 in meters and radians.
   * @param localMeasurementStdDevs  Standard deviations of the encoder and gyro
   *                                 measurements. Increase these numbers to
   *                                 trust sensor readings from encoders
   *                                 and gyros less. This matrix is in the form
   *                                 [theta], with units in radians.
   * @param visionMeasurementStdDevs Standard deviations of the vision
   *                                 measurements. Increase these numbers to
   *                                 trust global measurements from vision
   *                                 less. This matrix is in the form
   *                                 [x, y, theta]ᵀ, with units in meters and
   *                                 radians.
   * @param nominalDt                The time in seconds between each robot
   *                                 loop.
   */
  SwerveDrivePoseEstimator(
      const Rotation2d& gyroAngle, const Pose2d& initialPose,
      SwerveDriveKinematics<NumModules>& kinematics,
      const wpi::array<double, 3>& stateStdDevs,
      const wpi::array<double, 1>& localMeasurementStdDevs,
      const wpi::array<double, 3>& visionMeasurementStdDevs,
      units::second_t nominalDt = 0.02_s)
      : m_observer([](const Eigen::Vector<double, 3>& x,
                      const Eigen::Vector<double, 3>& u) { return u; },
                   [](const Eigen::Vector<double, 3>& x,
                      const Eigen::Vector<double, 3>& u) {
                     return x.block<1, 1>(2, 0);
                   },
                   stateStdDevs, localMeasurementStdDevs,
                   frc::AngleMean<3, 3>(2), frc::AngleMean<1, 3>(0),
                   frc::AngleResidual<3>(2), frc::AngleResidual<1>(0),
                   frc::AngleAdd<3>(2), nominalDt),
        m_kinematics(kinematics),
        m_nominalDt(nominalDt) {
    SetVisionMeasurementStdDevs(visionMeasurementStdDevs);

    // Create correction mechanism for vision measurements.
    m_visionCorrect = [&](const Eigen::Vector<double, 3>& u,
                          const Eigen::Vector<double, 3>& y) {
      m_observer.Correct<3>(
          u, y,
          [](const Eigen::Vector<double, 3>& x,
             const Eigen::Vector<double, 3>& u) { return x; },
          m_visionContR, frc::AngleMean<3, 3>(2), frc::AngleResidual<3>(2),
          frc::AngleResidual<3>(2), frc::AngleAdd<3>(2));
    };

    // Set initial state.
    m_observer.SetXhat(PoseTo3dVector(initialPose));

    // Calculate offsets.
    m_gyroOffset = initialPose.Rotation() - gyroAngle;
    m_previousAngle = initialPose.Rotation();
  }

  /**
   * Resets the robot's position on the field.
   *
   * You NEED to reset your encoders (to zero) when calling this method.
   *
   * The gyroscope angle does not need to be reset in the user's robot code.
   * The library automatically takes care of offsetting the gyro angle.
   *
   * @param pose      The position on the field that your robot is at.
   * @param gyroAngle The angle reported by the gyroscope.
   */
  void ResetPosition(const Pose2d& pose, const Rotation2d& gyroAngle) {
    // Reset state estimate and error covariance
    m_observer.Reset();
    m_latencyCompensator.Reset();

    m_observer.SetXhat(PoseTo3dVector(pose));

    m_gyroOffset = pose.Rotation() - gyroAngle;
    m_previousAngle = pose.Rotation();
  }

  /**
   * Gets the pose of the robot at the current time as estimated by the Extended
   * Kalman Filter.
   *
   * @return The estimated robot pose in meters.
   */
  Pose2d GetEstimatedPosition() const {
    return Pose2d(m_observer.Xhat(0) * 1_m, m_observer.Xhat(1) * 1_m,
                  Rotation2d(units::radian_t{m_observer.Xhat(2)}));
  }

  /**
   * Sets the pose estimator's trust of global measurements. This might be used
   * to change trust in vision measurements after the autonomous period, or to
   * change trust as distance to a vision target increases.
   *
   * @param visionMeasurementStdDevs Standard deviations of the vision
   *                                 measurements. Increase these numbers to
   *                                 trust global measurements from vision
   *                                 less. This matrix is in the form
   *                                 [x, y, theta]ᵀ, with units in meters and
   *                                 radians.
   */
  void SetVisionMeasurementStdDevs(
      const wpi::array<double, 3>& visionMeasurementStdDevs) {
    // Create R (covariances) for vision measurements.
    m_visionContR = frc::MakeCovMatrix(visionMeasurementStdDevs);
  }

  /**
   * Add a vision measurement to the Unscented Kalman Filter. This will correct
   * the odometry pose estimate while still accounting for measurement noise.
   *
   * This method can be called as infrequently as you want, as long as you are
   * calling Update() every loop.
   *
   * @param visionRobotPose The pose of the robot as measured by the vision
   *                        camera.
   * @param timestamp       The timestamp of the vision measurement in seconds.
   *                        Note that if you don't use your own time source by
   *                        calling UpdateWithTime() then you must use a
   *                        timestamp with an epoch since FPGA startup
   *                        (i.e. the epoch of this timestamp is the same
   *                        epoch as Timer#GetFPGATimestamp.) This means
   *                        that you should use Timer#GetFPGATimestamp as your
   *                        time source or sync the epochs.
   */
  void AddVisionMeasurement(const Pose2d& visionRobotPose,
                            units::second_t timestamp) {
    m_visionPose = visionRobotPose;
    m_visionLatency = m_prevTime - timestamp;
    m_latencyCompensator.ApplyPastGlobalMeasurement<3>(
        &m_observer, m_nominalDt, PoseTo3dVector(visionRobotPose),
        m_visionCorrect, timestamp);
  }

  /**
   * Adds a vision measurement to the Unscented Kalman Filter. This will correct
   * the odometry pose estimate while still accounting for measurement noise.
   *
   * This method can be called as infrequently as you want, as long as you are
   * calling Update() every loop.
   *
   * Note that the vision measurement standard deviations passed into this
   * method will continue to apply to future measurements until a subsequent
   * call to SetVisionMeasurementStdDevs() or this method.
   *
   * @param visionRobotPose          The pose of the robot as measured by the
   *                                 vision camera.
   * @param timestamp                The timestamp of the vision measurement in
   *                                 seconds. Note that if you don't use your
   *                                 own time source by calling
   *                                 UpdateWithTime(), then you must use a
   *                                 timestamp with an epoch since FPGA startup
   *                                 (i.e. the epoch of this timestamp is the
   *                                 same epoch as
   *                                 frc::Timer::GetFPGATimestamp(). This means
   *                                 that you should use
   *                                 frc::Timer::GetFPGATimestamp() as your
   *                                 time source in this case.
   * @param visionMeasurementStdDevs Standard deviations of the vision
   *                                 measurements. Increase these numbers to
   *                                 trust global measurements from vision
   *                                 less. This matrix is in the form
   *                                 [x, y, theta]ᵀ, with units in meters and
   *                                 radians.
   */
  void AddVisionMeasurement(
      const Pose2d& visionRobotPose, units::second_t timestamp,
      const wpi::array<double, 3>& visionMeasurementStdDevs) {
    SetVisionMeasurementStdDevs(visionMeasurementStdDevs);
    AddVisionMeasurement(visionRobotPose, timestamp);
  }

  /**
   * Updates the the Unscented Kalman Filter using only wheel encoder
   * information. This should be called every loop, and the correct loop period
   * must be passed into the constructor of this class.
   *
   * @param gyroAngle    The current gyro angle.
   * @param moduleStates The current velocities and rotations of the swerve
   *                     modules.
   * @return The estimated pose of the robot in meters.
   */
  template <typename... ModuleState>
  Pose2d Update(const Rotation2d& gyroAngle, ModuleState&&... moduleStates) {
    return UpdateWithTime(units::microsecond_t(wpi::Now()), gyroAngle,
                          moduleStates...);
  }

  /**
   * Updates the the Unscented Kalman Filter using only wheel encoder
   * information. This should be called every loop, and the correct loop period
   * must be passed into the constructor of this class.
   *
   * @param currentTime  Time at which this method was called, in seconds.
   * @param gyroAngle    The current gyroscope angle.
   * @param moduleStates The current velocities and rotations of the swerve
   *                     modules.
   * @return The estimated pose of the robot in meters.
   */
  template <typename... ModuleState>
  Pose2d UpdateWithTime(units::second_t currentTime,
                        const Rotation2d& gyroAngle,
                        ModuleState&&... moduleStates) {
    auto dt = m_prevTime >= 0_s ? currentTime - m_prevTime : m_nominalDt;
    m_prevTime = currentTime;

    auto angle = gyroAngle + m_gyroOffset;
    auto omega = (angle - m_previousAngle).Radians() / dt;

    auto chassisSpeeds = m_kinematics.ToChassisSpeeds(moduleStates...);
    auto fieldRelativeSpeeds =
        Translation2d(chassisSpeeds.vx * 1_s, chassisSpeeds.vy * 1_s)
            .RotateBy(angle);

    Eigen::Vector<double, 3> u{fieldRelativeSpeeds.X().value(),
                               fieldRelativeSpeeds.Y().value(), omega.value()};

    Eigen::Vector<double, 1> localY{angle.Radians().value()};
    m_previousAngle = angle;

    m_latencyCompensator.AddObserverState(m_observer, u, localY, currentTime);

    m_observer.Predict(u, dt);
    m_observer.Correct(u, localY);

    m_estimatedPose = GetEstimatedPosition();
    return m_estimatedPose;
  }

  void InitSendable(wpi::SendableBuilder& builder) override {
    m_observer.InitSendable(builder);
    builder.SetSmartDashboardType("SwerveDrivePoseEstimator");
    builder.AddDoubleProperty(
        "VisionStdDevMetersX", [&] { return std::sqrt(m_visionContR(0, 0)); },
        [&](double stdDev) { m_visionContR(0, 0) = stdDev * stdDev; });
    builder.AddDoubleProperty(
        "VisionStdDevMetersY", [&] { return std::sqrt(m_visionContR(1, 1)); },
        [&](double stdDev) { m_visionContR(1, 1) = stdDev * stdDev; });
    builder.AddDoubleProperty(
        "VisionStdDevDegrees",
        [&] {
          return units::degree_t{
              units::radian_t{std::sqrt(m_visionContR(3, 3))}}
              .value();
        },
        [&](double stdDevDegrees) {
          double stdDevRads =
              units::radian_t{units::degree_t{stdDevDegrees}}.value();
          m_visionContR(3, 3) = stdDevRads * stdDevRads;
        });
    builder.AddDoubleProperty(
        "VisionPoseMetersX", [&] { return m_visionPose.X().value(); }, nullptr);
    builder.AddDoubleProperty(
        "VisionPoseMetersY", [&] { return m_visionPose.Y().value(); }, nullptr);
    builder.AddDoubleProperty(
        "VisionPoseDegrees",
        [&] { return m_visionPose.Rotation().Degrees().value(); }, nullptr);
    builder.AddDoubleProperty(
        "VisionLatencyMs",
        [&] { return units::millisecond_t{m_visionLatency}.value(); }, nullptr);
    builder.AddDoubleProperty(
        "EstimatedPoseMetersX", [&] { return m_estimatedPose.X().value(); },
        nullptr);
    builder.AddDoubleProperty(
        "EstimatedPoseMetersY", [&] { return m_estimatedPose.Y().value(); },
        nullptr);
    builder.AddDoubleProperty(
        "EstimatedPoseDegrees",
        [&] { return m_estimatedPose.Rotation().Degrees().value(); }, nullptr);
  }

 private:
  UnscentedKalmanFilter<3, 3, 1> m_observer;
  SwerveDriveKinematics<NumModules>& m_kinematics;
  KalmanFilterLatencyCompensator<3, 3, 1, UnscentedKalmanFilter<3, 3, 1>>
      m_latencyCompensator;
  std::function<void(const Eigen::Vector<double, 3>& u,
                     const Eigen::Vector<double, 3>& y)>
      m_visionCorrect;

  Eigen::Matrix3d m_visionContR;

  units::second_t m_nominalDt;
  units::second_t m_prevTime = -1_s;

  Rotation2d m_gyroOffset;
  Rotation2d m_previousAngle;

  Pose2d m_visionPose;
  Pose2d m_estimatedPose;

  units::second_t m_visionLatency;

  template <int Dim>
  static wpi::array<double, Dim> StdDevMatrixToArray(
      const Eigen::Vector<double, Dim>& vector) {
    wpi::array<double, Dim> array;
    for (size_t i = 0; i < Dim; ++i) {
      array[i] = vector(i);
    }
    return array;
  }
};

}  // namespace frc
