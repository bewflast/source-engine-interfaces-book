#include "utils.hpp"

static void getSharedLibName( const SharedLib* lib, std::span<char>	buffer )
{

# ifdef _WIN32
     GetModuleFileNameA(*lib, buffer.data(), buffer.size());
# else 
    std::string_view libName = lib->dlpi_name;
    std::copy(libName.begin(), libName.end(), buffer.begin());
# endif

};

static std::string trimSharedLibName( const SharedLib* lib )
{

	static std::size_t             	subFrom 	{ 0     };
	static std::string_view    		bufferView 	{ "\0"  };
	static std::array<char, 1024>  	buffer  	{ '\0'  };
	buffer.fill('\0');

    getSharedLibName(lib, buffer);
    bufferView	=	buffer.data();
    subFrom 	= 	bufferView.find_last_of("/\\");

    return ( std::string( std::next(bufferView.begin() + subFrom), bufferView.end()) );

};

#ifdef __linux__
    static int  addLibName( SharedLib* lib, std::size_t size, void* data )
    {
        static_cast<libsList*>(data)->push_back(trimSharedLibName(lib));

        return( 0 );
    }
#endif


static void    iterateThroughLoadedLibs( std::unique_ptr<libsList>& libsNamesList )
{
# ifdef __linux__
    dl_iterate_phdr(addLibName, static_cast<void*>(libsNamesList.get()));
# else

    DWORD   					modulesCount        { 0 };
	HANDLE  					curProcess			{ GetCurrentProcess() };
	std::array<HMODULE, 512>	modules 			{ nullptr };

	modules.fill(nullptr);

    EnumProcessModules(curProcess, modules.data(), sizeof(modules), &modulesCount);

	for (const auto& module : modules)
		libsNamesList->push_back(trimSharedLibName(&module));

# endif 
};

static  void*   GetModHandle( std::string_view& moduleName )
{
    ModuleHandle    hMod {nullptr};

	hMod = GET_LIB_HANDLE(moduleName.data());

    if (!hMod)
        return ( nullptr );
    
    return ( static_cast<void*>(hMod) );
}

static uintptr_t    getExportedCreateInterfaceAddr( ModuleHandle hmod )
{
    uintptr_t       exportedCreateInterfaceAddr { reinterpret_cast<uintptr_t>(GET_SYM_ADDR(hmod, "CreateInterface")) };

    return ( exportedCreateInterfaceAddr );
}




std::unique_ptr<const libsList>   getSharedLibsNames( void )
{

    std::unique_ptr<libsList>   result = std::make_unique<libsList>();
    
    iterateThroughLoadedLibs(result);

    return( result );

};


uintptr_t   getCreateInterface_addr( std::string_view moduleName )
{

    uintptr_t       exportedCreateInterfaceAddr { 0 };
    ModuleHandle    hMod                        { nullptr };

    hMod = static_cast<ModuleHandle>(GetModHandle( moduleName ));
    
    if (!hMod)
        return ( 0 );

    return ( getExportedCreateInterfaceAddr(hMod) );

}


uintptr_t   get_s_InterfaceRegs_addr( uintptr_t CreateInterfaceLoc )
{

#if defined _WIN32 && defined _WIN64

    if ( *reinterpret_cast<uint8_t*>(CreateInterfaceLoc) == 0x4C )
        return (CreateInterfaceLoc + 7 + *reinterpret_cast<int32_t*>(CreateInterfaceLoc + 3)); //vphysics
        
    int32_t         jmpOffset                   { *reinterpret_cast<int32_t*>(CreateInterfaceLoc + 1)           };
    uintptr_t       createInterfaceInternal     { CreateInterfaceLoc + 5 + jmpOffset                            };
    uintptr_t       createInterfaceInternal_mov { createInterfaceInternal + 15                                  };
    int32_t         mov_displ                   { *reinterpret_cast<int32_t*>(createInterfaceInternal_mov + 3)  };
    uintptr_t       s_pInterfaceRegs            { createInterfaceInternal_mov + 7 + mov_displ                   };

# else

    int32_t     jmpOffset                       { *reinterpret_cast<int32_t*>   (CreateInterfaceLoc + 5)                                            }; 
    uintptr_t   s_pInterfaceRegs                { *reinterpret_cast<uintptr_t*> (CreateInterfaceLoc + 9 + jmpOffset + OFFSET_TO_MOV_BEGINNING + 2)  };

#endif

    return ( s_pInterfaceRegs );

}
