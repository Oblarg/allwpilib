#pragma once

#include "gmock/gmock.h"
#include "simulation/DriverStationSim.h"
#include <regex>
#include "mockdata/MockHooks.h"

class ErrorConfirmer {
 public:
  ErrorConfirmer(const char* msg) : m_msg(msg) {
    if (instance != nullptr) return;
    HALSIM_SetSendError(HandleError);
    EXPECT_CALL(*this, ConfirmError());
    instance = this;
  }

  ~ErrorConfirmer() {
   HALSIM_SetSendError(nullptr);
   instance = nullptr;
  }

  MOCK_METHOD0(ConfirmError, void());

  const char* m_msg;

  static int32_t HandleError(HAL_Bool isError, int32_t errorCode, HAL_Bool isLVCode, const char* details,
      const char* location, const char* callStack, HAL_Bool printMsg);

 private:
  static ErrorConfirmer* instance;
};

