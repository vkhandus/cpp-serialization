#ifndef __EXTRACT_H__
#define __EXTRACT_H__

#include "ISerializable.h"
#include "Magic.h"

#include <istream>
#include <vector>
#include <list>
#include <map>

std::istream& read_internal( std::istream& istream, char* p, std::size_t size )
{
    return istream.read( p, size );
}

/** deserialization of POD types */
template<typename T>
std::istream& extract( std::istream& istream, T& t, typename enable_if< !is_base_of<ISerializable, T>::value >::type* = 0 )
{
    return read_internal( istream, (char*)&t, sizeof(t) );
}

/** deserialization of ISerializable */
template<typename T>
std::istream& extract( std::istream& istream, T*& t )
{
    t = new T;
    return t->extract( istream );
}

template<class T>
std::istream& extract( std::istream& istream, T& t, typename enable_if< is_base_of<ISerializable, T>::value >::type* = 0 )
{
    return t.extract( istream );
}

/** deserialization of std::string */
std::istream& extract( std::istream& istream, std::string& str )
{
    std::size_t size = 0;
    extract( istream, size );
    
    std::vector<char> strContent( size, '\0');
    read_internal( istream, &strContent[0], size );
    
    str = std::string( strContent.begin(), strContent.end() );
    return istream;
}

/** deserialization of STL containers */
template<typename STLPair>
std::istream& extractPair( std::istream& istream, STLPair& p )
{
    extract( istream, p.first );
    extract( istream, p.second );
    return istream;
}

template<typename K, typename V>
std::istream& extract( std::istream& istream, std::pair< K, V >& pair )
{
    return extractPair( istream, pair );
}

template< typename STLContainer>
std::istream& extractContainer( std::istream& istream, STLContainer& c )
{
    std::size_t size = 0;
    extract( istream, size );

    std::vector<typename STLContainer::value_type > tempVector;
    for ( std::size_t i = 0; i < size; ++i )
    {
        typename STLContainer::value_type element;
        extract ( istream, element );
        tempVector.push_back( element );
    }

    c = STLContainer( tempVector.begin(), tempVector.end() );

    return istream;
}

template<typename T>
std::istream& extract( std::istream& istream, std::vector< T >& c )
{
    return extractContainer( istream, c);
}

template<typename T>
std::istream& extract( std::istream& istream, std::list< T >& c )
{
    return extractContainer( istream, c);
}

template<typename K, typename V>
std::istream& extract( std::istream& istream, std::map< K, V >& c )
{
    std::size_t size = 0;
    extract( istream, size );

    for ( std::size_t i = 0; i < size; ++i )
    {
        std::pair<K, V> element;
        extract( istream, element );
        c.insert( element );
    }

    return istream;
}

#endif // __EXTRACT_H__
