#ifndef __TEST_H__
#define __TEST_H__

#include "Insert.hpp"
#include "Extract.hpp"

#include <sstream>

#define VERIFY(cond) \
do \
{ \
    if (!(cond)) \
    { \
        std::cout << "Check failed: (" << #cond << ") in " << __FUNCTION__ <<  ":" << __LINE__ << std::endl; \
    } \
} while(0)

class Object : public ISerializable
{
public:
    Object(int aI = 0, std::string anString = "")
        : i(aI)
        , s(anString)
    {
    }

    int i;
    std::string s;

    std::ostream& insert( std::ostream& os ) const
    {
        ::insert( os, i );
        ::insert( os, s );
        return os;
    }

    std::istream& extract( std::istream& is )
    {
        ::extract( is, i );
        ::extract( is, s );
        return is;
    }
};


class SerializationTester
{
public:
    static void testPODTypes()
    {
        std::string serialized;

        {
            // serialization
            std::ostringstream os;

            insert<bool>( os, true );
            insert<char>( os, 'x' );
            insert<int>( os, 1 );
            insert<long>( os, 9999 );
            insert<float>( os, 8888.f );
            insert<double>( os, 9999. );

            serialized = os.str();
        }

        {
            // deserialization
            std::istringstream is(serialized);

            bool b = false;
            char c = '\0';
            int i = 0;
            long l = 0;
            float f = 0.;
            double d = 0.;

            extract<bool>( is, b );
            extract<char>( is, c );
            extract<int>( is, i );
            extract<long>( is, l );
            extract<float>( is, f );
            extract<double>( is, d );

            VERIFY( b == true );
            VERIFY( c == 'x' );
            VERIFY( i == 1 );
            VERIFY( l == 9999 );
            VERIFY( f == 8888.f );
            VERIFY( d == 9999. );
        }
    }

    static void testSTDString()
    {
        std::string serialized;

        {
            // serialization
            std::ostringstream os;

            std::string in = "qwertyuiop";
            insert( os, in );

            serialized = os.str();
        }

        {
            // deserialization
            std::istringstream is(serialized);

            std::string out;
            extract( is, out );

            VERIFY( out == "qwertyuiop" );
        }
    }

    static void testPODContainerVector()
    {
        std::string serialized;

        {
            // serialization
            std::ostringstream os;

            std::vector<int> out;
            out.push_back( 1 );
            out.push_back( 3 );
            out.push_back( 5 );


            insert( os, out );

            serialized = os.str();
        }

        {
            // deserialization
            std::istringstream is(serialized);

            std::vector<int> in;
            extract( is, in );

            VERIFY( in.size() == 3 );
            VERIFY( in[0] == 1 );
            VERIFY( in[1] == 3 );
            VERIFY( in[2] == 5 );
        }
    }

    static void testStringContainerVector()
    {
        std::string serialized;

        {
            // serialization
            std::ostringstream os;

            std::vector<std::string> out;
            out.push_back( "one" );
            out.push_back( "three" );

            insert( os, out );

            serialized = os.str();
        }

        {
            // deserialization
            std::istringstream is(serialized);

            std::vector<std::string> in;
            extract( is, in );

            VERIFY( in.size() == 2 );
            VERIFY( in[0] == "one" );
            VERIFY( in[1] == "three" );
        }
    }

    static void testPODContainerMap()
    {
        std::string serialized;

        {
            std::ostringstream os;

            std::map<long, std::string> out;
            out[1] = "one";
            out[2] = "two";
            insert( os, out );

            serialized = os.str();
        }

        {
            // deserialization
            std::istringstream is(serialized);

            std::map<long, std::string> in;
            extract( is, in );

            VERIFY( in.size() == 2 );
            VERIFY( in[1] == "one" );
            VERIFY( in[2] == "two" );
        }
    }

    static void testNonPODVector()
    {
        std::string serialized;

        {
            // serialization
            std::ostringstream os;

            std::vector<Object*> out;
            Object* one = new Object(1, "one");
            Object* two = new Object(2, "two");
            out.push_back( one );
            out.push_back( two );
            insert( os, out );

            delete one;
            delete two;

            serialized = os.str();

        }

        {
            // deserialization
            std::istringstream is(serialized);

            std::vector<Object*> in;
            extract( is, in );

            VERIFY( in.size() == 2 );
            VERIFY( in[0]->i == 1 );
            VERIFY( in[1]->s == "two" );
        }
    }
    
    static void testSerializableType()
    {
        std::string serialized;
        
        {
            // serialization
            std::ostringstream os;
            
            Object obj(1, "one");
            insert(os, obj);
            
            serialized = os.str();
        }
        
        {
            // deserialization
            std::istringstream is(serialized);
            
            Object obj;
            extract( is, obj );
            
            VERIFY(obj.i == 1);
            VERIFY(obj.s == "one");
        }
    }
    
    

    static void runAllTests()
    {
        testPODTypes();
        testSTDString();
        testPODContainerVector();
        testStringContainerVector();
        testPODContainerMap();
        testNonPODVector();
        testSerializableType();
    }
};

int main()
{
    SerializationTester::runAllTests();
    return 0;
}


#endif // __TEST_H__
