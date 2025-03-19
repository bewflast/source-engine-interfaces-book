#include <memory>
#define WIN32_LEAN_AND_MEAN

#include 	"utils/source-engine-interfaces-utils.hpp"

#include	<array>
#include 	<string>

#include  	<Windows.h>
#include   	<Psapi.h>

using   SharedLib       =   HMODULE;
using   ModuleHandle    =   HMODULE;

static inline constexpr const auto SHARED_LIB_NAME_BUFFER_SIZE {1024U};
static inline constexpr const auto SHARED_LIBS_COUNT_BUFFER_SIZE {512U};

namespace {
auto trimSharedLibName(const SharedLib *lib) -> std::string
{
	std::array<char, SHARED_LIB_NAME_BUFFER_SIZE> buffer{'\0'};
	buffer.fill('\0');
	GetModuleFileNameA(*lib, buffer.data(), buffer.size());

	const std::string_view bufferView{buffer.data()};
	const auto subFrom{bufferView.find_last_of("/\\")};

	if (subFrom == std::string_view::npos){
		return "";
	}

	return std::string{bufferView.substr(subFrom + 1)};
}

auto iterateThroughLoadedLibs(sourceEngineInterfaces::utils::DynamicLibsList& libsNamesList) -> void
{
	DWORD modulesCount{0};
	HANDLE curProcess{GetCurrentProcess()};
	std::array<HMODULE, SHARED_LIBS_COUNT_BUFFER_SIZE> modules{nullptr};

	modules.fill(nullptr);

	EnumProcessModules(curProcess, modules.data(), sizeof(modules), &modulesCount);

	for (const auto& module : modules) {
		const auto trimmedSharedLibraryName {trimSharedLibName(&module)};
		if (not trimmedSharedLibraryName.empty()){
			libsNamesList.emplace_back(trimmedSharedLibraryName);
		}
	}
}

auto GetModHandle(std::string_view moduleName) -> ModuleHandle
{
	ModuleHandle moduleHandle{GetModuleHandleA(moduleName.data())};

	if (not moduleHandle){
		return nullptr;
	}

	return static_cast<ModuleHandle>(moduleHandle);
}
} // namespace

namespace sourceEngineInterfaces::utils
{
auto getSymbolFromDynamicLib(std::string_view dynamicLibName, std::string_view symbolName) -> void*
{
    const auto dynLibHandle {static_cast<ModuleHandle>(GetModHandle(dynamicLibName))};

	if (not dynLibHandle) {
		return nullptr;
	}
	
	auto* procedureAdress {static_cast<void*>(GetProcAddress(dynLibHandle, symbolName.data()))};
	return procedureAdress;
}

auto getLoadedSharedLibsNames() -> DynamicLibsList
{
	DynamicLibsList   result{};

	iterateThroughLoadedLibs(result);

	return result;
}
}