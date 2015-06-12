#ifndef __MAGIC_H__
#define __MAGIC_H__

namespace Meta{
    typedef char (&yes)[1];
    typedef char (&no)[2];

    template <typename B, typename D>
    struct Host
    {
        operator B*() const;
        operator D*();
    };

    template <typename B, typename D>
    struct is_base_of
    {
        template <typename T>
        static yes check(D*, T);
        static no check(B*, int);

        static const bool value = sizeof(check(Host<B,D>(), int())) == sizeof(yes);
    };

    template<bool B, class T = void>
    struct enable_if {};

    template<class T>
    struct enable_if<true, T> { typedef T type; };
}


#endif //  __MAGIC_H__
