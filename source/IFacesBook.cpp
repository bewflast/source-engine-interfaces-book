#include "interfaces.hpp"
#include "source_engine_sdk.hpp"
#include "source-engine-interfaces-book/IFacesBook.hpp"

#include <utils/utils.hpp>

#include <memory>
namespace
{
auto buildInterfacesMap() -> std::unordered_map<std::string, void*>
{
	const auto    modules { getSharedLibsNames() };
	if (modules.empty()) {
		return {};
	}

	auto book {std::unordered_map<std::string, void*>{}};

	for (const std::string_view moduleName : modules)
	{

		if ((moduleName.find("steam") != std::string_view::npos) or (moduleName.find("vstdlib_s") != std::string_view::npos)) {
			continue;
		}

		const auto       s_pInterfaceRegs    { get_s_InterfaceRegs_addr(moduleName) };
		if (not s_pInterfaceRegs) {
			continue;
		}

		auto*   interfaceList       { reinterpret_cast<InterfaceReg*>(*reinterpret_cast<uintptr_t*>(s_pInterfaceRegs)) };
		for (; nullptr != interfaceList; interfaceList = interfaceList->m_pNext)
		{
			for (const auto& [interfaceType, interfaceVersions] : interfacesMap)
			{
				if (not interfaceVersions.contains(interfaceList->m_pName)) {
					continue;
				}
				book.emplace(	(not book.contains(interfaceType)		\
											? interfaceType				\
											: interfaceList->m_pName),
								  interfaceList->m_CreateFn());
			}
		}

	}
	return book;
}
}  // namespace

IFacesBook::IFacesBook() : _book{buildInterfacesMap()}
{};

auto IFacesBook::findInterface(std::string_view interfaceName) const -> void*
{
	std::string interfaceType {interfaceName};

	std::transform(interfaceType.begin(), interfaceType.end(), interfaceType.begin(),
				   [](unsigned char letter) -> int
				   {
					   return ( std::tolower(letter) );
				   });

	return _book.contains(interfaceType) ? _book.at(interfaceType) : nullptr;
};

auto IFacesBook::getInterface(std::string_view interfaceName) -> void*
{
	static const std::unique_ptr<IFacesBook> interfacesBookGlobalInstance {new IFacesBook};

	return interfacesBookGlobalInstance->findInterface(interfaceName);
}