#ifndef     IFACESBOOK_HPP
# define    IFACESBOOK_HPP

# include   <string>
# include   <memory>
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
    bool    hasInterface( const char* interfaceName ) const;

public:
    IFacesBook();

    void*   operator[]( const char* interfaceName ) const;

    template<typename T>
	T* getInterface( const char*	interfaceName ) const;

};

template<typename T>
T* IFacesBook::getInterface( const char*	interfaceName ) const
{
    return ( reinterpret_cast<T*>((*this)[interfaceName]) );
};

#endif