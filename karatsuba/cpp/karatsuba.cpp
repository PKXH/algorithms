#include <iostream>
#include <list>

// *** UNDER CONSTRUCTION *** UNDER CONSTRUCTION *** UNDER CONSTRUCTION ***

// using namespace std;

// Karatsuba stuff:
//
// Create a number list object, that can be initialized with a numeric string, maybe? Should it be a number?
// I think we can only accept ints up to a certain size, so I think it's going to have to be a string input.
//
// We're also going to have to find a good unit test package for c++.

// TODO: we're going to want to make sure that we trap all possible failures and convert them to 
// int return codes, or at least explore this possibility and its implications?

int main() {

    int int_array[] = {0,1,2,3,4};
    std::list<int> test_list(4,100); 

    std::cout << "test list: ";
    for (std::list<int>::iterator it=test_list.begin(); it!=test_list.end(); it++) {
        std::cout << *it << ' ';
    }
    std::cout << "\n";

    return 0;
}

#include <iostream>
//#include "IntList.h"

//LargeInt karatsub(unsigned long int x, unsigned long int y) {
//
//    return karatsuba(LargeInt(x), LargeInt(y)); 
//}

std::list<unsigned int>& karatsuba(std::list<unsigned int> &x,std::list<unsigned int> &y) {
    //
    // Calculate a product using Karatsuba multiplication.
    //
    // x,y are arbitrarily large integers to be multiplied.
    //
    unsigned int x_len = x.size(), y_len = y.size();

    if ((x_len == 0) || (y_len == 0)) {
        unsigned int val = x.front() * y.front();
        //std::list<unsigned int> retval = { val };
        return x; 
    }
    else { 
        return y;
    }
}

// So thinking about putting this thing in its own namespace / source file, and adding some handy 
// factor objects that can take lists or tuples as constructor arguments

// So still thinking about how we should handle this structure: smart pointers? Or pass in a reference
// to a dedicated return value? Or repurpose one of the inputs? And say that what you pass to the 
// method will be overwritten?
// 
// Mostly I'm wondering if a share_ptr can handle this creation inside of a scope, and then survive being
// returned by reference and used in the enclosing scope...
