#ifndef __INSERT_H__
#define __INSERT_H__

#include "ISerializable.h"
#include "Magic.h"

#include <ostream>
#include <vector>
#include <list>
#include <map>

std::ostream& write_internal(std::ostream &ostream, const char *p, std::size_t size)
{
    ostream.write ( p, size );
    return ostream;
}

/** serialization of POD types */
template<typename T>
std::ostream& insert( std::ostream& ostream, const T& t, typename enable_if<!is_base_of<ISerializable, T>::value >::type* = 0 )
{
    return write_internal( ostream, (const char*)&t, sizeof(t) );
}

/** serialization of ISerializable* */
template<typename T>
std::ostream& insert( std::ostream& ostream, T* serializable )
{
    return serializable->insert( ostream );
}

template<class T>
std::ostream& insert( std::ostream& ostream, const T& t, typename enable_if<is_base_of<ISerializable, T>::value >::type* = 0 )
{
    return t.insert( ostream );
}

/** serialization of std::string */
std::ostream& insert( std::ostream& ostream, const std::string& str )
{
    std::size_t size = str.size();
    insert( ostream, size );
    write_internal( ostream, str.c_str(), size );
    return ostream;
}

/** serialization of STL containers */
template<typename STLPair>
std::ostream& insertPair( std::ostream& ostream, const STLPair& pair )
{
    insert( ostream, pair.first );
    insert( ostream, pair.second );
    return ostream;
}

template<typename K, typename V>
std::ostream& insert( std::ostream& ostream, const std::pair< K, V >& pair )
{
    return insertPair( ostream, pair);
}

template< typename STLContainer>
std::ostream& insertContainer( std::ostream& ostream, const STLContainer& c )
{
    std::size_t size = c.size();
    insert( ostream, size );

    for( typename STLContainer::const_iterator it = c.begin(); it != c.end(); ++it )
    {
        typedef typename STLContainer::value_type value_type;
        value_type value = *it;
        insert( ostream, value );
    }
    return ostream;
}

template<typename T>
std::ostream& insert( std::ostream& ostream, const std::vector< T >& c )
{
    return insertContainer( ostream, c);
}

template<typename T>
std::ostream& insert( std::ostream& ostream, const std::list< T >& c )
{
    return insertContainer( ostream, c);
}

template<typename K, typename V>
std::ostream& insert( std::ostream& ostream, const std::map< K, V >& c )
{
    return insertContainer( ostream, c);
}


#endif // __INSERT_H__
