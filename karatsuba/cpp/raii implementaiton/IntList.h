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

// spaceship op is making demands...
#include <compare>

class IntList;
//class WriteCheck;

class IntList
//
// An iterable list of non-negative integer digits
//
{
public:
    using value_type = unsigned int;
    using int_list_t = std::vector<value_type>;

private:
    // list implementation type
//    using value_type = unsigned int;
//    using int_list_t = std::vector<value_type>;

    // list implementation 
    //int_list_t il;
    std::vector<unsigned int> il;

    // utility functions
    // pkxh TODO: we didn't use these consts in our privious version; did we just
    //            not think about it?
//    bool greater_than_or_equal_to( const IntList& a, const IntList& b); 

    // msd utility functions
    static inline const value_type& msd (const int_list_t& il ) { return il.front(); }
    static inline void delete_msd (int_list_t& il ) { il.erase(il.begin()); }

    void trim_leading_zeros( int_list_t& int_list ); 

//    value_type& value_at_index(int); // called by subscript operators

// #if defined(BUILD_UNIT_TESTS) 
//     inline bool is_zero_trimmed( const int_list_t& il ) { return msd(il) != 0 || il.size() == 1; }
// #endif

public:
    // constructors
    // PKXH TODO: make sure we want to use public constructors this time... I don't
    // think we're going to be using factory functions.
    // PKXH TODO: should I use a reference on this initializer list?
    // will it mess up initialization? Does it have to be this?
    //using value_type = unsigned int;
    //
    IntList( std::initializer_list<value_type> il ); // initializer list
    IntList( unsigned int ui ); // unsigned int 
    IntList( const IntList& ) = delete; // no copy constructor!
    IntList( IntList&& il); // move constructor

    // manual initialization
    void push_back(value_type);

    // class operators
    // PKXH TODO: this time, let's not put any operators inside the class that don't
    // need to be inside the class just as a matter of Stroustrupness.

//    bool operator==(const IntList& a) const;
//    bool operator!=(const IntList& a) const;
// IntList operator+(const IntList& a, const IntList& b);
// IntList operator-(const IntList& a, const IntList& b);
    auto operator<=>(const IntList&) const = default;
    IntList& operator=(const IntList& il) = delete; //no copy operator!
    IntList& operator=(IntList&& il); // move operator
    value_type& operator[]( int i ); // indexing operator
    const value_type& operator[]( int i) const; // read-only indexing operator

    unsigned long size() const { return il.size(); }
//    IntList clone();

// iterator helpers
    value_type* begin();
    value_type* end  ();

    const value_type* cbegin() const;
    const value_type* cend() const;

    IntList clone() const; // "clone" from an existing integer list

    // TODO: consider this mechanism: trims the zeros internally and then returns the this pointer so 
    // that external code can handle this thing floating in the middle of expressions... dumb idea? Too cute?
    // Hm... this is feeling too convoluted; and I don't think it's a pointer that we're going to want to be 
    // getting from this thing... maybe instead we should be enforcing 0-trim? Just put an assert at the end of
    // the constructor that makes the point that that's the way it should be... and any operation that manipulates
    // that structure should also reaffirm the zero-trim upon completion.
    //
    // THEN we can keep those comparision operators outside the class, not requiring any access to the
    // internal representation.
    //
    //IntList* trim_leading_zeros(); 

#if defined(BUILD_UNIT_TESTS)
    static inline bool is_zero_trimmed( const IntList& il ) { return msd(il.il) !=0 || il.size()<=1; }  
#endif

#if defined(BUILD_UNIT_TESTS)
    friend bool has_same_index_0_data_address_as_previous(IntList& il);
    friend void run_trim_leading_zeros_tests(); 
#endif
};

// bool operator>=(const IntList& a, const IntList& b);

#if defined(BUILD_UNIT_TESTS)
void set_previous_index_0_data_address(IntList& il);
#endif

#if defined(BUILD_UNIT_TESTS)
template<typename Container>
Container test_move_return(Container& c);
#endif

#endif // __int_list_h
