//
// karatsuba.cpp
//
// Created by PKXH on 22 May 2022
//
// Implementation of Karatsuba multiplication
//
// NOTE: when updating code, compile with:
// g++-11 -std=c++2a -DBUILD_KARATSUBA_UNIT_TEST_RUNNER IntList.cpp karatsuba.cpp
// and run a.out to test changes for breaks
//
#include <iostream>
#include <vector>

#include "IntList.h"

// use this define to run unit tests without externally-defined test runner
#if defined(BUILD_KARATSUBA_UNIT_TEST_RUNNER)
#define BOOST_TEST_MODULE Karatsuba Test
#define BUILD_UNIT_TESTS
#include <boost/test/included/unit_test.hpp>

// use these defines ONLY when linking to an externally-defined test runner 
#elif defined(BUILD_KARATSUBA_UNIT_TESTS) || defined(BUILD_ALL_UNIT_TESTS)
#define BUILD_UNIT_TESTS
#include <boost/test/unit_test.hpp>
#endif

#ifdef BUILD_UNIT_TESTS
#include <climits>
#endif

//using vui = std::vector<unsigned int>;

#ifdef BUILD_UNIT_TESTS
static unsigned int power(unsigned int base, unsigned int exponent) 
{
    unsigned int product = 1;
    for (auto n=0; n<exponent; n++)
        product *= base;
    return product;
}
#endif

// *******************************************************************************
// split the specified integer list into two, with the 1st value's msd starting 
// with il->size()-1, and the 2nd value's msd starting with spl_idx-1. 
//
// NOTE that "empty" integer lists resulting from the split are interpreted as 0!
// *******************************************************************************
//
std::pair<IntList, IntList> split_int_list( const IntList& il, unsigned int spl_idx)
{
    auto size = il.size();

    auto a = std::vector<unsigned int>();
    auto b = std::vector<unsigned int>();

    // TODO: ok, the problem is this: we may be splitting against a much bigger int, 
    // and so the split index will be far larger than our integer. In that case we don't
    // want to keep going in the same direction, "past the end" of our digits. Rather we
    // have some sort of prepend sitution. Well, this code in here right now seems kind
    // of correct? The left part of the split will be 0 and the right will just be the
    // number. So.....

// TODO: think we'll be removing this; keeping the simple behavior, and having an adaptor
// function that passes through an "adjusted" split index as if the incoming number was
// zero-padded 
//
//    if (spl_idx >= size) {
//        a.push_back(0);
//        std::copy(il.cbegin(), il.cend(), std::back_inserter(b));
//    } else {
        for ( auto i=0; i<size; i++ ) 
            if (i<spl_idx) 
                a.push_back(il[i]);
            else 
                b.push_back(il[i]);
//    }

    // IntList doesn't like to be initialized with empty lists; make sure
    // that we have valid numeric digits for it.
    if (a.size() == 0) a.push_back(0);
    if (b.size() == 0) b.push_back(0);

    // pack them together and return them
    IntList A(a);
    IntList B(b);

    // note that there are two steps to this last line: the first
    // moves the integers lists into the pair within the scope of this
    // function; the second is then moving the bundle out of this scope
    return std::make_pair(std::move(A), std::move(B));
}

#ifdef BUILD_UNIT_TESTS

BOOST_AUTO_TEST_CASE( integer_list_split_test )
{   //
    // test a mix of cases
    //
    IntList il( 12345 );

    {
        auto [a,b] = split_int_list( il, 1 );
        IntList expected_a(1);
        IntList expected_b(2345);

        BOOST_CHECK( a == expected_a && b == expected_b );
    }

    {
        auto [a,b] = split_int_list( il, 3 );
        IntList expected_a(123);
        IntList expected_b(45);

        BOOST_CHECK( a == expected_a && b == expected_b );
    }

    {
        auto [a,b] = split_int_list( il, 0 );
        IntList expected_a(0);
        IntList expected_b(12345);

        BOOST_CHECK( a == expected_a && b == expected_b );
    }

    {
        auto [a,b] = split_int_list( il, 10000 );
        IntList expected_a(12345);
        IntList expected_b(0);

        BOOST_CHECK( a == expected_a && b == expected_b );
    }
}

#endif // BUILD_UNIT_TESTS


// *******************************************************************************
// split the specified integer list into two, while assuming that the value
// specified by 'il' is leading-zero-padded to a total length of 'total_digits'. 
//
// NOTE that "empty" integer lists resulting from the split are interpreted as 0!
// *******************************************************************************
//
std::pair<IntList, IntList> split_zero_padded_int_list( const IntList& il, unsigned int spl_idx, unsigned int total_digits)
{
#ifdef BUILD_UNIT_TESTS 
    BOOST_ASSERT( total_digits >= il.size() );
#endif

    // Adjust the split index to act as if the integer list value has a total prepended zero-padded
    // length of 'total_digits' aka if the pad is longer than the split index, then the effective
    // split index passed along will be 0; otherwise, subtract off the pad for the effective index.

    auto pad_sz = total_digits - il.size();
    auto adj_spl_idx = (pad_sz < spl_idx ) ? spl_idx - pad_sz : 0;

    return split_int_list( il, adj_spl_idx );
}

#ifdef BUILD_UNIT_TESTS

BOOST_AUTO_TEST_CASE( zero_padded_integer_list_split_test )
{   //
    // test a mix of cases
    //
    IntList il( 12345 );

    const int num_zero_pad_tests = 10;
    for ( int i=0; i < num_zero_pad_tests; i++ ) {
        //
        // these two functions should always give the same result for the same
        // split if the zero pad has zero length...
        //
        auto [a ,b ] = split_int_list             ( il, i            );
        auto [az,bz] = split_zero_padded_int_list ( il, i, il.size() );

        BOOST_CHECK( a == az && b == bz ); 
    }

    {   //
        // splitting 012345 at i=1
        //
        auto [a,b] = split_zero_padded_int_list( il, 1, il.size() + 1 );
        IntList expected_a(0);
        IntList expected_b(12345);

        BOOST_CHECK( a == expected_a && b == expected_b );
    }

    {   //
        // splitting 0012345 at i=3
        //
        auto [a,b] = split_zero_padded_int_list( il, 3, il.size() + 2 );
        IntList expected_a(1);
        IntList expected_b(2345);

        BOOST_CHECK( a == expected_a && b == expected_b );
    }

    {   //
        // splitting 0000012345 at i=10
        //
        auto [a,b] = split_zero_padded_int_list( il, 10, il.size() + 5 );
        IntList expected_a(12345);
        IntList expected_b(0);

        BOOST_CHECK( a == expected_a && b == expected_b );
    }

    {   //
        // splitting 000000000000...0000000012345 at i=10
        //
        auto [a,b] = split_zero_padded_int_list( il, 10, il.size() + 1000 );
        IntList expected_a(0);
        IntList expected_b(12345);

        BOOST_CHECK( a == expected_a && b == expected_b );
    }
}

#endif // BUILD_UNIT_TESTS


/*
IntList karatsuba( const IntList& x, const IntList& y) {
    IntList test {0};
    return test;
}
*/


// *******************************************************************************
// Calculate a product using Karatsuba multiplication.
// x, y are integer list representations of arbitrarily large integers
// returns an IntList containing the product of the inputs 
// *******************************************************************************
//
IntList karatsuba(const IntList& x, const IntList& y) {

    auto x_size = x.size();
    auto y_size = y.size();

    // we interpret an empty list as 0
    if (x_size == 0 || y_size == 0) {
        IntList zero (0);
        return zero;
    }

    // if we're down to one digit, then multiply it out
    else if (x_size == 1 && y_size == 1) {

        auto xi = x.to_uint();
        auto yi = y.to_uint();

        IntList one_digit_product ( xi * yi );
        return one_digit_product;
    }

    else {

        auto max_size = std::max(x_size, y_size);
        auto m  = max_size/2 + (max_size%2?1:0); // take the ceil
        auto m2 = m*2;

        auto [a,b] = split_zero_padded_int_list( x, max_size-m, max_size );  // on odd-lengthed values, split so the most 
        auto [c,d] = split_zero_padded_int_list( y, max_size-m, max_size );  // significant part is smaller

        auto    s1 = karatsuba(a,c);
        auto    s2 = karatsuba(b,d);
        auto s1xs2 = karatsuba(a+b,c+d);

#ifdef BUILD_UNIT_TESTS
        BOOST_CHECK( s1xs2      >= s1 );
        BOOST_CHECK( s1xs2 - s1 >= s2 );
#endif 

        auto s3 = s1xs2 - s1 - s2;

        auto s1zs = s1.clone();
        for (auto i=0; i < m2; i++)
            s1zs.push_back(0);

        auto s3zs = s3.clone();
        for (auto i=0; i < m; i++)
            s3zs.push_back(0);

#ifdef BUILD_UNIT_TESTS
        {   //
            // if it shouldn't be greater than unsigned maxint, check our 0-appends against
            // the actual multiplied-out version of the numbers
            //
            static IntList max_int (UINT_MAX);

            if ( s1zs.size() < max_int.size() ) {
                // 
                // a.k.a. s1 == lhs * 10^(m2) ? 
                //
                IntList lhs_expo_version ( s1.to_uint() * power(10,m2) );
                BOOST_CHECK( s1zs == lhs_expo_version );
            }

            if ( s3zs.size() < max_int.size() ) {
                //
                // a.k.a. s3 == rhs * 10^m ?
                //
                IntList rhs_expo_version ( s3.to_uint() * power(10,m) );
                BOOST_CHECK( s3zs == rhs_expo_version );
            }
        }
#endif

        return s1zs + s2 + s3zs;  
    }
}

#ifdef BUILD_UNIT_TESTS

BOOST_AUTO_TEST_CASE( test_karatusba_multiplication )
{   //
    // test karatsuba multiplication against known results
    //

    //
    // specific simple cases
    //
    { IntList in1 (     0 ); IntList in2 (     0 ); IntList out (         0 ); BOOST_CHECK( karatsuba( in1, in2 ) == out ); }
    { IntList in1 (     1 ); IntList in2 (     1 ); IntList out (         1 ); BOOST_CHECK( karatsuba( in1, in2 ) == out ); } 
    { IntList in1 (     9 ); IntList in2 (     4 ); IntList out (        36 ); BOOST_CHECK( karatsuba( in1, in2 ) == out ); }
    { IntList in1 (    13 ); IntList in2 (     3 ); IntList out (        39 ); BOOST_CHECK( karatsuba( in1, in2 ) == out ); }
    { IntList in1 (    12 ); IntList in2 (    12 ); IntList out (       144 ); BOOST_CHECK( karatsuba( in1, in2 ) == out ); }
    { IntList in1 (   456 ); IntList in2 (    18 ); IntList out (      8208 ); BOOST_CHECK( karatsuba( in1, in2 ) == out ); }
    { IntList in1 (    18 ); IntList in2 (   456 ); IntList out (      8208 ); BOOST_CHECK( karatsuba( in1, in2 ) == out ); }
    { IntList in1 (  1234 ); IntList in2 (  5678 ); IntList out (   7006652 ); BOOST_CHECK( karatsuba( in1, in2 ) == out ); }
    { IntList in1 (  5678 ); IntList in2 (  1234 ); IntList out (   7006652 ); BOOST_CHECK( karatsuba( in1, in2 ) == out ); }
    { IntList in1 ( 56789 ); IntList in2 ( 12345 ); IntList out ( 701060205 ); BOOST_CHECK( karatsuba( in1, in2 ) == out ); }

    // beware of max uint
    { IntList in1 ( 73134 ); IntList in2 ( 81168 ); IntList out { 5,9,3,6,1,4,0,5,1,2 }; BOOST_CHECK( karatsuba( in1, in2 ) == out ); }

    {   //
        // go big or go hoem
        //
        IntList in1 {3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3,2,3};
        IntList in2 {2,7,1,8,2,8,1,8,2,8,4,5,9,0,4,5,2,3};
        IntList out {8,5,3,9,7,3,4,2,2,2,6,7,3,5,6,7,0,2,5,6,1,9,8,6,1,3,9,8,9,1,7,7,9,2,9};

        BOOST_CHECK( karatsuba( in1, in2 ) == out );
    } 

    {   //
        // moar
        //
        IntList in1 {3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3,2,3,8,4,6,2,6,4,3,3,8,3,2,7,9,5,0,2,8,8,4,1,9,7,1,6,9,3,9,9,3,7,5,1,0,5,8,2,0,9,7,4,9,4,4,5,9,2};
        IntList in2 {2,7,1,8,2,8,1,8,2,8,4,5,9,0,4,5,2,3,5,3,6,0,2,8,7,4,7,1,3,5,2,6,6,2,4,9,7,7,5,7,2,4,7,0,9,3,6,9,9,9,5,9,5,7,4,9,6,6,9,6,7,6,2,7};
        IntList out {8,5,3,9,7,3,4,2,2,2,6,7,3,5,6,7,0,6,5,4,6,3,5,5,0,8,6,9,5,4,6,5,7,4,4,9,5,0,3,4,8,8,8,5,3,5,7,6,5,1,1,4,9,6,1,8,7,9,6,0,1,1,2,7,0,6,7,7,4,3,0,4,4,8,9,3,2,0,4,8,4,8,6,1,7,8,7,5,0,7,2,2,1,6,2,4,9,0,7,3,0,1,3,3,7,4,8,9,5,8,7,1,9,5,2,8,0,6,5,8,2,7,2,3,1,8,4};

        BOOST_CHECK( karatsuba( in1, in2 ) == out );
    }

    //
    // random multiplication checks
    //
    std::srand(time(nullptr)); 
    int num_random_tests = 10000;
    const int max_rand_val = 9999;
    for ( auto i=1; i <= num_random_tests; i++ ) {

        unsigned long a = std::rand() % max_rand_val + 1;
        unsigned long b = std::rand() % max_rand_val + 1;
        auto          c = a*b;

        IntList in1 (a);
        IntList in2 (b);
        IntList out (c);

        auto d = karatsuba( in1, in2 );

        std::stringstream error_msg_ss;
        error_msg_ss << a << " * " << b << " = " << c << " (but karatsuba said " << d.to_str() << ")"; 

        BOOST_CHECK_MESSAGE( d == c, error_msg_ss.str() );
    }
}

#endif // BUILD_UNIT_TESTS
