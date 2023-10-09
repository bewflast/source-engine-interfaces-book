#ifndef     IFACESBOOK_HPP
# define    IFACESBOOK_HPP

# include   <string>
# include	<algorithm>
# include 	<typeinfo>
# include   <unordered_map>

class IFacesBook final
{
public:
	IFacesBook(IFacesBook&& other)     noexcept    					= delete;
	IFacesBook(const IFacesBook& other)                   			= delete;
	auto operator=(IFacesBook&& other) noexcept 	-> IFacesBook&	= delete;
	auto operator=(const IFacesBook& other) 		-> IFacesBook&	= delete;

    IFacesBook();
	~IFacesBook() = default;

    auto operator[](const char* interfaceName) 	const -> void*;
	auto hasInterface(const char* interfaceName)	const -> bool;

    template<typename T>
	auto getInterface() const -> T*;

private:
	using BookType = std::unordered_map<std::string, void*>;
	BookType   _book{};

};

template<typename T>
auto IFacesBook::getInterface() const -> T*
{
	const std::string 	interfaceTypeIDFull { typeid(T).name() };
	constexpr std::string_view stringToSub {"class "};
	std::string interfaceType {interfaceTypeIDFull.substr(stringToSub.length())};

	std::transform(interfaceType.begin(), interfaceType.end(), interfaceType.begin(),
				   [](unsigned char letter) -> int
				   {
						return ( std::tolower(letter) );
				   });

    return reinterpret_cast<T*>(_book.at(interfaceType));
};

#endif