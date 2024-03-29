//
// IntList.h
//
// created by PKXH on 29 Aug 2021
//
// class declaration for integer list type (using RAII patterns)
// (ex: an integer list for 0 is [0], 123 is [1,2,3], etc.)
//
#ifndef __int_list_h
#define __int_list_h

#include <vector>
#include <string>
#include <compare>
#include <ranges>

class IntList
//
// An iterable list of non-negative integer digits
//
{
public:
    // list implementation type
    using value_type = unsigned int;
    static const value_type lower_bound = 0;
    static const value_type upper_bound = 9;

private:
    // list implementation 
    using int_list_t = std::vector<value_type>;
    int_list_t il;

    // msd utility functions
    static inline const value_type& msd (const int_list_t& il ) { return il.front(); }
    static inline void delete_msd (int_list_t& il ) { il.erase(il.begin()); }

    // lsd utility functions
    static inline const value_type& lsd (const int_list_t& il ) { return il.back(); }
    static inline void delete_lsd (int_list_t& il ) { il.erase(il.end()-1); }

    static void throw_on_invalid_value_range( const value_type& v);
    static void throw_on_any_invalid_value_range( const int_list_t& il);
    static void throw_on_invalid_min_size( const int_list_t& il );

    void trim_leading_zeros( int_list_t& int_list ); 

public:
    // constructors
    IntList( std::initializer_list<value_type> il ); // init by initializer list
    IntList( std::vector<value_type>& vec );         // init by vector of digits 
    IntList( unsigned int ui );                      // init by unsigned int 

    // copy & move semantics / construction
    IntList( const IntList& ) = delete; // no copy constructor!
    IntList( IntList&& il);             // yes move constructor

    // manual initialization
    void push_back(value_type);

    // copy & move semantics / operators 
    IntList& operator=(const IntList& il) = delete; //no copy operator!
    IntList& operator=(IntList&& il); // move operator

    // in lieu of copy constructor
    IntList clone() const; // "clone" from an existing integer list

    // indexing operations
    value_type& operator[]( int i );
    const value_type& operator[]( int i) const;
    void throw_on_invalid_index( int i, const int_list_t& il ) const;

    unsigned long size() const { return il.size(); }

    // available iterator types
    using iterator = int_list_t::iterator;
    using const_iterator = int_list_t::const_iterator;
    using reverse_iterator = int_list_t::reverse_iterator;
    using const_reverse_iterator = int_list_t::const_reverse_iterator;

    // iterator access
    iterator begin() { return il.begin(); }
    iterator end()   { return il.end  (); } 

    const_iterator cbegin() const { return il.cbegin(); }
    const_iterator cend()   const { return il.cend();   }

    reverse_iterator rbegin() { return il.rbegin(); }
    reverse_iterator rend()   { return il.rend();   }

    const_reverse_iterator crbegin() const { return il.crbegin(); }
    const_reverse_iterator crend() const   { return il.crend();   }

    // generate string representation
    std::string to_str() const;

    // generate uint representation (if small enough!)
    unsigned int to_uint() const;

    // c++20 autocomparison generation
    auto operator<=>(const IntList& that) const
    {
        // If one is longer than the other, then that's the bigger
        // one...
        if ( this->size() < that.size() )
            return std::strong_ordering::less;
        else if ( this->size() > that.size() )
            return std::strong_ordering::greater;
    
        // ...and if they're the same length, then go ahead and use default
        // lexicographic comparison.
        else return this->il <=> that.il;
    }

    bool operator==(const IntList&) const = default; // have to explicitly state this since we have custom <=> 


#if defined(BUILD_UNIT_TESTS)
    // anything that monkeys with the representation should test this condition before returning
    static inline bool is_zero_trimmed( const IntList& il ) { return msd(il.il) !=0 || il.size()<=1; }  
#endif

#if defined(BUILD_UNIT_TESTS)
    // for testing clone v. move behaviors
    friend bool has_same_index_0_data_address_as_previous(IntList& il);

    // for testing initialization helpers and validators
    friend void run_msd_tests();
    friend void run_lsd_tests();
    friend void run_trim_leading_zeros_tests(); 
    friend void size_check_int_list_representation( std::initializer_list<IntList::value_type> il );
    friend void value_check_int_list_value( const value_type& val );
    friend void value_check_int_list( std::initializer_list<value_type> il );
#endif
};

IntList operator+(const IntList& a, const IntList& b);
IntList operator-(const IntList& a, const IntList& b);

#if defined(BUILD_UNIT_TESTS)
void set_previous_index_0_data_address(IntList& il);
#endif

#if defined(BUILD_UNIT_TESTS)
template<typename Container>
Container test_move_return(Container& c);
#endif

#endif // __int_list_h
