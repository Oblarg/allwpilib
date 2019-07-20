#pragma once

#include "gmock/gmock.h"
#include "simulation/DriverStationSim.h"
#include <regex>
#include "mockdata/MockHooks.h"

class ErrorConfirmer {
 public:
  ErrorConfirmer(const char* msg) : m_msg(msg) {
  //  HALSIM_SetSendError(&HandleError);
  EXPECT_CALL(*this, ConfirmError());
  }

  ~ErrorConfirmer() {
  //  HALSIM_SetSendError(nullptr);
  }

  MOCK_METHOD0(ConfirmError, void());

  uint32_t HandleError(HAL_Bool isError, int32_t errorCode, HAL_Bool isLVCode, const char* details,
      const char* location, const char* callStack, HAL_Bool printMsg);

 private:
  const char* m_msg;
};