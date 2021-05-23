//
// IntList.cpp
//
// created by PKXH on 25 Apr 2021
//
// Method, function, and operator definitions for a representation for 
// arbitrarily-lengthed integers as a list of single digits 
// e.g., 3238675309 = {3,2,3,8,6,7,5,3,0,9}
//
#include <iostream>
#include <sstream>
#include <string>
#include "IntList.h"

// use this define to run unit tests without externally-defined test runner
#if defined(BUILD_INTLIST_UNIT_TEST_RUNNER)
#define BOOST_TEST_MODULE IntList Test
#define BUILD_UNIT_TESTS
#include <boost/test/included/unit_test.hpp>

// use these defines ONLY when linking to an externally-defined test runner
#elif defined(BUILD_INTLIST_UNIT_TESTS) || defined(BUILD_ALL_UNIT_TESTS) 
#define BUILD_UNIT_TESTS
#include <boost/test/unit_test.hpp>
#endif


// *******************************************************************************
// IntList constructors
// *******************************************************************************
//
// *******************************************************************************
// construct this IntList using an IntList reference
// *******************************************************************************
//
IntList::IntList( const IntList& il, bool trim_leading_zeros ) 
{
    for (auto& i : il) 
        this->il.push_back(i);
    if (trim_leading_zeros)
        remove_leading_zeros();
}

// *******************************************************************************
// construct this IntList using an unsigned int vector reference
// *******************************************************************************
//
IntList::IntList( const std::vector<unsigned int>& v, bool trim_leading_zeros ) 
{
    for (auto& i : v) {
        il.insert(il.begin(),i);
    }

    if (trim_leading_zeros)
        remove_leading_zeros();
}

// *******************************************************************************
// construct this IntList using a numeric string reference
// *******************************************************************************
//
IntList::IntList( const std::string& s, bool trim_leading_zeros)
{
    for (auto& c : s) { 

        std::stringstream ss;
        unsigned int n;

        ss << c;
        ss >> n;

        if (n==0 && c!='0') {
            throw std::invalid_argument(s);
        }

        il.insert(il.begin(),n);
    }

    if (trim_leading_zeros)
        remove_leading_zeros();
}

// *******************************************************************************
// construct this IntList using an unsigned integer value
// *******************************************************************************
//
IntList::IntList( unsigned int n )
{
    if (n==0)
        il.push_back(0);
    else
        while (n>0) {
            il.push_back(n%10);
            n/=10;
        }

#ifdef BUILD_UNIT_TESTS
    // We expect no leading zeros from this process
    BOOST_ASSERT( this->size() <= 1 || this->msd()!=0 );
#endif
}

#ifdef BUILD_UNIT_TESTS

BOOST_AUTO_TEST_CASE( test_initialization )
{   //
    // test string initialization integrity
    //
    {   //
        // list contents can be compared directly with ==
        //
        std::list<unsigned int> l1 = {0,1,2,3};
        std::list<unsigned int> l2 = {0,1,2,3};

        BOOST_CHECK( l1 == l2 );
    }

    {   // 
        // note that when comparing contents the pointers must be dereferenced
        //
        using vui = std::vector<unsigned int>;

        auto il1 = new_int_list_sp( vui({0,1,2,3,4}) );
        auto il2 = new_int_list_sp( vui({0,1,2,3,4}) );
        auto il3 = new_int_list_sp( vui({1,2,3,4,5}) );

        BOOST_CHECK( *il1 == *il2 );
        BOOST_CHECK( *il1 != *il3 );
    }

    {   //
        // test unsigned int initialization
        //
        using vui = std::vector<unsigned int>;
        unsigned int n = 7365000;

        auto nl     = new_int_list_sp( n );
        auto nlchk1 = new_int_list_sp( vui({8,6,7,5,3,0,9}) ); 
        auto nlchk2 = new_int_list_sp( vui({7,3,6,5,0,0,0}) );
        
        BOOST_CHECK( *nl != *nlchk1 );
        BOOST_CHECK( *nl == *nlchk2 );
    }

    {   //
        // check list initialization from numeric string
        //
        using vui = std::vector<unsigned int>;
        std::string ns = "8675309";

        auto nsl     = new_int_list_sp( ns );
        auto nslchk1 = new_int_list_sp( vui({8,6,7,5,3,0,9}) );
        auto nslchk2 = new_int_list_sp( vui({7,3,6,5,0,0,0}) );

        BOOST_CHECK( *nsl == *nslchk1 );
        BOOST_CHECK( *nsl != *nslchk2 );
    }

    {   //
        // make sure that invalid string inputs are rejected and called out by name
        //
        std::string  bad_integer_string = "ABC12D3";
        BOOST_CHECK_EXCEPTION( new_int_list_sp(bad_integer_string), 
                               std::invalid_argument,
                               [&](const std::invalid_argument& ex){ 
                                   BOOST_CHECK_EQUAL(ex.what(), bad_integer_string);
                                   return true;
                               }
                             );
    }
}

#endif // BUILD_UNIT_TESTS

// *******************************************************************************
// delete the most significant digit in this integer list
// *******************************************************************************
//
void IntList::delete_msd()
{
    il.pop_back();
}

// *******************************************************************************
// remove the leading zeros from this integer list
// *******************************************************************************
//
void IntList::remove_leading_zeros() 
{
    // while we have extra leading zeros, remove them
    while (this->size() > 1 && this->msd() == 0) 
        delete_msd();
}

#ifdef BUILD_UNIT_TESTS

BOOST_AUTO_TEST_CASE( test_no_leading_zeros )
{   //
    // let's try a few obvious hand-gen'd samples and edge cases
    //
    using vui = std::vector<unsigned int>;

    BOOST_CHECK( *new_int_list_sp( vui({0,1,2,3,4,5}) ) == *new_int_list_sp( vui({1,2,3,4,5}) ));
    BOOST_CHECK( *new_int_list_sp( vui({0,0,0,0,1,2}) ) == *new_int_list_sp( vui({1,2}      ) ));
    BOOST_CHECK( *new_int_list_sp( vui({0,0,0,0,0}  ) ) == *new_int_list_sp( vui({0}        ) ));
    BOOST_CHECK( *new_int_list_sp( vui({0}          ) ) == *new_int_list_sp( vui({0}        ) )); 
}

#endif // BUILD_UNIT_TESTS

// *******************************************************************************
// indexing operator
// *******************************************************************************
//
WriteCheck IntList::operator[](unsigned int i)
{   //
    // if the caller is requesting an out-of-range digit (above msd), fake
    // zero padding and don't allow out-of-range write-backs
    // 
    static unsigned int zero = 0;

#ifdef BUILD_UNIT_TESTS
    // nobody should be able to change this, but it's a crazy world
    BOOST_ASSERT( zero == 0 ); 
#endif

    return WriteCheck(i<il.size() ? (il[i]) : zero, i<il.size());
}

#ifdef BUILD_UNIT_TESTS

BOOST_AUTO_TEST_CASE( test_out_of_index_behavior )
{   //
    // make sure our integer list indexing operator is behaving properly
    //
    using vui = std::vector<unsigned int>;

    {   //
        // Verify that construction returns valid initialized values, and 
        // zeros everywhere else
        //
        auto il = new_int_list_sp(123);

        //
        // Verify little endian; zeros for non-initialized indices
        //
        BOOST_CHECK( (*il)[    0] == 3 );
        BOOST_CHECK( (*il)[    1] == 2 );
        BOOST_CHECK( (*il)[    2] == 1 );
        BOOST_CHECK( (*il)[    3] == 0 );
        BOOST_CHECK( (*il)[  100] == 0 ); 
        BOOST_CHECK( (*il)[10000] == 0 );
       
        //
        // Verify in-bounds assignment allowed 
        //
        BOOST_CHECK( *new_int_list_sp(vui({1,2,3})) == *il );
        (*il)[2] = 5;
        BOOST_CHECK( *new_int_list_sp(vui({5,2,3})) == *il );

        //
        // Verify out-of-range assignment is rejected
        //
        BOOST_CHECK_EXCEPTION( (*il)[3] = 5, 
                               std::out_of_range,
                               [](const std::out_of_range& ex){ return true; } 
                             );
    }
}

BOOST_AUTO_TEST_CASE( test_trim_leading_zero_functionality )
{   //
    // make sure we can trim/not trim leading zeros on our integer lists
    //
    using vui = std::vector<unsigned int>;

    // starting with all-zero list
    auto il = new_int_list_sp(vui({0,0,0,0,0,0,0,0,0,0}), false);

    (*il)[8] = 1;
    (*il)[3] = 2;

#ifdef BUILD_UNIT_TESTS
    // verify new values were written to the expected offsets, and that the
    // leading zero is intact
    BOOST_CHECK( *il == *new_int_list_sp(vui({0,1,0,0,0,0,2,0,0,0}), false) );
#endif 

    il->remove_leading_zeros();

#ifdef BUILD_UNIT_TESTS
    // verify leading zero was chopped off
    BOOST_CHECK( *il == *new_int_list_sp(vui({1,0,0,0,0,2,0,0,0}), false) );
#endif

    // Verify newly-out-of-range assignment is rejected
    BOOST_CHECK_EXCEPTION( (*il)[9] = 5, 
                           std::out_of_range,
                           [](const std::out_of_range& ex){ return true; } 
                         );
    
}

#endif // BUILD_UNIT_TESTS

// *******************************************************************************
// equality operator
// *******************************************************************************
//
bool IntList::operator==(const IntList& il)
{
   return (this->il == il.il);
}

// *******************************************************************************
// inequality operator
// *******************************************************************************
//
bool IntList::operator!=(const IntList& il)
{
    return !(*this == il); 
}

// *******************************************************************************
// return bool true if *a >= *b, false otherwise
// *******************************************************************************
//
bool IntList::greater_than_or_equal_to(int_list_sp& a, int_list_sp& b)
{
    auto size_a = a->size();
    auto size_b = b->size();

    // NOTE that there may be leading zeros at this point, since this is 
    // recursive function that keeps stripping off the first digit as it
    // does it's destructive comparison, eventually uncovering non-leading
    // zeros, so please refrain from too-clever-by-half assertions on the 
    // matter; any leading-zero checks should be in the enclosing >= 
    // operator overload.

    if (size_a > size_b)
        return true;

    else if (size_a < size_b)
        return false;

    else
        if (size_a == 0)
            return true;
        
        else if (a->msd() != b->msd()) 
            return a->msd() >= b->msd();
        
        else { 
            a->delete_msd();
            b->delete_msd();
            return greater_than_or_equal_to(a,b);
        }
}

// *******************************************************************************
// operator that returns 'true' if the integer encoded by '*this' is >= the 
// integer encoded by 'that', and 'false' otherwise
// *******************************************************************************
//
bool IntList::operator>=(const IntList& that)
{   //
    // we expect the helper function might rip apart the integer lists while looking for
    // its answer on account of its curiously-un-const input references, so let's make copies 
    // that it can trash in the name of runtime efficiency 
    //
    auto a = new_int_list_sp(*this);
    auto b = new_int_list_sp(that);

#ifdef BUILD_UNIT_TESTS
    // assert that we have no leading zeros; since our constructor is expected to not
    // allow this to happen, it would be a surprise here
  
    std::stringstream error_msg_ss;
    error_msg_ss << "a:" << a->str() <<", b:" << b->str();

    BOOST_CHECK_MESSAGE( a->size() <= 1 || a->msd()!=0, error_msg_ss.str() );
    BOOST_CHECK_MESSAGE( b->size() <= 1 || b->msd()!=0, error_msg_ss.str() );
#endif

    return greater_than_or_equal_to(a,b); 
}

#ifdef BUILD_UNIT_TESTS

BOOST_AUTO_TEST_CASE( test_greater_than_or_equal_to )
{   //
    // Some basic unit testing to make sure our greater-than-or-equal-to
    // functions are working as expected; these should be run whenever there
    // are changes to any of the functions tested below.    
    //
    using vui = std::vector<unsigned int>;

    //
    // specific/edge cases
    // 
    BOOST_CHECK( *new_int_list_sp( vui({1}      ) ) >= *new_int_list_sp( vui({0}        ) ) );
    BOOST_CHECK( *new_int_list_sp( vui({1,2,3}  ) ) >= *new_int_list_sp( vui({0}        ) ) );
    BOOST_CHECK( *new_int_list_sp( vui({1,2,3}  ) ) >= *new_int_list_sp( vui({0,0,0,0,0}) ) );
    BOOST_CHECK( *new_int_list_sp( vui({0,1,2,3}) ) >= *new_int_list_sp( vui({0,0,0,0,0}) ) );
    BOOST_CHECK( *new_int_list_sp( vui({0,1,2,3}) ) >= *new_int_list_sp( vui({0}        ) ) );
    BOOST_CHECK( *new_int_list_sp( vui({0,1,2,3}) ) >= *new_int_list_sp( vui({0,0,0,0}  ) ) );
    BOOST_CHECK( *new_int_list_sp( vui({0,0,0,1}) ) >= *new_int_list_sp( vui({0,0,0,0}  ) ) );
    BOOST_CHECK( *new_int_list_sp( vui({0,0,0,1}) ) >= *new_int_list_sp( vui({0}        ) ) );
    BOOST_CHECK( *new_int_list_sp( vui({0,0,1,2}) ) >= *new_int_list_sp( vui({0,1,2}    ) ) );
    //
    BOOST_CHECK( ! ( *new_int_list_sp( vui({0}        ) ) >= *new_int_list_sp( vui({1}      ) ) ) );
    BOOST_CHECK( ! ( *new_int_list_sp( vui({0}        ) ) >= *new_int_list_sp( vui({1,2,3}  ) ) ) );
    BOOST_CHECK( ! ( *new_int_list_sp( vui({0,0,0,0,0}) ) >= *new_int_list_sp( vui({1,2,3}  ) ) ) );
    BOOST_CHECK( ! ( *new_int_list_sp( vui({0,0,0,0,0}) ) >= *new_int_list_sp( vui({0,1,2,3}) ) ) );
    BOOST_CHECK( ! ( *new_int_list_sp( vui({0}        ) ) >= *new_int_list_sp( vui({0,1,2,3}) ) ) );
    BOOST_CHECK( ! ( *new_int_list_sp( vui({0,0,0,0}  ) ) >= *new_int_list_sp( vui({0,1,2,3}) ) ) );
    BOOST_CHECK( ! ( *new_int_list_sp( vui({0,0,0,0}  ) ) >= *new_int_list_sp( vui({0,0,0,1}) ) ) );
    BOOST_CHECK( ! ( *new_int_list_sp( vui({0}        ) ) >= *new_int_list_sp( vui({0,0,0,1}) ) ) );

    {   //
        // double-check random values with c++ math
        //
        const unsigned int  num_random_tests   = 1000;

        std::srand(std::time(nullptr));

        for (auto i=0; i < num_random_tests; i++ ) {

            auto a = std::rand();
            auto b = std::rand();

            std::stringstream error_msg_ss;
            error_msg_ss << "random test #" << i << ": " << a << " >= " << b;

            if (a >= b) {
                error_msg_ss << " was expecting TRUE";
                BOOST_CHECK_MESSAGE( *new_int_list_sp(a) >= *new_int_list_sp(b), error_msg_ss.str() );
            } else {
                error_msg_ss << " was expecting FALSE";
                BOOST_CHECK_MESSAGE( *new_int_list_sp(b) >= *new_int_list_sp(a), error_msg_ss.str() );
            }
        }
    }
}

#endif // BUILD_UNIT_TESTS

// *******************************************************************************
// Return the size of the integer list
// *******************************************************************************
//
int IntList::size()
{
    return il.size();
}

// *******************************************************************************
// return the value of the most significant digit
// *******************************************************************************
//
unsigned int IntList::msd()
{
    return il.back();
}

// *******************************************************************************
// return the value of the least significant digit
// *******************************************************************************
//
unsigned int IntList::lsd()
{
    return il.front();
}

// *******************************************************************************
// A string representation of the contents of integer list
// *******************************************************************************
//
std::string IntList::str()
{
    std::stringstream str;

    str << "{";

    for (auto i=il.rbegin(); i!=il.rend(); ++i)
        str << *i << ",";

    str << "}\n";

    return str.str(); 
}

#ifdef BUILD_UNIT_TESTS

BOOST_AUTO_TEST_CASE( test_integer_list_string_representation ) {

    const unsigned int num_random_tests = 1000;
    for (int i=0; i < num_random_tests; i++) {
    
        // generate a random unsigned int value
        std::srand(std::time(nullptr));
        unsigned int rval = std::rand(); 
        auto il = new_int_list_sp( rval );

        // convert random unsigned int value to a string
        std::stringstream rval_ss;
        rval_ss << rval;

        // strip out non-numeric characters from string representation
        std::stringstream ilstr;
        for (auto& i : il->str())
            if (std::isdigit(i))
                ilstr << i;

        // in case things go wrong, let the user know the identity of the culprits
        std::stringstream error_msg;
        error_msg << "(" << rval << " != " << ilstr.str() << ")";

        // make sure the uint() val and the initialization value are identical
        BOOST_CHECK_MESSAGE( ilstr.str() == rval_ss.str(), error_msg.str() );
    }
}

#endif // BUILD_UNIT_TESTS

// *******************************************************************************
// A unsigned int representation of the contents of integer list (if it fits)
// *******************************************************************************
//
unsigned int IntList::uint()
{
    unsigned int sum = 0;
    unsigned int ten_to_the_i = 1;

    for (unsigned int i=0; i<il.size(); i++) {
        sum += ten_to_the_i * il[i];
        ten_to_the_i *= 10;
    }

    return sum;
}

#ifdef BUILD_UNIT_TESTS

BOOST_AUTO_TEST_CASE( test_integer_list_uint_representation ) {

    const unsigned int num_random_tests = 1000;
    for (int i=0; i < num_random_tests; i++) {
    
        // generate a random unsigned int value
        std::srand(std::time(nullptr));
        unsigned int rval = std::rand(); 
        auto il = new_int_list_sp( rval );

        // make sure the uint() val and the initialization value are identical
        BOOST_CHECK( il->uint() == rval );
    }
}

#endif // BUILD_UNIT_TESTS

// *******************************************************************************
// int_list shared pointer factory functions
// *******************************************************************************

// create an int_list shared pointer from another int list
int_list_sp new_int_list_sp(const IntList& il, bool trim_leading_zeros) 
{ 
    return int_list_sp( new IntList(il, trim_leading_zeros) ); 
}

// create an int_list shared pointer from an std::vector integer list reference
int_list_sp new_int_list_sp(const std::vector<unsigned int>& v, bool trim_leading_zeros)
{
    return int_list_sp( new IntList(v, trim_leading_zeros) ); 
}

// create an int_list shared pointer from std::string integer string
int_list_sp new_int_list_sp(const std::string& s, bool trim_leading_zeros)
{
    return int_list_sp( new IntList(s, trim_leading_zeros) );
}

// create an int_list shared pointer from unsigned int value
int_list_sp new_int_list_sp(unsigned int n)
{
    return int_list_sp( new IntList(n) );
}

#ifdef BUILD_UNIT_TESTS

BOOST_AUTO_TEST_CASE( test_integer_list_factory_functions )
{   //
    // verify that all the factory functions generate the same representation
    // for equivalent initialization values
    //
    using vui = std::vector<unsigned int>;

    const unsigned int num_random_tests = 1000;
    for (int i=0; i < num_random_tests; i++) {
    
        // generate a random unsigned int value
        std::srand(std::time(nullptr));
        unsigned int rval = std::rand(); 

        // convert random unsigned int value to a string
        std::stringstream rval_ss;
        rval_ss << rval;

        // convert random unsigned int value to unsigned int vector of digits 
        unsigned int val = rval;
        std::vector<unsigned int> rval_vec;
        if (val==0)
            rval_vec.push_back(0);
        else
            while (val > 0) { 
                rval_vec.push_back( val%10 );
                val/=10;
        }
        std::reverse(rval_vec.begin(), rval_vec.end());

        // make four int_list_sp with the same value using different 
        // initialization methods
        auto ilsp1 = new_int_list_sp( rval          );
        auto ilsp2 = new_int_list_sp( rval_ss.str() );
        auto ilsp3 = new_int_list_sp( rval_vec      );
        auto ilsp4 = new_int_list_sp( *ilsp1        );

        // check every one against each other; they should all represent the 
        // same number
        BOOST_CHECK(ilsp1 == ilsp2);
        BOOST_CHECK(ilsp1 == ilsp3);
        BOOST_CHECK(ilsp1 == ilsp4);
        BOOST_CHECK(ilsp2 == ilsp3);
        BOOST_CHECK(ilsp2 == ilsp4);
        BOOST_CHECK(ilsp3 == ilsp4);
    }
}

#endif // BUILD_UNIT_TESTS

// *******************************************************************************
// operator for comparing two integer lists referenced by smart pointers
// *******************************************************************************
//
bool operator==(int_list_sp a, int_list_sp b)
{
    return *a == *b;
}

#ifdef BUILD_UNIT_TESTS

BOOST_AUTO_TEST_CASE( test_integer_list_equality )
{   //
    // make sure integer list equality is working as expected
    //
    BOOST_CHECK(     new_int_list_sp(0      ) == new_int_list_sp(0      )   );
    BOOST_CHECK(     new_int_list_sp(1234567) == new_int_list_sp(1234567)   );
    BOOST_CHECK( ! ( new_int_list_sp(1234567) == new_int_list_sp(7654321) ) );
}

#endif // BUILD_UNIT_TESTS

// *******************************************************************************
// operator for adding two integer lists referenced by smart pointers, resulting
// in an integer list sum also referenced by a smart pointer
// *******************************************************************************
//
int_list_sp operator+(int_list_sp a, int_list_sp b) 
{
    // determine sizes
    const auto this_size = a->size();
    const auto that_size = b->size();
    const auto  max_size = std::max(this_size, that_size);

    // sum will go in here; make sure it's big enough to hold it and that we
    // can writeback the sum's digits
    auto sum = new_int_list_sp( std::vector<unsigned int>(max_size+1, 0), false );

    // starting from lsd, add and carry up to msd 
    for (int i=0; i < max_size; i++) {
        
        auto dgt_sum = (*a)[i] + (*b)[i] + (*sum)[i];
        auto dgt_div = dgt_sum / 10;
        auto dgt_mod = dgt_sum % 10;
        (*sum)[i]    = dgt_mod;
        (*sum)[i+1]  = dgt_div; 
    }

    // if there was no final carry, clean up the space for it
    sum->remove_leading_zeros();

    return sum;
}

#ifdef BUILD_UNIT_TESTS

BOOST_AUTO_TEST_CASE( test_integer_list_addition )
{   // 
    // make sure integer list addition is working as expected
    //
    using vui = std::vector<unsigned int>;

    //
    // specific edge cases
    //
    BOOST_CHECK( new_int_list_sp(0)                + new_int_list_sp(0)                == new_int_list_sp(0)     );
    BOOST_CHECK( new_int_list_sp(vui({0,0,0,0}))   + new_int_list_sp(0)                == new_int_list_sp(0)     );
    BOOST_CHECK( new_int_list_sp(0)                + new_int_list_sp(vui({0,0,0,0}))   == new_int_list_sp(0)     );
    BOOST_CHECK( new_int_list_sp(123)              + new_int_list_sp(456)              == new_int_list_sp(579)   );
    BOOST_CHECK( new_int_list_sp(vui({0,0,1,2,3})) + new_int_list_sp(456)              == new_int_list_sp(579)   );
    BOOST_CHECK( new_int_list_sp(123)              + new_int_list_sp(vui({0,0,4,5,6})) == new_int_list_sp(579)   );
    BOOST_CHECK( new_int_list_sp(9999)             + new_int_list_sp(1)                == new_int_list_sp(10000) );

    {   //
        // double-checking random values with c++ math
        //
        const unsigned int num_random_tests = 1000;

        std::srand(std::time(nullptr));

        for (auto i=0; i < num_random_tests; i++ ) {
            //
            // generate two random numbers, add them, and also add their integer
            // list equivalents; verify that the two sums match.
            //
            auto a = std::rand();
            auto b = std::rand();
            auto c = a + b;
            auto d = (new_int_list_sp(a) + new_int_list_sp(b))->uint();

            std::stringstream error_msg_ss;
            error_msg_ss << "random test #" << i << ": " << a << " + " << b << " = " << c
                         << " (was expecting " << d << ")";
            BOOST_CHECK_MESSAGE( c == d, error_msg_ss.str() );
        }
    }
}

#endif // BUILD_UNIT_TESTS

// *******************************************************************************
// Return the difference (in integer list form) of two non-negative
// integer lists, with the requirement that a>=b.
// *******************************************************************************
//
int_list_sp operator-(int_list_sp a, int_list_sp b)
{   //
    // make sure a>=b
    //
    if (a->uint() < b->uint()) {
        std::stringstream error_msg_ss;
        error_msg_ss << "a (" << a->uint() << ") must be >= (" << b->uint() << ")";
        throw std::invalid_argument(error_msg_ss.str());
    }

    //
    // make a note of our sizes; find the biggest
    //
    auto a_size = a->size();
    auto b_size = b->size();
    auto [max_size, min_size] = a_size >= b_size 
                              ? std::make_pair(a_size, b_size) 
                              : std::make_pair(b_size, a_size);
    //
    // make copies of our inputs since we're going to be changing them during
    // the calculation
    //
    auto ap   = new_int_list_sp(*a);
    auto bp   = new_int_list_sp(*b);

    std::vector<unsigned int> diff;

    for (auto i=0; i < max_size; i++) {

        int d = (*ap)[i] - (*bp)[i];
        if (d < 0) {
            auto ci = 1;
            while ((*ap)[i+ci] == 0) {
                (*ap)[i+ci] = 9;
                ci += 1;
            }
            (*ap)[i+ci] = (*ap)[i+ci] - 1;
            d += 10;
        }
        diff.insert(diff.begin(),d);
    }

    return new_int_list_sp(diff);
}

#ifdef BUILD_UNIT_TESTS

BOOST_AUTO_TEST_CASE( test_integer_list_subtraction )
{   //
    // make sure the integer list subtraction is working as expected
    //
    using vui = std::vector<unsigned int>;

    {   //
        // make sure that invalid string inputs are rejected and called out by name
        //
        auto a = new_int_list_sp(998);
        auto b = new_int_list_sp(999);
        std::stringstream expected_error_msg_ss;
        expected_error_msg_ss << "a (" << a->uint() << ") must be >= (" << b->uint() << ")";
        BOOST_CHECK_EXCEPTION( a - b, 
                               std::invalid_argument,
                               [&](const std::invalid_argument& ex){ 
                                   BOOST_CHECK_EQUAL(ex.what(), expected_error_msg_ss.str());
                                   return true;
                               }
                             );
    }

    //
    // specific edge cases
    //
    BOOST_CHECK( new_int_list_sp(0)                - new_int_list_sp(0)                = new_int_list_sp(0)          );
    BOOST_CHECK( new_int_list_sp(vui({0,0,0,0}))   - new_int_list_sp(vui({0}))         = new_int_list_sp(vui({0}))   );
    BOOST_CHECK( new_int_list_sp(vui({0}))         - new_int_list_sp(vui({0,0,0,0}))   = new_int_list_sp(vui({0}))   );
    BOOST_CHECK( new_int_list_sp(456)              - new_int_list_sp(123)              = new_int_list_sp(333)        );
    BOOST_CHECK( new_int_list_sp(456)              - new_int_list_sp(vui({0,0,1,2,3})) = new_int_list_sp(333)        );
    BOOST_CHECK( new_int_list_sp(vui({0,0,4,5,6})) - new_int_list_sp(123)              = new_int_list_sp(333)        );
    BOOST_CHECK( new_int_list_sp(10000)            - new_int_list_sp(1)                = new_int_list_sp(9999)       );
    BOOST_CHECK( new_int_list_sp(1792339757)       - new_int_list_sp(218598761)        = new_int_list_sp(1573740996) );

    //
    // double-checking random values with c++ math
    //
    const unsigned int num_random_tests = 1000;

    for (auto i=1; i <= num_random_tests; i++ ) {
        //
        // generate two random numbers; subtract the bigger one from the smaller one, 
        // and also their integer list equivalents; verify that the two differences match.
        //
        auto a = std::rand();
        auto b = std::rand();
        auto [bigger, smaller] = a>b ? std::make_pair(a,b) : std::make_pair(b,a);
        auto c = bigger - smaller;
        auto d = new_int_list_sp(bigger) - new_int_list_sp(smaller);

        std::stringstream error_msg_ss;
        error_msg_ss << "random test #" << i << ": " << bigger << " - " << smaller << " = " << c
                     << " (but we got " << d->uint() << ")";
        BOOST_CHECK_MESSAGE(c == d->uint(), error_msg_ss.str());
    }
}

#endif // BUILD_UNIT_TESTS
