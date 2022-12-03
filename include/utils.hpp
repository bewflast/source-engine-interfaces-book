#ifndef     UTILS_HPP
# define    UTILS_HPP

# include   <string>
# include   <list>
# include   <new>

# ifdef _WIN32
    # include   <Windows.h>
    # include   <Psapi.h>
    using   SharedLib       =   HMODULE;
    using   ModuleHandle    =   HMODULE;
    # define OFFSET_TO_MOV_BEGINNING  static_cast<unsigned int>(4)
# else 
    # include   <link.h>
    using   SharedLib       =   struct dl_phdr_info;
    using   ModuleHandle    =   void* ;
    # define OFFSET_TO_MOV_BEGINNING  static_cast<unsigned int>(9)
# endif

const std::list<std::string>*   getSharedLibsNames();
uintptr_t                       getCreateInterface_addr( const std::string& moduleName );
uintptr_t                       get_s_InterfaceRegs_addr( uintptr_t CreateInterfaceLoc );

#endif