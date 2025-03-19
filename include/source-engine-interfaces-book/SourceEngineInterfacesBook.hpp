#include <string_view>

namespace sourceEngineInterfaces::book
{
    auto getInterface(std::string_view interfaceType, std::string_view interfaceVersion = "") -> void*;
}