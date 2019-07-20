#include "ErrorConfirmer.h"

uint32_t ErrorConfirmer::HandleError(HAL_Bool isError, int32_t errorCode, HAL_Bool isLVCode, const char* details,
  const char* location, const char* callStack, HAL_Bool printMsg) {
  if (std::regex_match(details, std::regex(m_msg))) {
    ConfirmError();
  }
  return 0;
}