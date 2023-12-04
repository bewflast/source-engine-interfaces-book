#include "utils/utils.hpp"

#include   	<link.h>
#include   	<string>

using   SharedLib       =   struct dl_phdr_info;
using   ModuleHandle    =   void* ;

namespace {
auto trimSharedLibName(const SharedLib *lib) -> std::string
{
	const std::string_view bufferView{lib->dlpi_name};
	const auto subFrom{bufferView.find_last_of("/\\")};

	if (subFrom == std::string_view::npos){
		return "";
	}

	return std::string{bufferView.substr(subFrom + 1)};
}

auto addLibName(SharedLib *lib, std::size_t /*size*/, void *data) -> int
{
	const auto trimmedSharedLibraryName {trimSharedLibName(lib)};
	if (not trimmedSharedLibraryName.empty()){
		static_cast<libsList *>(data)->emplace_back(trimmedSharedLibraryName);
	}

	return 0;
}

auto GetModHandle(std::string_view &moduleName) -> void*
{
	ModuleHandle hMod{dlopen(moduleName.data(), RTLD_NOLOAD | RTLD_NOW)};

	if (not hMod) {
		return nullptr;
	}

	return static_cast<void *>(hMod);
}

auto getSymbol(std::string_view moduleName, std::string_view symbol) -> uintptr_t
{
	const auto    hMod { static_cast<ModuleHandle>(GetModHandle( moduleName )) };

	if (not hMod) {
		return 0;
	}

	return reinterpret_cast<uintptr_t>(dlsym(hMod, symbol.data()));
}
} //namespace


auto getSharedLibsNames() -> libsList
{
	libsList result {};

	dl_iterate_phdr(addLibName, static_cast<void *>(&result));

	return result;
}

auto get_s_InterfaceRegs_addr( std::string_view moduleName ) -> uintptr_t
{
	const auto createInterfaceFnLocation {getSymbol(moduleName, "CreateInterface")};
	if (createInterfaceFnLocation == 0){
		return 0;
	}

	const auto s_pInterfaceRegsSymbol {getSymbol(moduleName, "s_pInterfaceRegs")};
	if (s_pInterfaceRegsSymbol){
		return s_pInterfaceRegsSymbol;
	}

	const int32_t     jmpOffset	{ *reinterpret_cast<int32_t*>   (createInterfaceFnLocation + 5) };
    const uintptr_t   s_pInterfaceRegs {
		*reinterpret_cast<uintptr_t*> (createInterfaceFnLocation + 9 + jmpOffset + static_cast<unsigned int>(9) + 2)};

	return s_pInterfaceRegs;
}
