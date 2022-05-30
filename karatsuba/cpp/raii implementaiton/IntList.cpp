//
// IntList.cpp
//
// created by PKXH on 29 Aug 2021
//
// class definitions for integer list type (using RAII patterns)
// (ex: an integer list for 0 is [0], 123 is [1,2,3], etc.)
//

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

#include <assert.h>
#include <limits.h>

#include <string>
#include <iostream>
#include <sstream>

#include "IntList.h"



// =============================================================================== 
// class IntList constructors
// =============================================================================== 

// *******************************************************************************
// IntList::IntList (initialize from initializer list)
// *******************************************************************************
//
// construct this IntList using an initializer list (of unsigned ints). 
// 
// -------------------------------------------------------------------------------
//                                IMPLEMENTATION
// ------------------------------------------------------------------------------- 
//
IntList::IntList( std::initializer_list<value_type> ilist ) 
    : il( ilist )
{   //
    // unpack the initializer list into the IntList; 
    // do validity-checking on each digit.
    //
    throw_on_invalid_min_size( il );
    throw_on_any_invalid_value_range( il );
    trim_leading_zeros( il );

#ifdef BUILD_UNIT_TESTS
    BOOST_ASSERT( IntList::is_zero_trimmed(*this) );
#endif
}
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(intlist_initializer_list_initialization_tests)
{   //
    // IntList construction/destruction
    //
    // With just the constructor we can only check that this thing comes and goes 
    // without blowing up when given something reasonable, and DOES blow up when
    // given something unreasonable.

    // BOOST doesn't seem to like this initialization notation...
    //   IntList ilx {22};
    // Have to enclose initializer list in parens...
    
    // check initializer list-based initialization
    BOOST_CHECK_NO_THROW( IntList il ( {    0} ); );  
    BOOST_CHECK_NO_THROW( IntList il ( {    1} ); ); 
    BOOST_CHECK_NO_THROW( IntList il ( {1,2,3} ); ); 
    BOOST_CHECK_NO_THROW( IntList il ( {0,0,0} ); ); 
    BOOST_CHECK_NO_THROW( IntList il ( {0,0,1} ); ); 
    BOOST_CHECK_NO_THROW( IntList il ( {0,0,1} ); ); 

    {   //
        // check trim leading zeros
        //
        IntList il1 {0,0,1,2,3};
        IntList il2 {1,2,3};

        BOOST_ASSERT( il1.size() == il2.size() );
        BOOST_ASSERT( il1 == il2 ); 
    }
    
    BOOST_CHECK_THROW( IntList il ( {0,0,22 } );, std::invalid_argument ); 
    BOOST_CHECK_THROW( IntList il ( {0,0,'a'} );, std::invalid_argument ); 
    BOOST_CHECK_THROW( IntList il ( {}        );, std::invalid_argument );
}
#endif // BUILD_UNIT_TESTS
// ------------------------------------------------------------------------------- 



// *******************************************************************************
// IntList::IntList (initialize from vector) 
// *******************************************************************************
//
// construct this IntList using an vector (of unsigned ints).
//
// -------------------------------------------------------------------------------
//                                IMPLEMENTATION
// ------------------------------------------------------------------------------- 
//
IntList::IntList( std::vector<value_type>& vec ) 
    : il( vec )
{   //
    // unpack the initializer list into the IntList; 
    // do validity-checking on each digit.
    //
    throw_on_invalid_min_size( il );
    throw_on_any_invalid_value_range( il );
    trim_leading_zeros( il );

#ifdef BUILD_UNIT_TESTS
    BOOST_ASSERT( IntList::is_zero_trimmed(*this) );
#endif
}
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(intlist_vector_initialization_tests)
{   //
    // IntList construction/destruction
    //
    // With just the constructor we can only check that this thing comes and goes 
    // without blowing up when given something reasonable, and DOES blow up when
    // given something unreasonable.

    // BOOST doesn't seem to like this initialization notation...
    //   IntList ilx {22};
    // Have to enclose initializer list in parens...
    
    // check initializer list-based initialization
    BOOST_CHECK_NO_THROW( IntList il ( {    0} ); );  
    BOOST_CHECK_NO_THROW( IntList il ( {    1} ); ); 
    BOOST_CHECK_NO_THROW( IntList il ( {1,2,3} ); ); 
    BOOST_CHECK_NO_THROW( IntList il ( {0,0,0} ); ); 
    BOOST_CHECK_NO_THROW( IntList il ( {0,0,1} ); ); 
    BOOST_CHECK_NO_THROW( IntList il ( {0,0,1} ); ); 

    {   //
        // check trim leading zeros
        //
        IntList il1 {0,0,1,2,3};
        IntList il2 {1,2,3};

        BOOST_ASSERT( il1.size() == il2.size() );
        BOOST_ASSERT( il1 == il2 ); 
    }
    
    BOOST_CHECK_THROW( IntList il ( {0,0,22 } );, std::invalid_argument ); 
    BOOST_CHECK_THROW( IntList il ( {0,0,'a'} );, std::invalid_argument ); 
    BOOST_CHECK_THROW( IntList il ( {}        );, std::invalid_argument );
}
#endif // BUILD_UNIT_TESTS
// ------------------------------------------------------------------------------- 



// *******************************************************************************
// IntList::IntList (initialize from unsigned int value)
// *******************************************************************************
//
// Construct this IntList using an unsigned integer value
//
// -------------------------------------------------------------------------------
//                                IMPLEMENTATION
// ------------------------------------------------------------------------------- 
//
IntList::IntList( unsigned int n ) 
{
    if (n==0)
        il.insert(il.begin(),0);
    else
        while (n>0) {
            il.insert(il.begin(), n%10);
            n/=10;
        }

#ifdef BUILD_UNIT_TESTS
    BOOST_ASSERT( IntList::is_zero_trimmed(*this) );
#endif
}
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(intlist_unsigned_int_initialization_tests)
{   //
    // IntList construction/destruction
    //
    // With just the constructor we can only check that this thing comes and goes 
    // without blowing up when given something reasonable, and DOES blow up when
    // given something unreasonable.
    //
    BOOST_CHECK_NO_THROW( IntList il (  0); );
    BOOST_CHECK_NO_THROW( IntList il (  1); );
    BOOST_CHECK_NO_THROW( IntList il (999); );

    {   //
        // Make sure zeros are trimmed
        //
        IntList il1(00000000007);  // octal 7 = decimal 7
        IntList il2 {7};

        BOOST_ASSERT( il1.size() == il2.size() );
        BOOST_ASSERT( il1 == il2 ); 
    }

    {   //
        // Check some random big int
        //
        IntList il1 (61587);
        IntList il2 {6,1,5,8,7};
        IntList il3 {8,6,7,5,3};

        BOOST_ASSERT( il1 == il2 );
        BOOST_ASSERT( il1 != il3 );
    }
}
#endif // BUILD_UNIT_TESTS
// ------------------------------------------------------------------------------- 



// *******************************************************************************
// IntList::Intlist (move constructor)
// *******************************************************************************
//
// Move constructor for cheap scope transfers 
//
// Takes an rvalue reference to an IntList and moves its value into this
// newly-constructed IntList
//
// -------------------------------------------------------------------------------
//                                 IMPLEMENTATION
// ------------------------------------------------------------------------------- 
//
IntList::IntList( IntList&& il )
    : il{std::move(il.il)}
{
#ifdef BUILD_UNIT_TESTS
    BOOST_ASSERT( IntList::is_zero_trimmed(*this) );
#endif
}
//
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(IntList_move_constructor_tests)
{   //
    // Make sure our IntList MOVES rather than COPIES under expected circumstances.
    //
    IntList il1 {1,2,3};
    set_previous_index_0_data_address(il1);

    // Should MOVE when returning from local variable in function.
    auto il2 = test_move_return(il1);
    BOOST_ASSERT( true == has_same_index_0_data_address_as_previous(il2) );
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------



// =============================================================================== 
// class IntList operators
// =============================================================================== 

// *******************************************************************************
//
// "move" operator for cheap rvalue assignments 
//
// Move the pointer to the actual data in the IntList specified by the right-side
// IntList (the rvalue reference prameter) into the IntList specified on the left
// (aka *this). 
//
// Leaves the right-hand IntList in a destructable state.
//
// *******************************************************************************
//
IntList& IntList::operator=(IntList&& il)
{
    this->il = std::move(il.il); // invoke std container move semantics
#ifdef BUILD_UNIT_TESTS
    BOOST_ASSERT( IntList::is_zero_trimmed( *this ) );
#endif
    return *this;
}
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(Intlist_move_assignment_operator_tests)
{   //
    // Make sure our IntList MOVES rather than COPIES under expected circumstances.
    //
    IntList il1 {1,2,3};
    set_previous_index_0_data_address(il1); // this is "previous"

    // TODO: make sure that this is how we should in fact test the move assignment

    IntList il2 {2,3,4};
    il2 = std::move(il1); // declare il1 to be an rvalue here; use move semantics 

    BOOST_ASSERT( true  == has_same_index_0_data_address_as_previous(il2) );
    BOOST_ASSERT( false == has_same_index_0_data_address_as_previous(il1) ); // should have been xfer'd away
}
#endif // BUILD_UNIT_TESTS
// ------------------------------------------------------------------------------- 



// TODO: but we're going to want to auto-zero the high end at some point, aren't we?

// *******************************************************************************
// IntList index ([]) operator (and friends)
// *******************************************************************************
//
// Returns reference to this object's value at the specified index. Range checked.
// Range checking will throw exception for bad indexes.
//
// *******************************************************************************
//
void IntList::throw_on_invalid_index( int i, const int_list_t& il) const
{
    if (i > il.size()) {
        std::string msg = "index must be >=0 and <= " + std::to_string( il.size() );
        throw std::out_of_range( msg );
    }
}
//
IntList::value_type& IntList::operator[]( int i )
{   //
    // given a valid index, offer indexed access to our number.
    // 
    throw_on_invalid_index( i, il );
    return il[i];
}
//
const IntList::value_type& IntList::operator[]( int i ) const
{   //
    // given a valid index, offer indexed read access to our number. 
    //
    throw_on_invalid_index( i, il );
    return il[i]; 
}
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(intlist_indexing_operator_tests)
{   //
    // Build confidence that we have per-digit access to our representation and 
    // that valid ranges are enforced.
    //
    IntList il1 = {1,2,3};

    // verify ability to access specified indices
    BOOST_ASSERT( il1[0] == 1 );
    BOOST_ASSERT( il1[1] == 2 );
    BOOST_ASSERT( il1[2] == 3 );

    // verify range-checking behavior
    BOOST_CHECK_THROW( auto val = il1[999]; , std::out_of_range );

    // verify ability to modify value
    il1[1] = 8;
    BOOST_ASSERT( il1[1] == 8 );
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------



// =============================================================================== 
// class IntList methods 
// =============================================================================== 

// *******************************************************************************
// IntList::clone
// *******************************************************************************
//
// return a clone of this integer list.
//
// *******************************************************************************
//
IntList IntList::clone( ) const 
{
    IntList il({0});
    il.il.clear(); 

    for ( auto p = this->cbegin(); p != this->cend(); ++p )
        il.push_back(*p);
    return il;
}
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(IntList_clone_function_tests)
{   //
    // Make sure our "clone" function can do the things we'll be expecting
    // it to do
    //
    IntList il1( {1,2,3} ); 

    // Make sure it doesn't just blow up
    BOOST_CHECK_NO_THROW( IntList il2 = il1.clone(); );

    // Make sure it is "equal" to that which it was cloned from
    IntList il3 = il1.clone();
    BOOST_ASSERT( il3 == il1 );
   
    // ...but also that it is a clone, and NOT a shared list
    set_previous_index_0_data_address(il1);
    BOOST_ASSERT(  true == has_same_index_0_data_address_as_previous(il1) );
    BOOST_ASSERT( false == has_same_index_0_data_address_as_previous(il3) ); // here's the actual test
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------



// *******************************************************************************
// IntList::throw_on_invalid_min_size
// *******************************************************************************
//
// Given an integer list representation, throw an exception if it has an invalid
// size; do nothing otherwise.
//
// *******************************************************************************
//
void IntList::throw_on_invalid_min_size( const int_list_t& il )
{
    if (il.size() == 0)
        throw std::invalid_argument( std::string("empty list is not allowed") );
}
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
void size_check_int_list_representation( std::initializer_list<IntList::value_type> il )
{   //
    // create an int_list_t representation and load it up with the supplied 
    // initializer list. The feed it to the function we are testing to see if
    // it does what we expect.
    //
    IntList::int_list_t ilt = il;
    IntList::throw_on_invalid_min_size( ilt );
}
#endif
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(IntList_throw_on_invalid_min_size_tests)
{   
    // Make sure it doesn't blow up when everything is good. 
    BOOST_CHECK_NO_THROW( size_check_int_list_representation( {1,2,3} ) );

    // ...and make sure that it DOES blow up when things aren't so good.
    BOOST_CHECK_THROW( size_check_int_list_representation( {} ), std::invalid_argument );
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------



// *******************************************************************************
// IntList::throw_on_invalid_value_range
// *******************************************************************************
//
// Given an integer list element value, throw an exception if it has an invalid
// value; do nothing otherwise.
//
// *******************************************************************************
//
void IntList::throw_on_invalid_value_range( const value_type& n )
{
    if ( !( n>=IntList::lower_bound && n<=IntList::upper_bound ) ) {
        throw std::invalid_argument( std::string("'")
                                   + std::to_string(n) 
                                   + std::string( "' is not a valid list digit; must be (" ) 
                                   + std::to_string(lower_bound) 
                                   + std::string( " <= n <= " )
                                   + std::to_string(upper_bound) 
                                   + std::string( ")" )
        );
    }
}
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
void value_check_int_list_value( const IntList::value_type& val )
{   //
    // feed the value to this private value-check function to see if it
    // behaves as we expect
    //
    IntList::throw_on_invalid_value_range( val );
}
#endif
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(IntList_throw_on_invalid_value_range_tests)
{   
    // Make sure it doesn't blow up when everything is good. 
    for ( int i=IntList::lower_bound; i <= IntList::upper_bound; i++ )
        BOOST_CHECK_NO_THROW( value_check_int_list_value( i ) );

    // ...and make sure that it DOES blow up when things aren't so good.
    BOOST_CHECK_THROW( value_check_int_list_value( IntList::lower_bound-1 ), std::invalid_argument );
    BOOST_CHECK_THROW( value_check_int_list_value( IntList::upper_bound+1 ), std::invalid_argument );
    BOOST_CHECK_THROW( value_check_int_list_value(                8675309 ), std::invalid_argument );
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------



// *******************************************************************************
// IntList::throw_on_any_invalid_value_range
// *******************************************************************************
//
// Given an integer list, throw an exception if any element it contains has an 
// invalid value; do nothing otherwise.
//
// *******************************************************************************
//
void IntList::throw_on_any_invalid_value_range( const int_list_t& il)
{
    std::for_each( il.begin(), il.end(), throw_on_invalid_value_range );
};
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
void value_check_int_list( std::initializer_list<IntList::value_type> il )
{
    IntList::int_list_t ilt = il; 
    IntList::throw_on_any_invalid_value_range( il );
}
#endif
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(IntList_throw_on_any_invalid_value_range_tests)
{   
    // Make sure it doesn't blow up when everything is good. 
    BOOST_CHECK_NO_THROW( value_check_int_list( {8,6,7,5,3,0,9} ) );

    // ...and make sure that it DOES blow up when things aren't so good.
    BOOST_CHECK_THROW( value_check_int_list( {8,6,7,5,3,0,9999} ), std::invalid_argument );
    BOOST_CHECK_THROW( value_check_int_list( {1,11,0,1}         ), std::invalid_argument );
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------



// *******************************************************************************
// IntList iterators 
// *******************************************************************************
//
// IntList iterators are simple pass-throughs of the internal std data 
// structure's iterators, so they should work; just do some cursory testing.
//
// *******************************************************************************
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(intlist_begin_end_function_tests)
{
    // Starting from 1 as IntList trims leading 0s by default
    IntList il1 {1,2,3,4,5,6,7,8,9};

    // use begin/end to iterate through il1 & check all of its values
    unsigned int cnt=1;
    for ( const auto& v : il1 ) {
        BOOST_ASSERT( v == cnt++ );
    }
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------



// *******************************************************************************
// IntList::push_back
// *******************************************************************************
//
// push the specified value onto the end of this integer list.
//
// *******************************************************************************
//
void IntList::push_back(value_type n)
{
    IntList::throw_on_invalid_value_range( n );
    il.push_back( n );
    IntList::trim_leading_zeros( il );
}
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(intlist_push_back_function_tests)
{
    // Starting from 1 as IntList trims leading 0s by default
    IntList il1 {1};

    // "push back" a few more numbers
    for (unsigned int i=2; i<=9; i++) {
        il1.push_back(i);
    }

    // iterate through il1 & check for presence of expected values
    unsigned int cnt=1;
    for ( const auto& v : il1 ) {
        BOOST_ASSERT( v == cnt++ );
    }
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------


// *******************************************************************************
// IntList::msd
// *******************************************************************************
//
// Return the value of the most significant digit.
//
// *******************************************************************************
//
// Defined in class declaration. 
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
void run_msd_tests()
{   //
    // Just try it out for a few values 
    // 
    IntList::int_list_t il1 {1};
    BOOST_ASSERT( IntList::msd(il1) == 1 );

    IntList::int_list_t il2 { 9,8,7,6,5,4,3,2,1 };
    BOOST_ASSERT( IntList::msd(il2) == 9 );

    IntList::delete_msd(il2);
    BOOST_ASSERT( IntList::msd(il2) == 8 );

    IntList::int_list_t il3 { 5,6,3,8,0,0,2,4,6,2,7,8,9,3,0,1,1,2,3,2,5,4,5,7,3,4,5,6,1,2,4,0,7,9,8,4,6,4,8 };
    BOOST_ASSERT( IntList::msd(il3) == 5 );

    IntList::delete_msd(il3);
    BOOST_ASSERT( IntList::msd(il3) == 6 );
}

BOOST_AUTO_TEST_CASE(intlist_msd_function_tests)
{ 
    run_msd_tests();
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------



// *******************************************************************************
// IntList::lsd
// *******************************************************************************
//
// Return the value of the least significant digit.
//
// *******************************************************************************
//
// Defined in class declaration. 
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
void run_lsd_tests()
{   //
    // Just try it out for a few values 
    // 
    IntList::int_list_t il1 {1};
    BOOST_ASSERT( IntList::lsd(il1) == 1 );

    IntList::int_list_t il2 { 9,8,7,6,5,4,3,2,1 };
    BOOST_ASSERT( IntList::lsd(il2) == 1 );

    IntList::delete_lsd(il2);
    BOOST_ASSERT( IntList::lsd(il2) == 2 ); 

    IntList::int_list_t il3 { 5,6,3,8,0,0,2,4,6,2,7,8,9,3,0,1,1,2,3,2,5,4,5,7,3,4,5,6,1,2,4,0,7,9,8,4,6,4,8 };
    BOOST_ASSERT( IntList::lsd(il3) == 8 );

    IntList::delete_lsd(il3);
    BOOST_ASSERT( IntList::lsd(il3) == 4 );
}

BOOST_AUTO_TEST_CASE(intlist_lsd_function_tests)
{ 
    run_lsd_tests();
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------


// *******************************************************************************
// IntList::trim_leading_zeros
// *******************************************************************************
//
// remove leading msb zeros until msb is not zero or the representation is 
// zero & length 1.
//
// *******************************************************************************
//
void IntList::trim_leading_zeros( IntList::int_list_t& il )
{
    while ( il.size() > 1 && IntList::msd(il) == 0 )
        IntList::delete_msd(il);
}
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS

void run_trim_leading_zeros_tests()
{   //
    // need to access internal types and structures so let's run the tests inside
    // this explicitly befriended function...
    //
    IntList ilx {0}; // need an instance so we can access the function for testing.

    {   //
        // test stripping of leading zeros from non-trivial number
        //
        IntList::int_list_t il {0,0,0,1,2,3};
        ilx.trim_leading_zeros( il );
        BOOST_ASSERT( il.size() == 3 );
        BOOST_ASSERT( il[0] == 1 );
    }
        
    {   //
        // test stripping of leading zeros from a long zero
        //
        IntList::int_list_t il {0,0,0,0};
        ilx.trim_leading_zeros( il );
        BOOST_ASSERT( il.size() == 1 );
        BOOST_ASSERT( il[0] == 0 );
    }
        
    {   //
        // test stripping zeros from a number with no leading zeros
        //
        IntList::int_list_t il {3,2,1,0};
        ilx.trim_leading_zeros( il );
        BOOST_ASSERT( il.size() == 4 );
        BOOST_ASSERT( il[0] == 3 );
    }
        
    {   //
        // test single-digit non-zero
        //
        IntList::int_list_t il {9};
        ilx.trim_leading_zeros( il );
        BOOST_ASSERT( il.size() == 1 );
        BOOST_ASSERT( il[0] == 9 );
    }
        
    {   //
        // test single-digit zero
        //
        IntList::int_list_t il {0};
        ilx.trim_leading_zeros( il );
        BOOST_ASSERT( il.size() == 1 );
        BOOST_ASSERT( il[0] == 0 );
    }
        
    {   //
        // test empty list
        //
        IntList::int_list_t il {};
        ilx.trim_leading_zeros( il );
        BOOST_ASSERT( il.size() == 0 );
    }
}

BOOST_AUTO_TEST_CASE(intlist_trim_leading_zeros_function_tests)
{
    run_trim_leading_zeros_tests();
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------



// *******************************************************************************
// IntList operator <=>  
// *******************************************************************************
//
// c++20 autogeneration of all comparison operator options by way of <=>
// ...but we need a slightly customized <=> to treat these things like
// numbers. See header for definition.
//
// *******************************************************************************
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(intlist_comparison_operator_tests)
{
    {   //
        // make sure that equal values are found to be equal
        //
        IntList il1 {3};
        IntList il2 {3};
        BOOST_ASSERT(!( il1 >  il2 ));
        BOOST_ASSERT(!( il1 <  il2 ));
        BOOST_ASSERT(   il1 >= il2  );
        BOOST_ASSERT(   il1 <= il2  );
        BOOST_ASSERT(   il1 == il2  );
        BOOST_ASSERT(!( il1 != il2 ));
    }

    {   //
        // make sure that unequal values are found to be unequal
        //
        IntList il1 {4};
        IntList il2 {5};
        BOOST_ASSERT(!( il1 >  il2 ));
        BOOST_ASSERT(   il1 <  il2  );
        BOOST_ASSERT(!( il1 >= il2 ));
        BOOST_ASSERT(   il1 <= il2  );
        BOOST_ASSERT(!( il1 == il2 ));
        BOOST_ASSERT(   il1 != il2  );
    }

    {   //
        // make sure that (longer) unequal values are found to be unequal
        //
        IntList il1 {1,2,3,4};
        IntList il2 {1,2,3,5};
        BOOST_ASSERT(!( il1 >  il2 ));
        BOOST_ASSERT(   il1 <  il2  );
        BOOST_ASSERT(!( il1 >= il2 ));
        BOOST_ASSERT(   il1 <= il2  );
        BOOST_ASSERT(!( il1 == il2 ));
        BOOST_ASSERT(   il1 != il2  );
    }


    {   //
        // make sure that leading zeros don't mess things up
        //
        IntList il1 {0,0,0,1,2,3};
        IntList il2 {1,2,3};
        BOOST_ASSERT(!( il1 >  il2 ));
        BOOST_ASSERT(!( il1 <  il2 ));
        BOOST_ASSERT(   il1 >= il2  );
        BOOST_ASSERT(   il1 <= il2  );
        BOOST_ASSERT(   il1 == il2  );
        BOOST_ASSERT(!( il1 != il2 ));
    }

    {   //
        // check comparison functionality against much-larger-than unsigned int values
        //
        IntList il1a {9,0,6,5,4,7,3,1,8,5,4,3,8,9,0,5,3,2,4,5,7,8,9,4,1,0,9,4,0,3,9,6,2,3,1,4,9,0,9,8,3,4,6,2,3,4,6,5,2,8,9,0,5,4,3,2,5,6,7,8}; // identical
        IntList il1b {9,0,6,5,4,7,3,1,8,5,4,3,8,9,0,5,3,2,4,5,7,8,9,4,1,0,9,4,0,3,9,6,2,3,1,4,9,0,9,8,3,4,6,2,3,4,6,5,2,8,9,0,5,4,3,2,5,6,7,8}; //
        IntList il2  {4,5,1,8,9,0,4,5,6,7,3,4,5,0,2,1,6,9,2,8,3,2,4,9,8,0,6,4,3,5,8,3,2,5,6,8,6}; // somewhat smaller!

        BOOST_ASSERT(!( il1a >  il1b ));  // long & identical
        BOOST_ASSERT(!( il1a <  il1b ));  //
        BOOST_ASSERT(   il1a >= il1b  );  //
        BOOST_ASSERT(   il1a <= il1b  );  //
        BOOST_ASSERT(   il1a == il1b  );  //
        BOOST_ASSERT(!( il1a != il1b ));  //

        BOOST_ASSERT(!( il2 >  il1a ));   // long w/ unequal values & lengths 
        BOOST_ASSERT(   il2 <  il1a  );   //
        BOOST_ASSERT(!( il2 >= il1a ));   //
        BOOST_ASSERT(   il2 <= il1a  );   //
        BOOST_ASSERT(!( il2 == il1a ));   //
        BOOST_ASSERT(   il2 != il1a  );   //

        BOOST_ASSERT( il1a <=> il2  > 0 );  // take me to your leader, c++20! 
        BOOST_ASSERT(  il2 <=> il1a < 0 );  // 
    }

    {   //
        // Make sure we're not doing per-character lexical comparison!
        //
        IntList a {9,9,9,9};
        IntList b {4,9,9,9,9};

        BOOST_ASSERT(a < b);
    }
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------



// *******************************************************************************
// IntList addition operator (+) 
// *******************************************************************************
//
// Add two arbitrarily-lengthed IntLists, resulting in an IntList result.
//
// *******************************************************************************
//
IntList operator+(const IntList& a, const IntList& b)
{
    std::vector<IntList::value_type> sum;

    unsigned int digit_sum = 0;
    unsigned int ones_sum  = 0;
    unsigned int carry     = 0;

    // Find the respective (possibly different) indices of each summand's least
    // significant digit, and walk towards the greatest significant digit (aka
    // towards index 0). Again, if the two summands are of different lengths,
    // they will get there at different times, and the one left decrementing
    // alone in that case will be partnered with a '0' during the per-digit sum.

    int i = a.size() - 1;
    int j = b.size() - 1;
    int digits_to_process = std::max(a.size(), b.size());

    while ( digits_to_process-- > 0 ) {

        digit_sum = ( (i >= 0) ? a[i--] : 0 )
                  + ( (j >= 0) ? b[j--] : 0 )
                  + carry;
        ones_sum = digit_sum%10;
        carry    = digit_sum/10;

        sum.push_back(ones_sum);
    }

    // don't forget about the last carry!
    if (carry != 0)
        sum.push_back(carry);

    // msd should be at index-0
    std::reverse( sum.begin(), sum.end() );

    return sum;
}
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(intlist_addition_operator_tests)
{   //
    // specific edge cases
    //
    {
        IntList a {0};
        IntList b {0};
        IntList a_plus_b {0};
        BOOST_CHECK( a + b == a_plus_b );
    }

    {
        IntList a {0,0,0,0};
        IntList b {0};
        IntList a_plus_b {0};
        BOOST_CHECK( a + b == a_plus_b );
    }

    {
        IntList a {0};
        IntList b {0,0,0,0};
        IntList a_plus_b {0};
        BOOST_CHECK( a + b == a_plus_b );
    }

    {
        IntList a {1,2,3};
        IntList b {4,5,6};
        IntList a_plus_b {5,7,9};
        BOOST_CHECK( a + b == a_plus_b );
    }

    {
        IntList a {0,0,1,2,3};
        IntList b {4,5,6};
        IntList a_plus_b {5,7,9};
        BOOST_CHECK( a + b == a_plus_b );
    }

    {
        IntList a (123);
        IntList b {0,0,4,5,6};
        IntList a_plus_b {5,7,9};
        BOOST_CHECK( a + b == a_plus_b );
    }

    {
        IntList a (9999);
        IntList b (1);
        IntList a_plus_b (10000);
        BOOST_CHECK( a + b == a_plus_b );
    }

    {   //
        // two long ones, same length w/ final carry
        //
        IntList a {9,4,5,6,3,7,8,3,5,7,4,1,0,4,3,5,6,7,2,9,0,8,5,1,8,4,3,9,0,6,4,3,4,7,5,3,2,7,9,0,1,2,0,4,3,6,2,8,1,0,9,8,5,8,1,2,6,3,2,5,7,4};
        IntList b {7,3,4,7,2,5,0,2,3,7,1,6,3,5,4,7,3,2,6,7,3,7,8,9,4,3,6,9,2,9,1,2,0,4,7,2,8,8,9,6,8,4,7,1,5,3,5,3,6,3,6,3,8,2,0,9,2,3,3,1,2,4};
        IntList a_plus_b {1,6,8,0,3,6,2,8,5,9,4,5,7,3,9,8,2,9,9,9,6,4,6,4,1,2,8,0,8,3,5,5,5,5,2,2,6,1,6,8,6,9,6,7,5,8,9,8,1,7,4,6,2,4,0,2,1,8,6,5,6,9,8};
        BOOST_CHECK( a + b == a_plus_b );
    }

    {   //
        // two long ones, different lengths
        //
        IntList a {5,5,2,0,8,4,2,2,5,3,2,2,7,1,0,4,6,0,9,0,0,4,2,5,6,7,3,2,7,9,9,7,8,4,3,4,6,8,8,9,5,4,8,2,3,9,5,8,7,2,3,0,7,4};
        IntList b {7,3,5,6,7,8,2,3,0,9,3,4,2,7,3,6,4,9,8,3,4,7,8,9,7,4,8,9,5,7,6,2,9,3,6,8,1,7,6,5,2,9,0,8,1,2,3,4,5,0,3,0,5,8,2,3,8,7,4,3};
        IntList a_plus_b {7,3,5,6,7,8,7,8,3,0,1,8,4,9,8,9,7,2,5,4,5,2,5,0,6,4,9,3,8,3,3,0,2,6,4,8,1,5,4,9,6,3,7,7,0,1,8,9,3,2,7,0,1,6,9,6,1,8,1,7};
        BOOST_CHECK( a + b == a_plus_b );
    }

    {   //
        // double-checking random values with c++ math
        //
        const unsigned int num_random_tests = 1000;

        std::srand(time(nullptr));

        for (auto i=0; i < num_random_tests; i++ ) {
            //
            // generate two random numbers, add them, and also add their integer
            // list equivalents; verify that the two sums match.
            //
            auto a = std::rand() % (UINT_MAX/2); // mind your head
            auto b = std::rand() % (UINT_MAX/2); //
            auto c = a + b;

            IntList s1(a);
            IntList s2(b);
            auto d = s1 + s2;
            
            std::stringstream error_msg_ss;
            error_msg_ss << "random test #" << i << ": " << a << " + " << b << " = " << c
                         << " (was expecting " << d.to_uint() << ")";
            BOOST_CHECK_MESSAGE( c == d.to_uint(), error_msg_ss.str() );
        }
    }
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------



// *******************************************************************************
// IntList subtraction operator (-) 
// *******************************************************************************
//
// Subtract the second arbitrarily-lengthed IntList from the first one and return 
// the result in another IntList.
//
// *******************************************************************************
//
IntList operator-(const IntList& a, const IntList& b)
{   //
    // make sure a>=b
    //
    if (!(a >= b)) {
        std::stringstream error_msg_ss;
        error_msg_ss << "a (" << a.to_str() << ") must be >= (" << b.to_str() << ")";
        throw std::invalid_argument(error_msg_ss.str());
    }

    //
    // make a note of our sizes; find the biggest
    //
    auto a_size = a.size();
    auto b_size = b.size();
    auto [max_size, min_size] = a_size >= b_size 
                              ? std::make_pair(a_size, b_size) 
                              : std::make_pair(b_size, a_size);

#ifdef BUILD_UNIT_TESTS
    BOOST_ASSERT( a_size >= b_size); // we expect only non-negative differences
#endif

    //
    // make copies of our inputs since we're going to be changing them during
    // the calculation
    //
    auto ac = a.clone();
    auto bc = b.clone();

    std::vector<IntList::value_type> diff;

    auto ai = ac.rbegin();                           // we're going to use iterators to subtract digit-by-digit so we can handle
    auto bi = bc.rbegin();                           // as arbitrarily-lengthed numbers as the computer an throw at us...

    for ( ; ai != ac.rend() ; ++ai ) {               // 'a' should be same size as 'b' or bigger, so use it to control full loop
        int bz;
        if (bi!=bc.rend()) {                         // 'b' may be shorter than 'a'; if it is we'll have to "fake" zeros for it
            bz = *bi;                                // once we run out of digits...
            ++bi;
        }
        else
            bz = 0;
        int d = *ai - bz;                            // subtract least-significant unprocessed digit in 'a' from counterpart in 'b'
#ifdef BUILD_UNIT_TESTS
        BOOST_ASSERT( -10 <= d && d < 10 );          // (we at most have a single digit minus another single digit)
#endif
        if (d < 0) {                                 // if a's digit was bigger than b's...
            auto brwi = ai+1;                        // get ready to start ripple-borrowing at the NEXT bigger digit...
            for ( ; *brwi == 0; ++brwi ) {           // while the next-bigger digit of 'a' is 0...
#ifdef BUILD_UNIT_TESTS
                BOOST_ASSERT( brwi != ac.rend() );   // (we always expect a >= b, so the final 'a' digit shoudl never be 0) 
#endif
                *brwi = 9;                           // make the 0 into a 9
            }
            *brwi -= 1;                              // we've finally reached a non-zero digit to "borrow" from, so borrow
            d += 10;                                 // add the borrowed 10 from the next digit to the underwater value
        }
        diff.insert(diff.begin(),d);                 // we are big endian, so push more significant values in at the front
    }

// we initially used this trashy array indexing version...
// isn't there some cleaner way to do it that
// these two ways? Should find a more elegant way to do this... 
//
//     for (long long int i=max_size-1; i >= 0; --i) {
//         auto size_diff = max_size - min_size;
//         int d = ac[i] - ((i-size_diff) < b_size ? bc[i-size_diff] : 0);  
//         if (d < 0) {                                                     
//             auto ci = 1;                                                 
//             while(ac[i-ci] == 0) {                                       
//                 BOOST_ASSERT( i-ci >= 0 );                               
//                 ac[i-ci] = 9;                                            
//                 ci -= 1;                                                 // and go to the next-bigger digit
//             }
//             ac[i-ci] -= 1;                                               
//             d += 10;                                                     
//         }
//         diff.insert(diff.begin(),d);                                     
// //        diff.push_back(d);
//     }
// 
// //    if (diff.size() == 0)
// //        diff.push_back(0);

    return IntList(diff);                            // move our calculated difference out to the caller
}
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(intlist_subtraction_operator_tests)
{   //
    // make sure the integer list subtraction is working as expected
    //
    using vui = std::vector<unsigned int>;

    {   //
        // make sure that invalid string inputs are rejected and called out by name
        //
        IntList a(998);
        IntList b(999);
        std::stringstream expected_error_msg_ss;
        expected_error_msg_ss << "a (" << a.to_str() << ") must be >= (" << b.to_str() << ")";
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
    {
        IntList a({0});
        IntList b({0});
        IntList a_minus_b({0});
        BOOST_CHECK( a - b == a_minus_b );
    }

    {
        IntList a({0,0,0,0});
        IntList b({0});
        IntList a_minus_b({0});
        BOOST_CHECK( a - b ==  a_minus_b );
    }

    {
        IntList a({0});
        IntList b({0,0,0,0});
        IntList a_minus_b({0});
        BOOST_CHECK( a - b == a_minus_b ); 
    }

    {
        IntList a(456);
        IntList b(123);
        IntList a_minus_b(333);
        BOOST_CHECK( a - b == a_minus_b );
    }

    {
        IntList a(456);
        IntList b({0,0,1,2,3});
        IntList a_minus_b(333);
        BOOST_CHECK( a - b == a_minus_b );
    }

    {
        IntList a({0,0,4,5,6});
        IntList b(123);
        IntList a_minus_b(333);
        BOOST_CHECK( a - b == a_minus_b );
    }

    {
        IntList a(10000);
        IntList b(1);
        IntList a_minus_b(9999);
        BOOST_CHECK( a - b == a_minus_b );
    }

    {
        IntList a(1792339757);
        IntList b(218598761);
        IntList a_minus_b(1573740996);
        BOOST_CHECK( a - b == a_minus_b );
    }

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

        IntList big(bigger);
        IntList small(smaller);
        auto d = big - small;

        std::stringstream error_msg_ss;
        error_msg_ss << "random test #" << i << ": " << bigger << " - " << smaller << " = " << c
                     << " (but we got " << d.to_uint() << ")";
        BOOST_CHECK_MESSAGE(c == d.to_uint(), error_msg_ss.str());
    }
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------



// *******************************************************************************
// A unsigned int representation of the contents of integer list (if it fits)
// *******************************************************************************
//
// Just scale out and add up powers of 10.
//
// *******************************************************************************
//
unsigned int IntList::to_uint() const
{
    IntList uint_max(UINT_MAX);
    if (*this <=> uint_max > 0) {
        std::string msg = "to_uint() integer must be no larger than UINT_MAX (" + std::to_string( UINT_MAX ) + ")";
        throw std::out_of_range( msg );
    }

    // start with the least significant digit, and add up the
    // powers of 10. NOTE that we have to process index 0, and
    // so the loop will fall through once we decrement into 
    // negative index territory...

    unsigned int sum = 0;
    unsigned int ten_to_the_i = 1;

    for ( int i=il.size()-1; i>=0; --i ) {
        sum += ten_to_the_i * il[i];
        ten_to_the_i *= 10;
    }

    return sum;
}
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(intlist_to_uint_tests)
{
    unsigned int ui1 = 0;
    IntList il1(ui1);
    BOOST_ASSERT( il1.to_uint() == ui1 );

    unsigned int ui2 = 7;
    IntList il2(ui2);
    BOOST_ASSERT( il2.to_uint() == ui2 );

    unsigned int ui3 = 99999;
    IntList il3(ui3);
    BOOST_ASSERT( il3.to_uint() == ui3);

    IntList il_too_big (UINT_MAX);
    il_too_big.push_back(9); // now we've done it
    BOOST_CHECK_THROW( unsigned int val = il_too_big.to_uint(), std::out_of_range);

    {   //
        // double-checking random values with c++ math
        //
        const unsigned int num_random_tests = 1000;

        std::srand(time(nullptr));

        for (auto i=0; i < num_random_tests; i++ ) {
            //
            // generate two random numbers, add them, and also add their integer
            // list equivalents; verify that the two sums match.
            //
            auto a = std::rand();
            IntList s1(a);
            auto b = s1.to_uint();

            std::stringstream error_msg_ss;
            error_msg_ss << "random test #" << i 
                         << ": IntList initialized with unsigned int " << a 
                         << " returned to_unit() value " << b 
                         << " (was expecting " << a << ")";
            BOOST_CHECK_MESSAGE( a == b, error_msg_ss.str() );
        }
    }
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------



// *******************************************************************************
// A string representation of the contents of integer list
// *******************************************************************************
//
std::string IntList::to_str() const
{
    std::stringstream str;

    for (auto i=il.begin(); i!=il.end(); ++i)
        str << *i ;

    return str.str(); 
}
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(intlist_to_str_tests)
{
    unsigned int ui1 = 0;
    IntList il1(ui1);
    auto t1 = il1.to_str();
    auto t2 = std::to_string(ui1);
    BOOST_ASSERT( il1.to_str() == std::to_string(ui1) );

    unsigned int ui2 = 7;
    IntList il2(ui2);
    BOOST_ASSERT( il2.to_str() == std::to_string(ui2) );

    unsigned int ui3 = 99999;
    IntList il3(ui3);
    BOOST_ASSERT( il3.to_str() == std::to_string(ui3) );

    IntList il_too_big (UINT_MAX);
    il_too_big.push_back(9); // should still be ok; string has arbitrary length 
    BOOST_CHECK_NO_THROW( auto val = il_too_big.to_str() );

    {   //
        // double-checking random values with c++ math
        //
        const unsigned int num_random_tests = 1000;

        std::srand(time(nullptr));

        for (auto i=0; i < num_random_tests; i++ ) {
            //
            // generate two random numbers, add them, and also add their integer
            // list equivalents; verify that the two sums match.
            //
            auto a = std::rand();
            IntList s1(a);
            auto b = s1.to_str();

            std::stringstream error_msg_ss;
            error_msg_ss << "random test #" << i 
                         << ": IntList initialized with unsigned int " << a 
                         << " returned to_str() value " << b 
                         << " (was expecting " << a << ")";
            BOOST_CHECK_MESSAGE( std::to_string(a) == b, error_msg_ss.str() );
        }
    }
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------



// *******************************************************************************
// set_previous_index_0_data_address (TEST BUILD-ONLY FUNCTION) 
// *******************************************************************************
//
// set the "previous" index-0 data address so we can later compare it with
// other ones.
//
// *******************************************************************************
//
#ifdef BUILD_UNIT_TESTS
void set_previous_index_0_data_address(IntList& il)
{
    has_same_index_0_data_address_as_previous(il);
}
#endif // BUILD_UNIT_TESTS
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(set_previous_index_0_data_address_function_tests)
{   //
    // Make sure the setup function does what we expect it to do
    //
    IntList il1 {1,2,3};
    IntList il2 {1,2,3};

    BOOST_CHECK_NO_THROW( set_previous_index_0_data_address(il1) );
    
    BOOST_ASSERT(  true == has_same_index_0_data_address_as_previous(il1) );
    BOOST_ASSERT( false == has_same_index_0_data_address_as_previous(il2) );
    BOOST_ASSERT(  true == has_same_index_0_data_address_as_previous(il2) );

    IntList il3 {1,2,3};

    BOOST_CHECK_NO_THROW( set_previous_index_0_data_address(il3) );

    BOOST_ASSERT(  true == has_same_index_0_data_address_as_previous(il3) );
    BOOST_ASSERT( false == has_same_index_0_data_address_as_previous(il2) );
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------



// *******************************************************************************
// has_same_index_0_data_address_as_previous (TEST BUILD-ONLY FUNCTION) 
// *******************************************************************************
//
// Given an integer list, inspect internal data structure and report 'true' if
// it is at the same address as the one previously passed to this function; in 
// all other cases, report 'false'.
//
// *******************************************************************************
//
#if defined(BUILD_UNIT_TESTS)
bool has_same_index_0_data_address_as_previous(IntList& il)
{   //
    // Assuming a non-empty list, check the address of the first item in the IntList's internal
    // list with that of the last IntList passed to the previous invocation of this function.
    // 
    static void* prev = nullptr; 
    bool is_same = (prev == &(il.il[0])); 
    prev = &(il.il[0]); 
    return is_same;
}
#endif
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(has_same_index_0_data_address_function_tests)
{   //
    // Make sure our testing helper function actually does what it's supposed to do!
    //
    IntList il1 {1,2,3};
    IntList il2 {1,2,3};

    set_previous_index_0_data_address(il1);

    BOOST_ASSERT(  true == has_same_index_0_data_address_as_previous(il1) ); // should match 
    BOOST_ASSERT( false == has_same_index_0_data_address_as_previous(il2) ); // should not match
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------



// *******************************************************************************
// test_move_return* (TEST BUILD-ONLY FUNCTIONS)
// *******************************************************************************
//
// Test the move constructor during function return-by-value 
//
// *******************************************************************************
//
#if defined(BUILD_UNIT_TESTS)
template<typename Container>
Container test_move_return(Container& c)
{   //
    // Note that we have to move our test list to a local IntList for the move to 
    // be automatically invoked by the compiler on return
    //
    Container inner_c = std::move(c);
    return inner_c;
}
#endif
//
template<typename Container>
std::pair<Container, Container> test_move_return_pair( Container& a_in, Container& b_in, bool apply_move_cast )
{
    Container a = std::move(a_in);
    Container b = std::move(b_in);

    // we expect that the compiler will not apply move semantics when creating this pair
    // because it is all happening within the scope of this function. So if we want the
    // move to happen we'll have to make sure the stuff to be moved is moved into tuple
    // prior to the return...

    if (apply_move_cast)
        return std::make_pair(std::move(a),std::move(b));
    else
        return std::make_pair(a, b);
}
//
//
// -------------------------------------------------------------------------------
//                             FUNCTIONALITY TESTS
// -------------------------------------------------------------------------------
//
#ifdef BUILD_UNIT_TESTS
BOOST_AUTO_TEST_CASE(test_move_return_function_tests)
{   //
    // Make sure our IntList MOVES rather than COPIES under expected circumstances.
    //
    std::vector<unsigned int> v1 {1,2,3};
    auto pv1 = &v1[0];

    // Should MOVE when returning from local variable in function.
    auto v2 = test_move_return(v1);
    BOOST_ASSERT( pv1 == &v2[0] );

    {   //
        // testing necessity of applying move cast
        // when creating tuple
        //
        std::vector<unsigned int> v2a {3,2,1};
        std::vector<unsigned int> v2b {6,5,4};
        auto pv2a = &v2a[0];
        auto pv2b = &v2b[0];

        // Should MOVE when returning from local variable in function
        auto [a,b] = test_move_return_pair(v2a, v2b, true);
        BOOST_ASSERT( pv2a == &a[0] );
        BOOST_ASSERT( pv2b == &b[0] ); 
    }

    {   //
        // testing necessity of applying move cast
        // when creating tuple
        //
        std::vector<unsigned int> v2a {3,2,1};
        std::vector<unsigned int> v2b {6,5,4};
        auto pv2a = &v2a[0];
        auto pv2b = &v2b[0];

        // Should NOT MOVE when returning from local variable in function
        auto [a,b] = test_move_return_pair(v2a, v2b, false);
        BOOST_ASSERT( pv2a != &a[0] );
        BOOST_ASSERT( pv2b != &b[0] ); 
    }
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------
