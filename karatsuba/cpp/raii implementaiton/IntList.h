//
// IntList.h
//
// created by PKXH on 29 Aug 2021
//
// class declaration for integer list type (using RAII patterns)
// (ex: an integer list for 0 is [0], 123 is [1,2,3], etc.)
//
// *** UNDER CONSTRUCTION !!! *** UNDER CONSTRUCTION !!! *** UNDER CONSTRUCTION !!! 
//
#ifndef __int_list_h
#define __int_list_h

#include <vector>
#include <memory>
#include <string>
// PKXH TODO: do we really need <memory> included?

class IntList;
//class WriteCheck;

class IntList
//
// An iterable list of non-negative integer digits
//
{
private:
    // list implementation type
    using value_type = unsigned int;
    using int_list_t = std::vector<value_type>;

    // list implementation 
    int_list_t il;

    // utility functions
    // pkxh TODO: we didn't use these consts in our privious version; did we just
    //            not think about it?
//    bool greater_than_or_equal_to( const IntList& a, const IntList& b); 

    // msd utility functions
    inline const value_type& msd (const int_list_t& il ) { return il.front(); }
    inline void delete_msd (int_list_t& il ) { il.erase(il.begin()); }

    void trim_leading_zeros( int_list_t& int_list ); 

public:
    // constructors
    // PKXH TODO: make sure we want to use public constructors this time... I don't
    // think we're going to be using factory functions.
    // PKXH TODO: should I use a reference on this initializer list?
    // will it mess up initialization? Does it have to be this?
    //using value_type = unsigned int;
    //
    IntList( std::initializer_list<unsigned int> il, bool trim_leading_zeros = true ); // initializer list
    IntList( unsigned int ui ); // unsigned int 
    IntList( const IntList& ) = delete; // no copy constructor!
    IntList( IntList&& il); // move constructor

    // manual initialization
    void push_back(unsigned int);

    // class operators
    // PKXH TODO: this time, let's not put any operators inside the class that don't
    // need to be inside the class just as a matter of Stroustrupness.

    bool operator==(const IntList& a);
    bool operator!=(const IntList& a);
// bool operator>=(const IntList& a, const IntList& b);
// IntList operator+(const IntList& a, const IntList& b);
// IntList operator-(const IntList& a, const IntList& b);
    IntList& operator=(const IntList& il) = delete; //no copy operator!
    IntList& operator=(IntList&& il); // move operator
    unsigned int& operator[]( int i ); // indexing operator

    int size() { return il.size(); }
//    IntList clone();

// iterator helpers
    unsigned int* begin();
    unsigned int* end  ();

    IntList clone(); // "clone" from an existing integer list

//
#if defined(BUILD_UNIT_TESTS)
    friend bool has_same_index_0_data_address_as_previous(IntList& il);
    friend void run_trim_leading_zeros_tests(); 
#endif
};

#if defined(BUILD_UNIT_TESTS)
void set_previous_index_0_data_address(IntList& il);
#endif

#if defined(BUILD_UNIT_TESTS)
template<typename Container>
Container test_move_return(Container& c);
#endif

#endif // __int_list_h
