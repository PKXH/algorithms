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
IntList::IntList( const IntList& il ) 
{
    for (auto& i : il) 
        this->il.push_back(i);

    remove_leading_zeros(this->il);
}

//
// construct this IntList using an unsigned in vector reference
//
IntList::IntList( const std::vector<unsigned int>& v ) 
{
    // TODO: make sure that we reject non-int'y vectors elegantly
    for (auto& i : v) {
        il.push_back(i);
    }

    remove_leading_zeros(il);
}

//
// construct this IntList using a numeric string reference
//
IntList::IntList( const std::string& s )
{
    // TODO: make sure that we reject non-int'y strings elegantly 
    for (auto& c : s) { 

        std::stringstream ss;
        unsigned int n;

        ss << c;
        ss >> n;
        il.push_back(n);
    }

    remove_leading_zeros(il);
}

//
// delete the most significant digit in this integer list
//
void IntList::delete_msd()
{
    il.erase(il.begin());
}

//
// remove the leading zeros from this integer list
//
void IntList::remove_leading_zeros( int_list_t& il ) 
{
    // while we have extra leading zeros, remove them
    while (il.size() > 1 && il.front() == 0) 
        delete_msd();
}

//
// indexing operator
//
int IntList::operator[](int i)
{
    return il[i];
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

    // assert that we have no leading zeros; since our constructor is expected to not
    // allow this to happen, it would be a surprise here
    
    BOOST_ASSERT( a->size() <= 1 || (*a)[0]!=0 );
    BOOST_ASSERT( b->size() <= 1 || (*b)[0]!=0 );

    if (size_a > size_b)
        return true;

    else if (size_a < size_b)
        return false;

    else
        if (size_a == 0)
            return true;
        
        else if ((*a)[0] != (*b)[0]) 
            return (*a)[0] >= (*b)[0];
        
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

    return greater_than_or_equal_to(a,b); 
}

//
// operator that returns 'true' if the integer encoded by '*this' is >= the integer encoded
// by '*that', and false otherwise
//
bool IntList::operator>=(const int_list_sp& that)
{   //
    // treat a int_list_sp like the IntList it points to by redirecting to the class-version
    // of the operator
    //
    return *this >= *that;
}

//
// Return the size of the integer list
//
int IntList::size()
{
    return il.size();
}

//
// create an int_list shared pointer from another int list
//
int_list_sp new_int_list_sp(const IntList& il) 
{
    return int_list_sp( new IntList(il) ); 
}

//
// create an int_list shared pointer from an std::vector integer list reference
//
int_list_sp new_int_list_sp(const std::vector<unsigned int>& v)
{
    return int_list_sp( new IntList(v) ); 
}

//
// create an int_list shared pointer from std::string integer string
//
int_list_sp new_int_list_sp(const std::string& s)
{
    return int_list_sp( new IntList(s) );
}


#ifdef BUILD_UNIT_TEST

//
// test string initialization integrity
//
BOOST_AUTO_TEST_CASE( test_initialization )
{
    // list contents can be compared directly with ==
    {
        std::list<unsigned int> l1 = {0,1,2,3};
        std::list<unsigned int> l2 = {0,1,2,3};

        BOOST_CHECK( l1 == l2 );
    }

    // note that when comparing contents the pointers must be dereferenced
    { 
        using vui = std::vector<unsigned int>;

        auto il1 = new_int_list_sp( vui({0,1,2,3,4}) );
        auto il2 = new_int_list_sp( vui({0,1,2,3,4}) );
        auto il3 = new_int_list_sp( vui({1,2,3,4,5}) );

        BOOST_CHECK( *il1 == *il2 );
        BOOST_CHECK( *il1 != *il3 );
    }

    // check list initialization from numeric string
    {
        using vui = std::vector<unsigned int>;
        std::string ns = "8675309";

        auto nsl     = new_int_list_sp( ns );
        auto nslchk1 = new_int_list_sp( vui({8,6,7,5,3,0,9}) );
        auto nslchk2 = new_int_list_sp( vui({7,3,6,5,0,0,0}) );

        std::cout << "nsl: ";
        for (auto& i:*nsl)
            std::cout << i << ",";
        std::cout << '\n';

        std::cout << "nslchk1: ";
        for (auto& i:*nslchk1)
            std::cout << i << ",";
        std::cout << '\n';

        BOOST_CHECK( *nsl == *nslchk1 );
        BOOST_CHECK( *nsl != *nslchk2 );
    }
    // test various inputs we're expected to handle
    // BOOST_CHECK( new_int_list_sp({})->length == 0 );
}

BOOST_AUTO_TEST_CASE( test_no_leading_zeros )
{   
    //
    // let's try a few obvious hand-gen'd samples and edge cases
    //
    using vui = std::vector<unsigned int>;

    BOOST_CHECK( *new_int_list_sp( vui({0,1,2,3,4,5}) ) == *new_int_list_sp( vui({1,2,3,4,5}) ));
    BOOST_CHECK( *new_int_list_sp( vui({0,0,0,0,1,2}) ) == *new_int_list_sp( vui({1,2}      ) ));
    BOOST_CHECK( *new_int_list_sp( vui({0,0,0,0,0}  ) ) == *new_int_list_sp( vui({0}        ) ));
    BOOST_CHECK( *new_int_list_sp( vui({0}          ) ) == *new_int_list_sp( vui({0}        ) )); 

    // And we should have tests for all initialization types, and also test
    // some cross initialization? Well, all of that should be done in the 
    // previous test case.
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
    //
    BOOST_CHECK( ! ( *new_int_list_sp( vui({0}        ) ) >= *new_int_list_sp( vui({1}      ) ) ) );
    BOOST_CHECK( ! ( *new_int_list_sp( vui({0}        ) ) >= *new_int_list_sp( vui({1,2,3}  ) ) ) );
    BOOST_CHECK( ! ( *new_int_list_sp( vui({0,0,0,0,0}) ) >= *new_int_list_sp( vui({1,2,3}  ) ) ) );
    BOOST_CHECK( ! ( *new_int_list_sp( vui({0,0,0,0,0}) ) >= *new_int_list_sp( vui({0,1,2,3}) ) ) );
    BOOST_CHECK( ! ( *new_int_list_sp( vui({0}        ) ) >= *new_int_list_sp( vui({0,1,2,3}) ) ) );
    BOOST_CHECK( ! ( *new_int_list_sp( vui({0,0,0,0}  ) ) >= *new_int_list_sp( vui({0,1,2,3}) ) ) );
    BOOST_CHECK( ! ( *new_int_list_sp( vui({0,0,0,0}  ) ) >= *new_int_list_sp( vui({0,0,0,1}) ) ) );
    BOOST_CHECK( ! ( *new_int_list_sp( vui({0}        ) ) >= *new_int_list_sp( vui({0,0,0,1}) ) ) );
}

// TODO: Add something to express the value contained by the int_list as a string 

#endif // BUILD_UNIT_TEST
