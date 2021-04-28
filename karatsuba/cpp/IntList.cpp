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
IntList::IntList( IntList& il ) 
{
    for (auto& i : il) {
        this->il.push_back(i);
    }
}

//
// construct this IntList using an unsigned in vector reference
//
IntList::IntList( std::vector<unsigned int>& v ) 
{
    // TODO: make sure that we reject non-int'y vectors elegantly
    for (auto& i : v) {
        il.push_back(i);
    }
}

//
// construct this IntList using a numeric string reference
//
IntList::IntList( std::string& s )
{
    // TODO: make sure that we reject non-int'y strings elegantly 
    for (auto& c : s) { 

        std::stringstream ss;
        unsigned int n;

        ss << c;
        ss >> n;
        il.push_back(n);
    }
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

//
// create an int_list shared pointer from another int list
//
int_list_sp new_int_list_sp(IntList& il) 
{
    return int_list_sp( new IntList(il) ); 
}

//
// create an int_list shared pointer from an std::vector integer list
//
int_list_sp new_int_list_sp( std::vector<unsigned int> v )
{
    return int_list_sp( new IntList(v) ); 
}

//
// create an int_list shared pointer from an std::vector integer list reference
//
int_list_sp new_int_list_sp( std::vector<unsigned int>& v )
{
    return int_list_sp( new IntList(v) ); 
}

//
// create an int_list shared pointer from std::string integer string
//
int_list_sp new_int_list_sp( std::string& s )
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
    std::list<unsigned int> l1 = {0,1,2,3};
    std::list<unsigned int> l2 = {0,1,2,3};

    BOOST_CHECK( l1 == l2 );

    // note that when comparing contents the pointers must be dereferenced
    auto il1 = new_int_list_sp({0,1,2,3,4});
    auto il2 = new_int_list_sp({0,1,2,3,4});
    auto il3 = new_int_list_sp({1,2,3,4,5});

    BOOST_CHECK( *il1 == *il2 );
    BOOST_CHECK( *il1 != *il3 );

    // check list initialization from numeric string
    std::string ns = "8675309";

    auto nsl     = new_int_list_sp(ns);
    auto nslchk1 = new_int_list_sp({8,6,7,5,3,0,9});
    auto nslchk2 = new_int_list_sp({7,3,6,5,0,0,0});

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

BOOST_AUTO_TEST_CASE( test_no_leading_zeros )
{   
    //
    // let's try a few obvious hand-gen'd samples and edge cases
    //
    BOOST_CHECK( *new_int_list_sp({0,1,2,3,4,5}) == *new_int_list_sp({1,2,3,4,5}));
    BOOST_CHECK( *new_int_list_sp({0,0,0,0,1,2}) == *new_int_list_sp({1,2}      ));
    BOOST_CHECK( *new_int_list_sp({0,0,0,0,0})   == *new_int_list_sp({0}        ));
    BOOST_CHECK( *new_int_list_sp({0})           == *new_int_list_sp({0}        )); 

    // So we're going to want to integrate leading-zero stripping for
    // all creation methods; seems like maybe all of these should be 
    // constructors for a slightly more elabarate int_list type? Maybe
    // have the object be protected to some factory function that does
    // the shared_pointer allocation? And how did static linking work 
    // to make the file not accessible from the outside again? (not that
    // we're going to be using that method...)
    
    // And we should have tests for all initialization types, and also test
    // some cross initialization? Well, all of that should be done in the 
    // previous test case.
}

#endif // BUILD_UNIT_TEST
