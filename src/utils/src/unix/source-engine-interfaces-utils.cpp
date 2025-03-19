#include "utils/source-engine-interfaces-utils.hpp"

#include   	<link.h>
#include <cstddef>
#include   	<string>
#include	<string_view>

using   SharedLib       =   struct dl_phdr_info;
using   ModuleHandle    =   void* ;

namespace {
auto trimSharedLibName(const SharedLib *lib) -> std::string
{
	const std::string_view bufferView{lib->dlpi_name};
	const auto subFrom{bufferView.find_last_of("/\\")};

	if (std::string_view::npos == subFrom){
		return "";
	}

	return std::string{bufferView.substr(subFrom + 1)};
}

auto addLibName(SharedLib *lib, std::size_t /*size*/, void *data) -> int
{
	const auto trimmedSharedLibraryName {trimSharedLibName(lib)};
	if (not trimmedSharedLibraryName.empty()){
		static_cast<sourceEngineInterfaces::utils::DynamicLibsList*>(data)->emplace_back(trimmedSharedLibraryName);
	}

	return 0;
}

auto getModuleHandle(std::string_view moduleName) -> void*
{
	ModuleHandle dynLibHandle{dlopen(moduleName.data(), RTLD_NOLOAD | RTLD_NOW)};

	if (not dynLibHandle) {
		return nullptr;
	}

	return static_cast<void *>(dynLibHandle);
}
} //namespace

namespace sourceEngineInterfaces::utils
{
auto getSymbolFromDynamicLib(std::string_view dynamicLibName, std::string_view symbolName) -> void*
{
	const auto    dynLibHandle { static_cast<ModuleHandle>(getModuleHandle(dynamicLibName)) };

	if (not dynLibHandle) {
		return nullptr;
	}

	return dlsym(dynLibHandle, symbolName.data());
}

auto getLoadedSharedLibsNames() -> DynamicLibsList
{
	DynamicLibsList result {};

	dl_iterate_phdr(addLibName, static_cast<void *>(&result));

	return result;
}
}