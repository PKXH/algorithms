//
// karatsuba.cpp
//
// Created by PKXH on 27 Apr 2021
//
// Implementation of Karatsuba multiplication
//
#include <iostream>
#include <list>

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

// ************************************************************************ 
// *** UNDER CONSTRUCTION *** UNDER CONSTRUCTION *** UNDER CONSTRUCTION ***
// ************************************************************************ 

// using namespace std;

// TODO: we're going to want to make sure that we trap all possible failures and convert them to 
// int return codes, or at least explore this possibility and its implications?


//LargeInt karatsub(unsigned long int x, unsigned long int y) {
//
//    return karatsuba(LargeInt(x), LargeInt(y)); 
//}

int_list_sp karatsuba(int_list_sp, int_list_sp y) {
    //
    // Calculate a product using Karatsuba multiplication.
    //
    // x,y are arbitrarily large integers to be multiplied.
    //
//    unsigned int x_size = x->size(
//    unsigned int y_size = y->size();
//
//    auto xs = new_int_list_sp(*x); // TODO: do we really need to make these copies?
//    auto ys = new_int_list_sp(*y);
//
//    // we interpret an empty list as 0
//    if (x_size == 0 || y_size == 0)
//        return new_int_list_sp(0);
//
//    // if we're down to one digit, then multiply it out
//    else if (x_size == 1 || y_size == 1)
//        return new_int_list_sp( x->uint() * y->uint() );
//
//    else {
//
//    auto m = 
//
//
//    }
    throw std::runtime_error("not implemented");
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

    // beware of max uint
    BOOST_CHECK( karatsuba( new_int_list_sp(73134), new_int_list_sp(81168) ) == new_int_list_sp(vui({5,9,3,6,1,4,0,5,1,2})) );

    //
    // random multiplication checks
    //
    std::srand(std::time(nullptr)); 
    int num_random_tests = 1000;
    for ( auto i=1; i <= num_random_tests; i++ ) {

        unsigned long a = std::rand();
        unsigned long b = std::rand();
        auto c = a * b;
        auto d = (new_int_list_sp(a) + new_int_list_sp(b))->uint();

        std::stringstream error_msg_ss;
        error_msg_ss << a << " + " << b << " = " << c << " (but karatsuba said " << d << ")"; 

        BOOST_CHECK_MESSAGE( d == c, error_msg_ss.str() );
    }
}

#endif // BUILD_UNIT_TESTS
