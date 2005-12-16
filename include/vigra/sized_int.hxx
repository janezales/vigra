/************************************************************************/
/*                                                                      */
/*               Copyright 1998-2002 by Ullrich Koethe                  */
/*       Cognitive Systems Group, University of Hamburg, Germany        */
/*                                                                      */
/*    This file is part of the VIGRA computer vision library.           */
/*    The VIGRA Website is                                              */
/*        http://kogs-www.informatik.uni-hamburg.de/~koethe/vigra/      */
/*    Please direct questions, bug reports, and contributions to        */
/*        koethe@informatik.uni-hamburg.de          or                  */
/*        vigra@kogs1.informatik.uni-hamburg.de                         */
/*                                                                      */
/*    Permission is hereby granted, free of charge, to any person       */
/*    obtaining a copy of this software and associated documentation    */
/*    files (the "Software"), to deal in the Software without           */
/*    restriction, including without limitation the rights to use,      */
/*    copy, modify, merge, publish, distribute, sublicense, and/or      */
/*    sell copies of the Software, and to permit persons to whom the    */
/*    Software is furnished to do so, subject to the following          */
/*    conditions:                                                       */
/*                                                                      */
/*    The above copyright notice and this permission notice shall be    */
/*    included in all copies or substantial portions of the             */
/*    Software.                                                         */
/*                                                                      */
/*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND    */
/*    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES   */
/*    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND          */
/*    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT       */
/*    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,      */
/*    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      */
/*    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR     */
/*    OTHER DEALINGS IN THE SOFTWARE.                                   */                
/*                                                                      */
/************************************************************************/


#ifndef VIGRA_SIZED_INT_HXX
#define VIGRA_SIZED_INT_HXX

#include "metaprogramming.hxx"

namespace vigra {

#ifndef NO_PARTIAL_TEMPLATE_SPECIALIZATION

namespace detail {

template<class T, class NEXT>
struct IntTypeList
{
    enum { size = sizeof(T)*8 };
    typedef T type;
    typedef NEXT next;
};

template<int SIZE, class LIST>
struct SelectIntegerType
{
    typedef typename 
       IfBool<(SIZE == LIST::size), 
           typename LIST::type,
           typename SelectIntegerType<SIZE, typename LIST::next>::type >::type
       type;
};

template<int SIZE>
struct SelectIntegerType<SIZE, VigraFalseType>
{
    typedef VigraFalseType type;
};

template<class LIST>
struct SelectBiggestIntegerType
{
    enum { cursize = LIST::size, 
           nextsize = SelectBiggestIntegerType<typename LIST::next>::size,
           size = (cursize < nextsize) ? nextsize : cursize };
    typedef typename 
       IfBool<(cursize < nextsize), 
           typename SelectBiggestIntegerType<typename LIST::next>::type,
           typename LIST::type>::type
       type;
};

template<>
struct SelectBiggestIntegerType<VigraFalseType>
{
    enum { size = 0 };
    typedef VigraFalseType type;
};

typedef IntTypeList<signed char, 
        IntTypeList<signed short,
        IntTypeList<signed int,
        IntTypeList<signed long,
        VigraFalseType > > > > SignedIntTypes;
typedef IntTypeList<unsigned char, 
        IntTypeList<unsigned short,
        IntTypeList<unsigned int,
        IntTypeList<unsigned long,
        VigraFalseType > > > > UnsignedIntTypes;

} // namespace detail

typedef detail::SelectIntegerType<8,  detail::SignedIntTypes>::type Int8;
typedef detail::SelectIntegerType<16, detail::SignedIntTypes>::type Int16;
typedef detail::SelectIntegerType<32, detail::SignedIntTypes>::type Int32;
typedef detail::SelectIntegerType<64, detail::SignedIntTypes>::type Int64;
typedef detail::SelectIntegerType<8,  detail::UnsignedIntTypes>::type UInt8;
typedef detail::SelectIntegerType<16, detail::UnsignedIntTypes>::type UInt16;
typedef detail::SelectIntegerType<32, detail::UnsignedIntTypes>::type UInt32;
typedef detail::SelectIntegerType<64, detail::UnsignedIntTypes>::type UInt64;

typedef detail::SelectBiggestIntegerType<detail::SignedIntTypes>::type   IntBiggest;
typedef detail::SelectBiggestIntegerType<detail::UnsignedIntTypes>::type UIntBiggest;

#else // NO_PARTIAL_TEMPLATE_SPECIALIZATION

typedef signed char    Int8;
typedef signed short   Int16;
typedef signed int     Int32;
typedef VigraFalseType Int64;
typedef unsigned char  UInt8;
typedef unsigned short UInt16;
typedef unsigned int   UInt32;
typedef VigraFalseType UInt64;

typedef Int32  IntBiggest;
typedef UInt32 UIntBiggest;

#endif // NO_PARTIAL_TEMPLATE_SPECIALIZATION

} // namespace vigra

#endif /* VIGRA_SIZED_INT_HXX */