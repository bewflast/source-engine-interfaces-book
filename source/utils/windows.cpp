#define WIN32_LEAN_AND_MEAN

#include 	"utils/utils.hpp"

#include	<array>
#include  	<Windows.h>
#include   	<Psapi.h>

using   SharedLib       =   HMODULE;
using   ModuleHandle    =   HMODULE;

namespace {
auto trimSharedLibName(const SharedLib *lib) -> std::string
{
	std::array<char, 1024> buffer{'\0'};
	buffer.fill('\0');
	GetModuleFileNameA(*lib, buffer.data(), buffer.size());

	const std::string_view bufferView{buffer.data()};
	const auto subFrom{bufferView.find_last_of("/\\")};

	if (subFrom == std::string_view::npos){
		return "";
	}

	return std::string{bufferView.substr(subFrom + 1)};
}

void iterateThroughLoadedLibs(libsList& libsNamesList)
{
	DWORD modulesCount{0};
	HANDLE curProcess{GetCurrentProcess()};
	std::array<HMODULE, 512> modules{nullptr};

	modules.fill(nullptr);

	EnumProcessModules(curProcess, modules.data(), sizeof(modules), &modulesCount);

	for (const auto& module : modules) {
		const auto trimmedSharedLibraryName {trimSharedLibName(&module)};
		if (not trimmedSharedLibraryName.empty()){
			libsNamesList.emplace_back(trimmedSharedLibraryName);
		}
	}
}

auto GetModHandle(std::string_view &moduleName) -> void*
{
	ModuleHandle hMod{GetModuleHandleA(moduleName.data())};

	if (not hMod){
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

	return reinterpret_cast<uintptr_t>(GetProcAddress(hMod, symbol.data()));
}
} // namespace

auto getSharedLibsNames() -> libsList
{
	libsList   result{};

	iterateThroughLoadedLibs(result);

	return result;
}

auto get_s_InterfaceRegs_addr(std::string_view moduleName) -> uintptr_t
{
	const auto createInterfaceFnLocation {getSymbol(moduleName, "CreateInterface")};
	if (createInterfaceFnLocation == 0){
		return 0;
	}

	const auto s_pInterfaceRegsSymbol {getSymbol(moduleName, "s_pInterfaceRegs")};
	if (s_pInterfaceRegsSymbol){
		return s_pInterfaceRegsSymbol;
	}

	if (sizeof(void*) == 4)
	{
		const int32_t jmpOffset{*reinterpret_cast<int32_t *>   (createInterfaceFnLocation + 5)};
		const uintptr_t s_pInterfaceRegs{
				*reinterpret_cast<uintptr_t *> (createInterfaceFnLocation + 9 + jmpOffset + 4 + 2)};
		return s_pInterfaceRegs;
	}

	if ( *reinterpret_cast<uint8_t*>(createInterfaceFnLocation) == 0x4C ) {
		return createInterfaceFnLocation + 7 + *reinterpret_cast<int32_t *>(createInterfaceFnLocation + 3); //vphysics
	}

	const uintptr_t       createInterface_mov	{ createInterfaceFnLocation + 15						};
	const int32_t         mov_displ				{ *reinterpret_cast<int32_t*>(createInterface_mov + 3)	};
	const uintptr_t       s_pInterfaceRegs		{ createInterface_mov + 7 + mov_displ					};

	return s_pInterfaceRegs;
}
