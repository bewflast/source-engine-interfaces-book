#ifndef     IFACESBOOK_HPP
# define    IFACESBOOK_HPP

# include   <string>
# include	<algorithm>
# include	<string_view>
# include   <unordered_map>

class IFacesBook final
{

public:
	IFacesBook(IFacesBook&& other)     noexcept    					= delete;
	IFacesBook(const IFacesBook& other)                   			= delete;
	auto operator=(IFacesBook&& other) noexcept 	-> IFacesBook&	= delete;
	auto operator=(const IFacesBook& other) 		-> IFacesBook&	= delete;

	~IFacesBook() = default;
	[[nodiscard]] static auto getInterface(std::string_view interfaceName) -> void*;

private:
	IFacesBook();
	[[nodiscard]] auto findInterface(std::string_view interfaceName) const -> void*;

	const std::unordered_map<std::string, void*>   _book;

};

#endif