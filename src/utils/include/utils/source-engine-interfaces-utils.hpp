#pragma once

#include <list>
#include <string>
#include <string_view>

namespace sourceEngineInterfaces::utils
{
using DynamicLibsList = std::list<std::string>;

static inline constexpr const auto* CREATE_INTERFACE_SYMBOL {"CreateInterface"};

auto getLoadedSharedLibsNames() -> DynamicLibsList;
auto getSymbolFromDynamicLib(std::string_view dynamicLibName, std::string_view symbolName) -> void*;
}