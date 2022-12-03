#include "utils.hpp"

static void getSharedLibName(SharedLib* lib, char* buff, std::size_t size)
{
# ifdef _WIN32
     GetModuleFileNameA(*lib, buff, size);
# else 
    const char* lname = lib->dlpi_name;

    for(std::size_t i = 0; lname[i] && (i < size); ++i)
        buff[i] = lname[i];
# endif
};

static std::string trimSharedLibName(SharedLib* lib)
{
    char        nameBuff[1024]  =   { 0 };
    std::size_t subFrom         =   0;
    std::string libName         =   "";

    getSharedLibName(lib, nameBuff, 1024);
    
    libName = nameBuff;
    subFrom = libName.find_last_of("/\\");

    return ( libName.substr(subFrom + 1) );

};

#ifdef __linux__
    static int  addLibName(SharedLib* lib, std::size_t size, void* data)
# else
    static int  addLibName(SharedLib* lib, std::list<std::string>* data)
# endif    
{
    static_cast<std::list<std::string>*>(data)->push_back(trimSharedLibName(lib));

    return (0);
};


static void    iterateThroughLoadedLibs( std::list<std::string>* libsNamesList )
{
# ifdef __linux__
    dl_iterate_phdr(addLibName, static_cast<void*>(libsNamesList));
# else
    
    HANDLE  curProcess          = GetCurrentProcess();  
    HMODULE modules[1024]       = { NULL };
    DWORD   modulesCount        = 0;

    EnumProcessModules(curProcess, modules, sizeof(modules), &modulesCount);
    for ( std::size_t i = 0; i < (modulesCount / sizeof(HMODULE)); i++)
        addLibName(&modules[i], libsNamesList);

# endif 
};

static  void*   GetModHandle( const std::string& moduleName)
{
    ModuleHandle    hMod(nullptr);

# ifdef _WIN32
    hMod    = GetModuleHandleA(moduleName.c_str());
# else
    hMod    = dlopen(moduleName.c_str(), RTLD_NOLOAD | RTLD_NOW);
# endif

    if (!hMod)
        return (nullptr);
    
    return ( static_cast<void*>(hMod) );
}

static uintptr_t    getExportedCreateInterfaceAddr(ModuleHandle hmod)
{
    uintptr_t       exportedCreateInterfaceAddr(0);

# ifdef _WIN32
    exportedCreateInterfaceAddr    = reinterpret_cast<uintptr_t>(GetProcAddress(hmod, "CreateInterface"));
# else
    exportedCreateInterfaceAddr    = reinterpret_cast<uintptr_t>(dlsym(hmod, "CreateInterface"));
# endif

    return ( exportedCreateInterfaceAddr );
}








const std::list<std::string>*   getSharedLibsNames()
{
    std::list<std::string>*   result = new(std::nothrow) std::list<std::string>;
    
    if (!result)
        return ( nullptr );
    
    iterateThroughLoadedLibs(result);

    return( result );
};


uintptr_t   getCreateInterface_addr( const std::string& moduleName )
{
    ModuleHandle    hMod(nullptr);
    uintptr_t       exportedCreateInterfaceAddr(0);

    hMod = static_cast<ModuleHandle>(GetModHandle( moduleName ));
    
    if (!hMod)
        return ( 0 );

    return ( getExportedCreateInterfaceAddr(hMod) );
}


uintptr_t   get_s_InterfaceRegs_addr( uintptr_t CreateInterfaceLoc )
{

# if defined _WIN32 && defined _WIN64

    if ( *reinterpret_cast<uint8_t*>(CreateInterfaceLoc) == 0x4C )
        return (CreateInterfaceLoc + 7 + *reinterpret_cast<int32_t*>(CreateInterfaceLoc + 3)); //vphysics
        
    int32_t         jmpOffset                   =  *reinterpret_cast<int32_t*>(CreateInterfaceLoc + 1);
    uintptr_t       createInterfaceInternal     =   CreateInterfaceLoc + 5 + jmpOffset;
    uintptr_t       createInterfaceInternal_mov =   createInterfaceInternal + 15;
    int32_t         mov_displ                   =   *reinterpret_cast<int32_t*>(createInterfaceInternal_mov + 3);
    uintptr_t       s_pInterfaceRegs            =   createInterfaceInternal_mov + 7 + mov_displ;

# else

    int32_t     jmpOffset           = *reinterpret_cast<int32_t*>(CreateInterfaceLoc + 5); 
    uintptr_t   s_pInterfaceRegs    = *reinterpret_cast<int32_t*>(CreateInterfaceLoc + 9 + jmpOffset + OFFSET_TO_MOV_BEGINNING + 2);

# endif

    return ( s_pInterfaceRegs );

}
