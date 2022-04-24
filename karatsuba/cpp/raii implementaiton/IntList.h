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
#include <compare>
// PKXH TODO: do we really need <memory> included?

class IntList;
//class WriteCheck;

class IntList
//
// An iterable list of non-negative integer digits
//
{
public:
    // list implementation type
    using value_type = unsigned int;
    using int_list_t = std::vector<value_type>;

private:
    // list implementation 
    std::vector<unsigned int> il;

    // msd utility functions
    static inline const value_type& msd (const int_list_t& il ) { return il.front(); }
    static inline void delete_msd (int_list_t& il ) { il.erase(il.begin()); }

    void trim_leading_zeros( int_list_t& int_list ); 

public:
    // constructors
    // PKXH TODO: should I use a reference on this initializer list?
    // will it mess up initialization? Does it have to be this?
    //
    IntList( std::initializer_list<value_type> il ); // init by initializer list
    IntList( unsigned int ui );                      // init by unsigned int 

    // copy & move semantics / construction
    IntList( const IntList& ) = delete; // no copy constructor!
    IntList( IntList&& il);             // yes move constructor

    // manual initialization
    void push_back(value_type);

    // class operators
    // PKXH TODO: this time, let's not put any operators inside the class that don't
    // need to be inside the class just as a matter of Stroustrupness.

    // c++20 autocomparison generation
    auto operator<=>(const IntList&) const = default;

    // copy & move semantics / operators 
    IntList& operator=(const IntList& il) = delete; //no copy operator!
    IntList& operator=(IntList&& il); // move operator

    // instead of copy constructor
    IntList clone() const; // "clone" from an existing integer list

    // indexing operations
    value_type& operator[]( int i );
    const value_type& operator[]( int i) const;

    unsigned long size() const { return il.size(); }

    // iterator support 
    value_type* begin();
    value_type* end  ();
    //
    const value_type* cbegin() const;
    const value_type* cend() const;

#if defined(BUILD_UNIT_TESTS)
    // anything that monkeys with the representation should test this condition before returning
    static inline bool is_zero_trimmed( const IntList& il ) { return msd(il.il) !=0 || il.size()<=1; }  
#endif

#if defined(BUILD_UNIT_TESTS)
    // for testing clone v. move behaviors
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
