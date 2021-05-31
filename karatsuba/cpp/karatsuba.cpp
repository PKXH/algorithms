//
// karatsuba.cpp
//
// Created by PKXH on 27 Apr 2021
//
// Implementation of Karatsuba multiplication
//
// NOTE: when updating code, compile with:
// clang++ -std=c++17 -DBUILD_KARATSUBA_UNIT_TEST_RUNNER IntList.cpp karatsuba.cpp
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

using vui = std::vector<unsigned int>;

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
std::pair<int_list_sp, int_list_sp> split_int_list( int_list_sp il, unsigned int spl_idx)
{
    auto size = il->size();

    auto a = std::vector<unsigned int>();
    auto b = std::vector<unsigned int>();

    for ( auto i=0; i<size; i++ ) 

        if (i>=spl_idx) 
            a.insert(a.begin(),(*il)[i]);

        else 
            b.insert(b.begin(),(*il)[i]);

    // pack them together and return them
    return std::make_pair(new_int_list_sp(a),new_int_list_sp(b));
}

#ifndef BUILD_UNIT_TESTS

BOOST_AUTO_TEST_CASE( integer_list_split_test )
{   //
    // test a mix of cases
    //
    using vui = std::vector<unsigned int>;
    
    il = new_int_list_sp(12345);

    auto [a1,a2] = split_int_list( il, 1 );
    BOOST_CHECK( a1 == new_int_list_sp(1234) && a2 == new_int_list_sp(5) );

    auto [b1,b2] = split_int_list( il, 3 );
    BOOST_CHECK( b1 == new_int_list_sp(12) && b2 == new_int_list_sp(345) );

    auto [c1,c2] = split_int_list( il, 0 );
    BOOST_CHECK( c1 == new_int_list_sp(12345) && c2 == new_int_list_sp(0) );

    auto [d1,d2] = split_int_list( il, 10000 );
    BOOST_CHECK( d1 == new_int_list_sp(0) && d2 == new_int_list_sp(12345) ); 
}

#endif // BUILD_UNIT_TESTS

// *******************************************************************************
// Calculate a product using Karatsuba multiplication.
// x, y are integer list representations of arbitrarily large integers
// returns an int_list_sp containing the product of the inputs 
// *******************************************************************************
//
int_list_sp karatsuba(int_list_sp x, int_list_sp y) {

    long int x_size = x->size();
    long int y_size = y->size();

    // we interpret an empty list as 0
    if (x_size == 0 || y_size == 0)
        return new_int_list_sp(0);

    // if we're down to one digit, then multiply it out
    else if (x_size == 1 && y_size == 1)
        return new_int_list_sp( x->uint() * y->uint() );

    else {

        auto max_size = std::max(x_size, y_size);
        unsigned int m = max_size/2 + (max_size%2?1:0); // take the ceil
        auto m2 = m*2;

        auto [a,b] = split_int_list(x,m);
        auto [c,d] = split_int_list(y,m);

        auto s1 = karatsuba(a,c);
        auto s2 = karatsuba(b,d);
        auto s1xs2 = karatsuba(a+b,c+d);
#ifdef BUILD_UNIT_TESTS
        BOOST_CHECK( s1xs2 >= s1 );
        BOOST_CHECK( s1xs2 - s1 >= s2 );
#endif 

        auto s3 = s1xs2 - s1 - s2;
        auto s1zs = new_int_list_sp(*s1);
        for (auto i=0; i < m2; i++)
            s1zs->append(0);
        auto s3zs = new_int_list_sp(*s3);
        for (auto i=0; i < m; i++)
            s3zs->append(0);

#ifdef BUILD_UNIT_TESTS
        {   //
            // if it shouldn't be greater than unsigned maxint, check our 0-appends against
            // the actual multiplied-out version of the numbers
            //
            BOOST_CHECK( s1zs->size() >= new_int_list_sp(UINT_MAX)->size() || s1zs == new_int_list_sp(s1->uint() * power(10,m2)) );
            BOOST_CHECK( s3zs->size() >= new_int_list_sp(UINT_MAX)->size() || s3zs == new_int_list_sp(s3->uint() * power(10,m )) );
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
    using vui = std::vector<unsigned int>;

    //
    // specific simple cases
    //
    BOOST_CHECK( karatsuba( new_int_list_sp(0    ), new_int_list_sp(0)     ) == new_int_list_sp(0)       );
    BOOST_CHECK( karatsuba( new_int_list_sp(1    ), new_int_list_sp(1)     ) == new_int_list_sp(1)       );
    BOOST_CHECK( karatsuba( new_int_list_sp(9    ), new_int_list_sp(4)     ) == new_int_list_sp(36)      );
    BOOST_CHECK( karatsuba( new_int_list_sp(13   ), new_int_list_sp(3)     ) == new_int_list_sp(39)      );
    BOOST_CHECK( karatsuba( new_int_list_sp(12   ), new_int_list_sp(12)    ) == new_int_list_sp(144)     );
    BOOST_CHECK( karatsuba( new_int_list_sp(456  ), new_int_list_sp(18)    ) == new_int_list_sp(8208)    );
    BOOST_CHECK( karatsuba( new_int_list_sp(18   ), new_int_list_sp(456)   ) == new_int_list_sp(8208)    );
    BOOST_CHECK( karatsuba( new_int_list_sp(1234 ), new_int_list_sp(5678)  ) == new_int_list_sp(7006652) );
    BOOST_CHECK( karatsuba( new_int_list_sp(5678 ), new_int_list_sp(1234)  ) == new_int_list_sp(7006652) );

    BOOST_CHECK( karatsuba( new_int_list_sp(56789), new_int_list_sp(12345)  ) == new_int_list_sp(701060205) );

    // beware of max uint
    BOOST_CHECK( karatsuba( new_int_list_sp(73134), new_int_list_sp(81168) ) == new_int_list_sp(vui({5,9,3,6,1,4,0,5,1,2})) );

    BOOST_CHECK( karatsuba(
        new_int_list_sp(vui({3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3,2,3})),
        new_int_list_sp(vui({2,7,1,8,2,8,1,8,2,8,4,5,9,0,4,5,2,3})))
        ==
        new_int_list_sp(vui({8,5,3,9,7,3,4,2,2,2,6,7,3,5,6,7,0,2,5,6,1,9,8,6,1,3,9,8,9,1,7,7,9,2,9})));

    BOOST_CHECK( karatsuba( 
        new_int_list_sp(vui({3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3,2,3,8,4,6,2,6,4,3,3,8,3,2,7,9,5,0,2,8,8,4,1,9,7,1,6,9,3,9,9,3,7,5,1,0,5,8,2,0,9,7,4,9,4,4,5,9,2})), 
        new_int_list_sp(vui({2,7,1,8,2,8,1,8,2,8,4,5,9,0,4,5,2,3,5,3,6,0,2,8,7,4,7,1,3,5,2,6,6,2,4,9,7,7,5,7,2,4,7,0,9,3,6,9,9,9,5,9,5,7,4,9,6,6,9,6,7,6,2,7}))) 
        == 
        new_int_list_sp(vui({8,5,3,9,7,3,4,2,2,2,6,7,3,5,6,7,0,6,5,4,6,3,5,5,0,8,6,9,5,4,6,5,7,4,4,9,5,0,3,4,8,8,8,5,3,5,7,6,5,1,1,4,9,6,1,8,7,9,6,0,1,1,2,7,0,6,7,7,4,3,0,4,4,8,9,3,2,0,4,8,4,8,6,1,7,8,7,5,0,7,2,2,1,6,2,4,9,0,7,3,0,1,3,3,7,4,8,9,5,8,7,1,9,5,2,8,0,6,5,8,2,7,2,3,1,8,4})));

    //
    // random multiplication checks
    //
    std::srand(std::time(nullptr)); 
    int num_random_tests = 10000;
    const int max_rand_val = 9999;
    for ( auto i=1; i <= num_random_tests; i++ ) {

        unsigned long a = std::rand() % max_rand_val + 1;
        unsigned long b = std::rand() % max_rand_val + 1;
        auto c = new_int_list_sp( a*b );
        auto d = karatsuba( new_int_list_sp(a), new_int_list_sp(b) );

        std::stringstream error_msg_ss;
        error_msg_ss << a << " * " << b << " = " << c->str() << " (but karatsuba said " << d->str() << ")"; 

        BOOST_CHECK_MESSAGE( d == c, error_msg_ss.str() );
    }
}

#endif // BUILD_UNIT_TESTS
