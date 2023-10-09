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

auto IFacesBook::hasInterface( const char* interfaceName ) const -> bool
{
	return  _book.contains(interfaceName);
}

auto IFacesBook::operator[]( const char* interfaceName ) const -> void*
{
	if (this->hasInterface(interfaceName)) {
		return _book.at(interfaceName);
	}

	return nullptr;
}