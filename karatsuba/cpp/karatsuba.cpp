//
// karatsuba.cpp
//
// Created by PKXH on 27 Apr 2021
//
// Implementation of Karatsuba multiplication
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

// ************************************************************************ 
// *** UNDER CONSTRUCTION *** UNDER CONSTRUCTION *** UNDER CONSTRUCTION ***
// ************************************************************************ 

// using namespace std;

// TODO: we're going to want to make sure that we trap all possible failures and convert them to 
// int return codes, or at least explore this possibility and its implications?

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
//    BOOST_CHECK( karatsuba( new_int_list_sp(0    ), new_int_list_sp(0)     ) == new_int_list_sp(0)       );
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
