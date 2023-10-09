#ifndef     UTILS_HPP
# define    UTILS_HPP

# include   <list>
# include 	<string_view>

using   libsList = std::list<std::string>;

auto get_s_InterfaceRegs_addr(std::string_view moduleName) -> uintptr_t;
auto getSharedLibsNames() -> libsList;

#endif