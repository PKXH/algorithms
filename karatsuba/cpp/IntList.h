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
// TODO: note that you just need to declare these as friends to the class below
//int_list_sp new_int_list(IntList& x);
//int_list_sp new_int_list(std::vector<unsigned int>& v);
//int_list_sp new_int_list(std::string& s);

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
    IntList( IntList& il ); 
    IntList( std::vector<unsigned int>& v );
    IntList( std::string& s );

public:
    // standard usage operators
    int operator[](int i);
    bool operator==(const IntList& il);     
    bool operator!=(const IntList& il);

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
    friend int_list_sp new_int_list_sp(IntList& x);
    friend int_list_sp new_int_list_sp(std::vector<unsigned int> x);   // we'll need this if we initialize with an std::list
    friend int_list_sp new_int_list_sp(std::vector<unsigned int>& x);
    friend int_list_sp new_int_list_sp(std::string& s);
};

#endif // __int_list_h
