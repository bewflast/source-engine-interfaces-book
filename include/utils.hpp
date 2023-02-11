#ifndef     UTILS_HPP
# define    UTILS_HPP

# include   <list>
# include	<span>
# include	<array>
# include   <string>
# include   <memory>

using   libsList = std::list<std::string>;

# ifdef _WIN32

    # include   <Windows.h>
    # include   <Psapi.h>
    using   SharedLib       =   HMODULE;
    using   ModuleHandle    =   HMODULE;
	# define GET_SYM_ADDR(lib, symName) ( GetProcAddress(lib, symName)  )
	# define GET_LIB_HANDLE(libName)    ( GetModuleHandleA(libName)     )
    # define OFFSET_TO_MOV_BEGINNING    static_cast<unsigned int>(4)

# else

    # include   <link.h>
    using   SharedLib       =   struct dl_phdr_info;
    using   ModuleHandle    =   void* ;
	# define GET_SYM_ADDR(lib, symName) ( dlsym(lib, symName)                       )
	# define GET_LIB_HANDLE(libName)    ( dlopen(libName, RTLD_NOLOAD | RTLD_NOW)   )
    # define OFFSET_TO_MOV_BEGINNING    static_cast<unsigned int>(9)

# endif

uintptr_t                       getCreateInterface_addr (   std::string_view moduleName     );
uintptr_t                       get_s_InterfaceRegs_addr(   uintptr_t CreateInterfaceLoc    );
std::unique_ptr<const libsList> getSharedLibsNames( void );

#endif