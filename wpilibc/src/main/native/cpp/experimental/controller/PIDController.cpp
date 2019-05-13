/*----------------------------------------------------------------------------*/
/* Copyright (c) 2008-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/experimental/controller/PIDController.h"

#include <algorithm>
#include <cmath>

#include <hal/HAL.h>

#include "frc/smartdashboard/SendableBuilder.h"

using namespace frc::experimental;

template <class T>
constexpr const T& clamp(const T& value, const T& low, const T& high) {
  return std::max(low, std::min(value, high));
}

PIDController::PIDController(double Kp, double Ki, double Kd, double period)
    : Controller(period), SendableBase(false) {
  m_Kp = Kp;
  m_Ki = Ki;
  m_Kd = Kd;

  static int instances = 0;
  instances++;
  HAL_Report(HALUsageReporting::kResourceType_PIDController, instances);
  SetName("PIDController", instances);
}

void PIDController::SetP(double Kp) {
  std::lock_guard<wpi::mutex> lock(m_thisMutex);
  m_Kp = Kp;
}

void PIDController::SetI(double Ki) {
  std::lock_guard<wpi::mutex> lock(m_thisMutex);
  m_Ki = Ki;
}

void PIDController::SetD(double Kd) {
  std::lock_guard<wpi::mutex> lock(m_thisMutex);
  m_Kd = Kd;
}

double PIDController::GetP() const {
  std::lock_guard<wpi::mutex> lock(m_thisMutex);
  return m_Kp;
}

double PIDController::GetI() const {
  std::lock_guard<wpi::mutex> lock(m_thisMutex);
  return m_Ki;
}

double PIDController::GetD() const {
  std::lock_guard<wpi::mutex> lock(m_thisMutex);
  return m_Kd;
}

double PIDController::GetOutput() const {
  std::lock_guard<wpi::mutex> lock(m_thisMutex);
  return m_output;
}

void PIDController::SetReference(double reference) {
  std::lock_guard<wpi::mutex> lock(m_thisMutex);

  if (m_maximumInput > m_minimumInput) {
    m_reference = clamp(reference, m_minimumInput, m_maximumInput);
  } else {
    m_reference = reference;
  }
}

double PIDController::GetReference() const {
  std::lock_guard<wpi::mutex> lock(m_thisMutex);
  return m_reference;
}

bool PIDController::AtReference(double tolerance, double deltaTolerance, 
    Tolerance toleranceType) const {
  double deltaError = GetDeltaError();

  std::lock_guard<wpi::mutex> lock(m_thisMutex);
  if (toleranceType == Tolerance::kPercent) {
    return std::abs(m_currError) < tolerance / 100 * m_inputRange &&
           std::abs(deltaError) < deltaTolerance / 100 * m_inputRange;
  } else {
    return std::abs(m_currError) < tolerance &&
           std::abs(deltaError) < deltaTolerance;
  }
}

bool PIDController::AtReference() const {
  return AtReference(m_tolerance, m_deltaTolerance, m_toleranceType);
}

void PIDController::SetContinuous(bool continuous) {
  std::lock_guard<wpi::mutex> lock(m_thisMutex);
  m_continuous = continuous;
}

void PIDController::SetInputRange(double minimumInput, double maximumInput) {
  {
    std::lock_guard<wpi::mutex> lock(m_thisMutex);
    m_minimumInput = minimumInput;
    m_maximumInput = maximumInput;
    m_inputRange = maximumInput - minimumInput;
  }

  SetReference(GetReference());
}

void PIDController::SetOutputRange(double minimumOutput, double maximumOutput) {
  std::lock_guard<wpi::mutex> lock(m_thisMutex);
  m_minimumOutput = minimumOutput;
  m_maximumOutput = maximumOutput;
}

void PIDController::SetAbsoluteTolerance(double tolerance,
                                         double deltaTolerance) {
  std::lock_guard<wpi::mutex> lock(m_thisMutex);
  m_toleranceType = Tolerance::kAbsolute;
  m_tolerance = tolerance;
  m_deltaTolerance = deltaTolerance;
}

void PIDController::SetPercentTolerance(double tolerance,
                                        double deltaTolerance) {
  std::lock_guard<wpi::mutex> lock(m_thisMutex);
  m_toleranceType = Tolerance::kPercent;
  m_tolerance = tolerance;
  m_deltaTolerance = deltaTolerance;
}

double PIDController::GetError() const {
  std::lock_guard<wpi::mutex> lock(m_thisMutex);
  return GetContinuousError(m_currError);
}

/**
 * Returns the change in error per second of the PIDController.
 *
 * @return The change in error per second.
 */
double PIDController::GetDeltaError() const {

  std::lock_guard<wpi::mutex> lock(m_thisMutex);
  return (m_currError - m_prevError) / GetPeriod();
}

double PIDController::Calculate(double measurement) {
  // Storage for function inputs
  double Kp;
  double Ki;
  double Kd;
  double minimumOutput;
  double maximumOutput;

  // Storage for function input-outputs
  double totalError;

  {
    std::lock_guard<wpi::mutex> lock(m_thisMutex);

    Kp = m_Kp;
    Ki = m_Ki;
    Kd = m_Kd;
    minimumOutput = m_minimumOutput;
    maximumOutput = m_maximumOutput;

    m_prevError = m_currError;
    m_currError = GetContinuousError(m_reference - measurement);
    totalError = m_totalError;
  }

  if (Ki != 0) {
    totalError = clamp(totalError + m_currError * GetPeriod(), minimumOutput / Ki,
                       maximumOutput / Ki);
  }

  double output =
      clamp(Kp * m_currError + Ki * totalError +
                Kd * (m_currError - m_prevError) / GetPeriod(),
            minimumOutput, maximumOutput);

  {
    std::lock_guard<wpi::mutex> lock(m_thisMutex);
    m_totalError = totalError;
    m_output = output;
  }

  return output;
}

double PIDController::Calculate(double measurement, double reference) {
  SetReference(reference);
  return Calculate(measurement);
}

void PIDController::Reset() {
  std::lock_guard<wpi::mutex> lock(m_thisMutex);
  m_prevError = 0;
  m_totalError = 0;
  m_output = 0;
}

void PIDController::InitSendable(SendableBuilder& builder) {
  builder.SetSmartDashboardType("PIDController");
  builder.SetSafeState([=]() { Reset(); });
  builder.AddDoubleProperty("Kp", [=] { return GetP(); },
                            [=](double value) { SetP(value); });
  builder.AddDoubleProperty("Ki", [=] { return GetI(); },
                            [=](double value) { SetI(value); });
  builder.AddDoubleProperty("Kd", [=] { return GetD(); },
                            [=](double value) { SetD(value); });
  builder.AddDoubleProperty("reference", [=] { return GetReference(); },
                            [=](double value) { SetReference(value); });
}

double PIDController::GetContinuousError(double error) const {
  if (m_continuous && m_inputRange > 0) {
    error = std::fmod(error, m_inputRange);
    if (std::abs(error) > m_inputRange / 2) {
      if (error > 0) {
        return error - m_inputRange;
      } else {
        return error + m_inputRange;
      }
    }
  }

  return error;
}
