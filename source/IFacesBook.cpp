#include "interfaces.hpp"
#include "source_engine_sdk.hpp"
#include "source-engine-interfaces-book/IFacesBook.hpp"

#include <utils/utils.hpp>

IFacesBook::IFacesBook()
{
	const auto    modules { getSharedLibsNames() };

	if (modules.empty()) {
		return;
	}

	for (const std::string_view moduleName : modules)
	{

		if ((moduleName.find("steam") != std::string_view::npos) || (moduleName.find("vstdlib_s") != std::string_view::npos)) {
			continue;
		}

		const auto       s_pInterfaceRegs    { get_s_InterfaceRegs_addr(moduleName) };
		if (!s_pInterfaceRegs){
			continue;
		}

		auto*   interfaceList       { reinterpret_cast<InterfaceReg*>(*reinterpret_cast<uintptr_t*>(s_pInterfaceRegs)) };
		for (; interfaceList; interfaceList = interfaceList->m_pNext)
		{
			for (const auto& elem : interfacesMap)
			{
				if (!elem.second.contains(interfaceList->m_pName)) {
					continue;
				}
				_book.emplace(	(!hasInterface(elem.first.c_str())		\
											? elem.first.c_str()		\
											: interfaceList->m_pName),
										interfaceList->m_CreateFn());
			}
		}

	}
};

auto IFacesBook::hasInterface(std::string_view interfaceName) const -> bool
{
	return  _book.contains(interfaceName.data());
}

auto IFacesBook::getInterface(std::string_view interfaceName) const -> void*
{
	std::string interfaceType {interfaceName};

	std::transform(interfaceType.begin(), interfaceType.end(), interfaceType.begin(),
				   [](unsigned char letter) -> int
				   {
					   return ( std::tolower(letter) );
				   });

	return reinterpret_cast<void*>(_book.at(interfaceType));
};