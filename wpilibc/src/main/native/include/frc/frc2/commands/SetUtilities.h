#pragma once

#include <wpi/SmallVector.h>
#include <wpi/ArrayRef.h>

namespace frc {
namespace frc2 {
template<typename T>
void SetInsert(wpi::SmallVectorImpl<T*>& vector, wpi::ArrayRef<T*> toAdd) {
  for (auto addCommand : toAdd) {
    bool exists = false;
    for (auto existingCommand : vector) {
      if (addCommand == existingCommand) {
        exists = true;
        break;
      }
    }
    if (!exists) {
      vector.emplace_back(addCommand);
    }
  }
}
}
}
