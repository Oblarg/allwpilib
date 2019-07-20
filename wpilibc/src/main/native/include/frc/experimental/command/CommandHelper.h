#pragma once

#include "Command.h"
#include <type_traits>

namespace frc {
namespace experimental {

template<typename Base, typename CRTP, typename = std::enable_if_t<std::is_base_of_v<Command, Base>>>
class CommandHelper: public Base {
    using Base::Base;

protected:
    std::unique_ptr<Command> TransferOwnership()&& override {
        return std::make_unique<CRTP>(std::move(*static_cast<CRTP*>(this)));
    }
};
}
}