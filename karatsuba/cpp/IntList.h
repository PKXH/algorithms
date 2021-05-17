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
class WriteCheck;

// list type definition
using int_list_sp = std::shared_ptr<IntList>;

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
    IntList( const IntList& il, bool trim_leading_zeros = true ); 
    IntList( const std::vector<unsigned int>& v, bool trim_leading_zeros = true );
    IntList( const std::string& s, bool trim_leading_zeros = true );
    IntList( unsigned int n ); 

    // utility functions
    bool greater_than_or_equal_to(int_list_sp& a, int_list_sp& b);
    void delete_msd();

public:
    // index operator; returns value-wrapper that blocks writebacks for
    // out-of-range selections
    WriteCheck operator[](unsigned int i);

    // class operators
    bool operator==(const IntList& il);     
    bool operator!=(const IntList& il);
    bool operator>=(const IntList& il);

    // user-friendly string representation
    std::string str();

    // smart pointer only?
    //int_list_sp operator+(const int_list_sp& that);

    int size();
    unsigned int msd();
    unsigned int lsd();

    // utility functions
    void remove_leading_zeros(); 

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
    friend int_list_sp new_int_list_sp(const IntList& x, bool trim_leading_zeros);
    friend int_list_sp new_int_list_sp(const std::vector<unsigned int>& x, bool trim_leading_zeros);
    friend int_list_sp new_int_list_sp(const std::string& s, bool trim_leading_zeros);
    friend int_list_sp new_int_list_sp(unsigned int n);
};

//
// class to wrap returned unsigned int reference from integer list's index overload.
// allows selected blocking of writebacks
//
class WriteCheck
{
public:
    unsigned int& checked_n;
    bool writeback_allowed;

public:
    // consructor
    WriteCheck(unsigned int& n, bool is_writeback_allowed) : checked_n(n), writeback_allowed(is_writeback_allowed) {}
   
    // allow read-access to wrapped data
    operator unsigned int const&() { return checked_n; }

    // called when user tries to modify the wrapped value. We expect that "writeback_allowed" will be
    // false if this wrapper was generated for a value that was index'd out-of-range (but it's ok for
    // the use to read it).
    WriteCheck& operator=(unsigned int const& rhs)
    {
        if (!writeback_allowed) 
            throw std::out_of_range("index is out of range");
        else {
            checked_n = rhs;
            return *this;
        }
    }
};

//
// int list factory functions; yes, you only get a smart pointer to it
// TODO: so maybe use a namespace to limit the scope of these freestanding functions?
//
int_list_sp new_int_list_sp(const IntList& x, bool trim_leading_zeros=true);
int_list_sp new_int_list_sp(const std::vector<unsigned int>& x, bool trim_leading_zeros=true);
int_list_sp new_int_list_sp(const std::string& s, bool trim_leading_zeros=true);
int_list_sp new_int_list_sp(unsigned int n);

#endif // __int_list_h
