#ifndef __I_SERIALIZABLE_H__
#define __I_SERIALIZABLE_H__

#include <iostream>

class ISerializable
{
public:
    virtual std::ostream& insert( std::ostream& ) const = 0;
    virtual std::istream& extract( std::istream& ) = 0;
};

#endif // __I_SERIALIZABLE_H__
