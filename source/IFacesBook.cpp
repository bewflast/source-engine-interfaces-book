#include "utils.hpp"
#include "SE_SDK.hpp"
#include "interfaces.hpp"
#include "IFacesBook.hpp"

IFacesBook::IFacesBook( void )
		:
		_book { std::make_unique<IFacesBook::BookType>() }
{
	std::unique_ptr<const libsList>    modules { getSharedLibsNames() };

	if (!modules)
		return ;

	for (std::string_view moduleName : *modules)
	{

		if ( (moduleName.find("steamclient") != std::string_view::npos) || (moduleName.find("vstdlib_s") != std::string_view::npos) )
			continue;

		uintptr_t   createInterfaceLoc      { getCreateInterface_addr(moduleName) };

		if (!createInterfaceLoc)
			continue;

		uintptr_t       s_pInterfaceRegs    { get_s_InterfaceRegs_addr(createInterfaceLoc) };
		InterfaceReg*   interfaceList       { reinterpret_cast<InterfaceReg*>(*reinterpret_cast<uintptr_t*>(s_pInterfaceRegs)) };

		for (; interfaceList; interfaceList = interfaceList->m_pNext)
		{
			for (const auto& elem : interfacesMap)
			{
				if (!elem.second.contains(interfaceList->m_pName))
					continue;
				this->_book->emplace(	(!this->hasInterface(elem.first.c_str())		\
											? elem.first.c_str()						\
											: interfaceList->m_pName),
										interfaceList->m_CreateFn());
			}
		}

	}

};


bool    IFacesBook::hasInterface( const char* interfaceName ) const
{
	return ( this->_book->contains(interfaceName) );
}

void*   IFacesBook::operator[]( const char* interfaceName ) const
{
	if (this->hasInterface(interfaceName))
		return ( (*this->_book)[interfaceName] );

	return ( nullptr );
}