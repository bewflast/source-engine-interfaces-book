#include <algorithm>
#include <functional>
#include <utils/source-engine-interfaces-utils.hpp>

#include "interfaces.hpp"
#include "source-engine-interfaces-book/SourceEngineInterfacesBook.hpp"

namespace
{
using InterfaceCreator = std::function<void*(const char*, int*)>;
using CreateInterfaceSig = void*(*)(const char*, int*);

auto buildCreateInterfaceFunctionsList() -> std::list<InterfaceCreator> 
{
    auto functionsList{std::list<InterfaceCreator>{}};

    const auto loadedLibrariesList {sourceEngineInterfaces::utils::getLoadedSharedLibsNames()};

    for (const auto& libraryName : loadedLibrariesList) {
        const auto* createInterfaceFnAddress {
            sourceEngineInterfaces::utils::getSymbolFromDynamicLib(libraryName, sourceEngineInterfaces::utils::CREATE_INTERFACE_SYMBOL)
        };
        
        if (not createInterfaceFnAddress) {
            continue;
        }
        
        auto* createInterfaceFnPtr {reinterpret_cast<CreateInterfaceSig>(createInterfaceFnAddress)};

        functionsList.emplace_back(createInterfaceFnPtr);
    }

    return functionsList;
}
}

namespace sourceEngineInterfaces::book
{
auto getInterface(std::string_view interfaceType, std::string_view interfaceVersion) -> void*
{
    static const auto createInterfaceFunctionsList {buildCreateInterfaceFunctionsList()};
    
    for(const auto& createInterfaceFunction : createInterfaceFunctionsList) {
        auto interfaceTypeStr {std::string{interfaceType}};

        std::ranges::transform(interfaceTypeStr, interfaceTypeStr.begin(), [](unsigned char letter) -> int {
            return std::tolower(letter);
        });

        auto* foundInterface {static_cast<void*>(nullptr)};
        const auto& interfaceVersions {interfacesMap.at(interfaceTypeStr)};

        foundInterface = std::find(interfaceVersions.begin(), interfaceVersions.end(), interfaceVersion) != interfaceVersions.end() 
            ? createInterfaceFunction(interfaceVersion.data(), nullptr) 
            : nullptr;

        if (foundInterface) {
            return foundInterface;
        }
        
        if (!interfaceVersion.empty()) {
            continue;
        }

        for (const auto& version : interfaceVersions) {
            foundInterface = createInterfaceFunction(version.data(), nullptr);
            if (foundInterface) {
                return foundInterface;
            }
        }
    }

    return nullptr;
}
}