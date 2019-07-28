#pragma once

#include "Command.h"
#include <type_traits>

namespace frc2 {

/**
 * CRTP implementation to allow polymorphic decorator functions in Command.
 * 
 * <p>Note: ALWAYS create a subclass by extending CommandHelper<Base, Subclass>, or decorators
 * will not function!
 */
template<typename Base, typename CRTP, typename = std::enable_if_t<std::is_base_of_v<Command, Base>>>
class CommandHelper: public Base {
    using Base::Base;

protected:
    std::unique_ptr<Command> TransferOwnership()&& override {
        return std::make_unique<CRTP>(std::move(*static_cast<CRTP*>(this)));
    }
};
}