//
// IntList.cpp
//
// created by PKXH on 25 Apr 2021
//
#include <iostream>
#include <sstream>
#include <string>
#include "IntList.h"

#ifdef BUILD_UNIT_TEST
#define BOOST_TEST_MODULE IntListTest
#include <boost/test/included/unit_test.hpp>
#endif

//
// construct this IntList using an IntList reference
//
IntList::IntList( const IntList& il, bool trim_leading_zeros ) 
{
    for (auto& i : il) 
        this->il.push_back(i);
    if (trim_leading_zeros)
        remove_leading_zeros();
}

//
// construct this IntList using an unsigned in vector reference
//
IntList::IntList( const std::vector<unsigned int>& v, bool trim_leading_zeros ) 
{
    for (auto& i : v) {
        il.insert(il.begin(),i);
    }

    if (trim_leading_zeros)
        remove_leading_zeros();
}

//
// construct this IntList using a numeric string reference
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

//
// construct this IntList using an unsigned integer value
//
IntList::IntList( unsigned int n )
{
    while (n>0) {
        il.push_back(n%10);
        n/=10;
    }

    // We expect no leading zeros from this process
    BOOST_ASSERT( this->size() <= 1 || this->msd()!=0 );
}

//
// delete the most significant digit in this integer list
//
void IntList::delete_msd()
{
    il.pop_back();
}

//
// remove the leading zeros from this integer list
//
void IntList::remove_leading_zeros() 
{
    // while we have extra leading zeros, remove them
    while (this->size() > 1 && this->msd() == 0) 
        delete_msd();
}

//
// indexing operator
//
WriteCheck IntList::operator[](unsigned int i)
{   //
    // if the caller is requesting an out-of-range digit (above msd), fake
    // zero padding and don't allow out-of-range write-backs
    // 
    static unsigned int zero = 0;
    BOOST_ASSERT( zero == 0 ); // nobody should be able to change this, but it's a crazy world
    return WriteCheck(i<il.size() ? (il[i]) : zero, i<il.size());
}

//
// equality operator
//
bool IntList::operator==(const IntList& il)
{
   return (this->il == il.il);
}

//
// inequality operator
//
bool IntList::operator!=(const IntList& il)
{
    return !(*this == il); 
}

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

//
// operator that returns 'true' if the integer encoded by '*this' is >= the integer encoded
// by 'that', and 'false' otherwise
//
bool IntList::operator>=(const IntList& that)
{   //
    // we expect the helper function might rip apart the integer lists while looking for
    // its answer on account of its curiously-un-const input references, so let's make copies 
    // that it can trash in the name of runtime efficiency 
    //
    auto a = new_int_list_sp(*this);
    auto b = new_int_list_sp(that);

    // assert that we have no leading zeros; since our constructor is expected to not
    // allow this to happen, it would be a surprise here
  
    std::stringstream error_msg_ss;
    error_msg_ss << "a:" << a->str() <<", b:" << b->str();

    BOOST_CHECK_MESSAGE( a->size() <= 1 || a->msd()!=0, error_msg_ss.str() );
    BOOST_CHECK_MESSAGE( b->size() <= 1 || b->msd()!=0, error_msg_ss.str() );

    return greater_than_or_equal_to(a,b); 
}

//
// operator that returns int_list_sp containing the sum of "this" int_list_sp and "that"
// int_list_sp; so I'm assuming this has to be something external... or I guess it can
// be an IntList member that returns an int_list_sp? Maybe it should be external... and
// namespace-protected...?
//
//int_list_sp IntList::operator+(const int_list_sp& that)

// What's the best way to handle the return? Something that needs to be copied? Or should we pass
// an object to be filled in?

// So I'm thinking maybe I need to change the writeback to extend the representation if 
// the index on the write is out-of-bounds and the digit being written is > 0 (if it is
// =0 then maybe just silently ignore?)

// Or maybe, we can specify whether or not to get rid of leading 0's? Seems like we should
// be able to specify the length of an all-0 list that doesn't auto-truncate. Maybe add a
// "remove leading zeros" member? And add a flag to the constructor that decides whether to
// apply it to the initialized value?

// So my pointer thing seems a little in-elegant here, since up until now we've been able to
// keep the smart point restriction outside of the IntList class implementation, but now we
// have to invoke it to get our sum object to receive our calculations... or should we define
// an external friend operator overload outside of the class?

int_list_sp operator+(int_list_sp& a, int_list_sp& b) 
{
    // determine sizes
    const auto this_size = a->size();
    const auto that_size = b->size();
    const auto  max_size = std::max(this_size, that_size);

    // sooo, make a max_size+1 zero'd int array, and maybe add directly from the
    // this and that, but capture out-of-bound indices and return zeros? Maybe a
    // function: get_zero_safe_index(this, 5)
    // Better yet, if the index is out of bounds, just return a zero!

    auto sum = new_int_list_sp( std::vector<unsigned int>(max_size+1, 0), false );

    for (int i=0; i < max_size; i++) {
        
        auto dgt_sum = (*a)[i] + (*b)[i] + (*sum)[i];
        auto dgt_div = dgt_sum / 10;
        auto dgt_mod = dgt_sum % 10;
        (*sum)[i]    = dgt_mod;
        (*sum)[i+1]  = dgt_div; 
    }

    sum->remove_leading_zeros();

    return sum;
}

//
// Return the size of the integer list
//
int IntList::size()
{
    return il.size();
}

//
// return the value of the most significant digit
//
unsigned int IntList::msd()
{
    return il.back();
}

//
// return the value of the least significant digit
//
unsigned int IntList::lsd()
{
    return il.front();
}

//
// A string representation of the contents of  integer list
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

//
// create an int_list shared pointer from another int list
//
int_list_sp new_int_list_sp(const IntList& il, bool trim_leading_zeros) 
{
    return int_list_sp( new IntList(il, trim_leading_zeros) ); 
}

//
// create an int_list shared pointer from an std::vector integer list reference
//
int_list_sp new_int_list_sp(const std::vector<unsigned int>& v, bool trim_leading_zeros)
{
    return int_list_sp( new IntList(v, trim_leading_zeros) ); 
}

//
// create an int_list shared pointer from std::string integer string
//
int_list_sp new_int_list_sp(const std::string& s, bool trim_leading_zeros)
{
    return int_list_sp( new IntList(s, trim_leading_zeros) );
}

//
// create an int_list shared pointer from unsigned int value
//
int_list_sp new_int_list_sp(unsigned int n)
{
    return int_list_sp( new IntList(n) );
}

#ifdef BUILD_UNIT_TEST

//
// test string initialization integrity
//
BOOST_AUTO_TEST_CASE( test_initialization )
{
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

    //
    // double-check random values with c++ math
    //
    const unsigned int  num_random_tests   = 1000;

    std::srand(std::time(nullptr));
    int random_variable = std::rand();

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

BOOST_AUTO_TEST_CASE( test_out_of_index_behavior )
{   //
    // make sure our integer list indexing operator is behaving properly
    //
    using vui = std::vector<unsigned int>;

    {   //
        // Make sure it returns valid initialized values, and zeros everywhere else
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

    {   //
        // starting with all-zero list
        //
        auto il = new_int_list_sp(vui({0,0,0,0,0,0,0,0,0,0}), false);

        (*il)[8] = 1;
        (*il)[3] = 2;

        // verify new values were written to the expected offsets, and that the
        // leading zero is intact
        BOOST_CHECK( *il == *new_int_list_sp(vui({0,1,0,0,0,0,2,0,0,0}), false) );

        il->remove_leading_zeros();

        // verify leading zero was chopped off
        BOOST_CHECK( *il == *new_int_list_sp(vui({1,0,0,0,0,2,0,0,0}), false) );

        //
        // Verify newly-out-of-range assignment is rejected
        //
        BOOST_CHECK_EXCEPTION( (*il)[9] = 5, 
                               std::out_of_range,
                               [](const std::out_of_range& ex){ return true; } 
                             );
    }
}

BOOST_AUTO_TEST_CASE( test_integer_list_addition )
{   
    using vui = std::vector<unsigned int>;

    auto il1 = new_int_list_sp(923);
    auto il2 = new_int_list_sp(211);

//    auto v = std::vector<unsigned int>(12,0);
//    auto ilv = new_int_list_sp(v,false);
//    std::cout << ilv->str();

    auto sum = il1 + il2;

    std::cout << sum->str();
}

#endif // BUILD_UNIT_TEST
