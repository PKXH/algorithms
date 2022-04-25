//
// IntList.cpp
//
// created by PKXH on 29 Aug 2021
//
// class definitions for integer list type (using RAII patterns)
// (ex: an integer list for 0 is [0], 123 is [1,2,3], etc.)
//
// *** UNDER CONSTRUCTION !!! *** UNDER CONSTRUCTION !!! *** UNDER CONSTRUCTION !!! 
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
#include "IntList.h"



// =============================================================================== 
// class IntList constructors
// =============================================================================== 

// *******************************************************************************
// IntList::IntList (initialize from initializer list)
// *******************************************************************************
//
// construct this IntList using an initializer list (of unsigned ints). If 
// trim_leading_zeros is specified, then do it!
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

    BOOST_ASSERT( IntList::is_zero_trimmed(*this) );
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
            il.insert(il.begin(),n%10);
            n/=10;
        }

    BOOST_ASSERT( IntList::is_zero_trimmed(*this) );
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
    BOOST_ASSERT( IntList::is_zero_trimmed(*this) );
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
    BOOST_ASSERT( IntList::is_zero_trimmed( *this ) );
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
// IntList::begin
// *******************************************************************************
//
// return a pointer to the first item in this integer list.
//
// *******************************************************************************
//
IntList::value_type* IntList::begin()
{
    return &il[0];
}
//
// *******************************************************************************
// IntList::end
// *******************************************************************************
//
// return a pointer to the "end" of this integer list.
//
// *******************************************************************************
//
IntList::value_type* IntList::end()
{
    return this->begin() + this->size(); 
}
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


const IntList::value_type* IntList::cbegin() const
{
//    return il.cbegin();
    return &il[0]; 
}

const IntList::value_type* IntList::cend() const
{
//    return il.cend();
    return this->cbegin() + this->size();
}


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
// test_move_return (TEST BUILD-ONLY FUNCTION)
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
}
#endif // BUILD_UNIT_TESTS
// -------------------------------------------------------------------------------
