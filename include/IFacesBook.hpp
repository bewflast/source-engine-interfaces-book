#ifndef     IFACESBOOK_HPP
# define    IFACESBOOK_HPP

# include   <string>
# include   <memory>
# include	<algorithm>
# include   <unordered_map>

class IFacesBook final
{

    IFacesBook( IFacesBook&& other )            noexcept    = delete;
    IFacesBook( const IFacesBook& other )                   = delete;
    IFacesBook& operator=( IFacesBook&& other ) noexcept    = delete;
    IFacesBook& operator=( const IFacesBook& other )        = delete;

    using BookType = std::unordered_map<std::string, void*>;

private:
    std::unique_ptr<BookType>   _book;

public:
    IFacesBook();

    void*   operator[]( const char* interfaceName ) 	const;
	bool    hasInterface( const char* interfaceName )	const;

    template<typename T>
	T* getInterface() const;

};

template<typename T>
T* IFacesBook::getInterface() const
{
	std::string 	interfaceTypeIDFull { typeid(T).name() };
	std::string 	interfaceType		(interfaceTypeIDFull.begin() + 6, interfaceTypeIDFull.end());

	std::transform(interfaceType.begin(), interfaceType.end(), interfaceType.begin(),
				   [](unsigned char c) -> int
				   {
						return ( std::tolower(c) );
				   });

    return ( reinterpret_cast<T*>((*this)[interfaceType.c_str()]) );
};

#endif