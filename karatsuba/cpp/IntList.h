//
// IntList.h
//
// created by PKXH on 25 Apr 2021
//
// class declaration for integer list type
// (ex: an integer list for 0 is [0], 123 is [1,2,3] etc.).
//
#ifndef __int_list_h
#define __int_list_h

#include <vector>
#include <memory>

class IntList;

// list type definition
using int_list_sp = std::shared_ptr<IntList>;
// TODO: note this usage instead of the typedef
//typedef std::shared_ptr< IntList > int_list_sp;

// int list factory functions; yes, you only get a smart pointer to it

class IntList
//
// An iterable list of non-negative integer digits
//  
{
private:
    // list implementation type
    using int_list_t = std::vector<unsigned int>;

    // list implementation 
    int_list_t il;

    // constructors
    IntList( const IntList& il ); 
    IntList( const std::vector<unsigned int>& v );
    IntList( const std::string& s );
    IntList( unsigned int n ); 

    // utility functions
    void remove_leading_zeros( int_list_t& il ); 
    bool greater_than_or_equal_to(int_list_sp& a, int_list_sp& b);
    void delete_msd();

public:
    // standard usage operators
    unsigned int& operator[](int i);

    // class operators
    bool operator==(const IntList& il);     
    bool operator!=(const IntList& il);
    bool operator>=(const IntList& il);

    std::string str();

    // smart pointer only?
    //int_list_sp operator+(const int_list_sp& that);

    // TODO: add >= and + operators at least, and also tests for them.

    int size();

    unsigned int msd();
    unsigned int lsd();

    // available iterator types
    using iterator = int_list_t::iterator;
    using const_iterator = int_list_t::const_iterator;

    // iterator access
    iterator begin() { return il.begin(); }
    iterator end()   { return il.end  (); } 

    const_iterator begin() const { return il.begin(); }
    const_iterator end()   const { return il.end();   }

    const_iterator cbegin() const { return il.cbegin(); }
    const_iterator cend()   const { return il.cend();   }

    // factory functions; this object will only be available via shared smart pointers
    friend int_list_sp new_int_list_sp(const IntList& x);
    friend int_list_sp new_int_list_sp(const std::vector<unsigned int>& x);
    friend int_list_sp new_int_list_sp(const std::string& s);
    friend int_list_sp new_int_list_sp(unsigned int n);
};

//
// int list factory functions; yes, you only get a smart pointer to it
// TODO: so maybe use a namespace to limit the scope of these freestanding functions?
//
int_list_sp new_int_list_sp(const IntList& x);
int_list_sp new_int_list_sp(const std::vector<unsigned int>& x);
int_list_sp new_int_list_sp(const std::string& s);
int_list_sp new_int_list_sp(unsigned int n);

#endif // __int_list_h
