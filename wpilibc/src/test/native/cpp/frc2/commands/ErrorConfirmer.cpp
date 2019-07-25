#include "ErrorConfirmer.h"

ErrorConfirmer* ErrorConfirmer::instance;

int32_t ErrorConfirmer::HandleError(HAL_Bool isError, int32_t errorCode, HAL_Bool isLVCode, const char* details,
  const char* location, const char* callStack, HAL_Bool printMsg) {
  if (std::regex_search(details, std::regex(instance->m_msg))) {
    instance->ConfirmError();
  }
  return 1;
}