#
# int_list.py
# created by PKXH on 2 Jan 2021
#
# functions for creating and manipulating integer lists
# (ex: an integer list for 0 is [0], 123 is [1,2,3], etc.).
#

import unittest
import random

# ********************************************************************** 
def listify(x):
'''
Convert an arbitrary-lengthed integer argument into a list of value 0-9 
integer digits, or simply return the input untouched if it is already a 
list.
'''
    assert type(x) is int or (type(x) is list)

    if type(x) is int:
        xs=[]
        while x>0 or len(xs)<1:
            xs.append(x%10)
            x//=10
        x = xs[::-1]
    else:
        if len(x)>0:
            # make sure int-lists don't have leading zeros
            while x[0]==0 and len(x)>1:
                x = x[1:]
    return x

def delistify(x):
    '''
    Convert the list of value 0-9 integer digits into the integer obtained 
    by concatenating the list's digits.
    '''
    assert type(x) is list and len(x)>0 and all([(type(i) is int and i<=9) for i in x])

    return int(''.join([str(d) for d in x]))

class TestListifyFunctions(unittest.TestCase):
    '''
    Some basic unit testing to make sure our de/listify functions are 
    working as expected; these should be run whenever there are changes
    to any of the functions tested below.
    '''
    def test_listify(self):
        '''
        test integer -> int_list conversion. 
        '''
        self.assertEqual(listify([]), [])
        self.assertEqual(listify([1]), [1])
        self.assertEqual(listify([1,2,3]), [1,2,3])
        self.assertEqual(listify(0), [0])
        self.assertEqual(listify(123), [1,2,3])

    def test_delistify(self):
        '''
        test int_list -> integer conversion.
        '''
        self.assertEqual(delistify([0]), 0)
        self.assertEqual(delistify([0,0,0]), 0)
        self.assertEqual(delistify([1,2,3]), 123)
        self.assertEqual(delistify([0,1,2,3]), 123)
        self.assertEqual(delistify([0,0,3,2,1]), 321)

    def test_inverse_behavior(self):
        '''
        test roundtrip integer -> int_ist -> integer integrity using a lot
        of randomly generated integers. 
        '''
        random.seed()
        num_random_tests = 1000
        max_random_num_val = 999999999999

        for i in range(num_random_tests):
            tval = random.randrange(max_random_num_val) 
            dval = delistify(listify(tval))
            self.assertEqual(dval, tval,
                    '\nrandom test #' + str(i) + ': ' + str(tval) + ' went in, ' + str(dval) + ' came out :(')


# ********************************************************************** 
def tuple_listify(ts):
    '''
    convert integer tuples into int-list tuples.
    '''
    return tuple(listify(t) for t in ts)

class TestTupleListifyFunctions(unittest.TestCase):
    '''
    Some basic unit testing to make sure our tuple listify functions are 
    working as expected; these should be run whenever there are changes
    to any of the functions tested below.
    '''
    def test_tuple_listify(self):
        '''
        Test the tuple listify functionality using some hand-selected
        tuples to be listified.
        '''
        self.assertEqual(tuple_listify(([],[])), ([],[]))
        self.assertEqual(tuple_listify(([0],[0])), ([0],[0]))
        self.assertEqual(tuple_listify((0,0)), ([0],[0]))
        self.assertEqual(tuple_listify((123,456)), ([1,2,3],[4,5,6]))


# ********************************************************************** 
def vector_listify(vs):
    '''
    convert integer vectors into int-list vectors.
    '''
    return [listify(v) for v in vs]

class TestVectorListifyFunctions(unittest.TestCase):
    '''
    Some basic unit testing to make sure our vector listify functions are 
    working as expected; these should be run whenever there are changes
    to any of the functions tested below.
    '''
    def test_vector_listify(self):
        '''
        Test the vector listify functionality using some hand-selected
        vectors to be listified.
        '''
        self.assertEqual(vector_listify([[],[]]), [[],[]])
        self.assertEqual(vector_listify([[0],[0]]), [[0],[0]])
        self.assertEqual(vector_listify([0,0]), [[0],[0]])
        self.assertEqual(vector_listify([123,456]), [[1,2,3],[4,5,6]])


# ********************************************************************** 
def greater_than_or_equal_to(a, b):
    '''
    Return a>=b boolean value if a and b are arbitrary-lengthed integer
    lists (ex: the integer 123 is rerresented by the integer digit list 
    [1,2,3]).
    '''
    assert type(a) is list and all([(type(i) is int and i<=9) for i in a])
    assert type(b) is list and all([(type(i) is int and i<=9) for i in b])

    len_a, len_b = (len(a), len(b))

    # check length: longer (non-leading zero'd) wins.
    # if same length, check first digit: highest wins.
    # same? recursively call gte on a[1:] and b[1:]

    if len_a > len_b:
        if a[0]==0:
            return greater_than_or_equal_to(a[1:],b)
        else:
            return True

    elif len_a < len_b: 
        if b[0]==0:
            return greater_than_or_equal_to(a,b[1:])
        else:
            return False
        
    else:
        if len_a==0:
            return True
        elif a[0] != b[0]:
            return a[0] >= b[0]
        else:
            return greater_than_or_equal_to(a[1:], b[1:])


class TestGreaterThanOrEqualToFunctions(unittest.TestCase):
    '''
    Some basic unit testing to make sure our greater-than-or-equal-to 
    functions are working as expected; these should be run whenever there 
    are changes to any of the functions tested below.
    '''
    def test_greater_than(self):
        '''
        Run several greater-than-or-equal-to tests. 
        '''
        #
        # specific/edge cases
        #
        self.assertTrue(greater_than_or_equal_to([1],[0]))
        self.assertTrue(greater_than_or_equal_to([1,2,3],[0]))
        self.assertTrue(greater_than_or_equal_to([1,2,3],[0,0,0,0,0]))
        self.assertTrue(greater_than_or_equal_to([0,1,2,3],[0,0,0,0,0]))
        self.assertTrue(greater_than_or_equal_to([0,1,2,3],[0]))
        self.assertTrue(greater_than_or_equal_to([0,1,2,3],[0,0,0,0]))
        self.assertTrue(greater_than_or_equal_to([0,0,0,1],[0,0,0,0]))
        self.assertTrue(greater_than_or_equal_to([0,0,0,1],[0]))

        #
        # double-checking random values with python math
        #
        random.seed()
        num_random_tests = 1000
        max_random_num_val = 99999999999999999999

        for i in range(num_random_tests):
            a = random.randrange(max_random_num_val)
            b = random.randrange(max_random_num_val)
            if a>b:
                self.assertTrue(greater_than_or_equal_to(listify(a),listify(b)),
                    'random test #' + str(i) + ': ' + str(a) + ' >= ' + str(b) + ' was expecting TRUE')
            else:
                self.assertTrue(greater_than_or_equal_to(listify(b),listify(a)),
                    'random test #' + str(i) + ': ' + str(b) + ' >= ' + str(a) + ' was expecting TRUE')

    def test_less_than(self):
        '''
        Run several less-than-or-equal-to tests.
        '''
        #
        # specific/edge cases
        #
        self.assertFalse(greater_than_or_equal_to([0],[1]))
        self.assertFalse(greater_than_or_equal_to([0],[1,2,3]))
        self.assertFalse(greater_than_or_equal_to([0,0,0,0,0],[1,2,3]))
        self.assertFalse(greater_than_or_equal_to([0,0,0,0,0],[0,1,2,3]))
        self.assertFalse(greater_than_or_equal_to([0],[0,1,2,3]))
        self.assertFalse(greater_than_or_equal_to([0,0,0,0],[0,1,2,3]))
        self.assertFalse(greater_than_or_equal_to([0,0,0,0],[0,0,0,1]))
        self.assertFalse(greater_than_or_equal_to([0],[0,0,0,1]))

        #
        # double-checking random values with python math
        #
        random.seed()
        num_random_tests = 1000
        max_random_num_val = 99999999999999999999

        for i in range(num_random_tests):
            a = random.randrange(max_random_num_val)
            b = random.randrange(max_random_num_val)
            if a<b:
                self.assertFalse(greater_than_or_equal_to(listify(a),listify(b)),
                    'random test #' + str(i) + ': ' + str(a) + ' >= ' + str(b) + ' was expecting FALSE')

            else:
                self.assertFalse(greater_than_or_equal_to(listify(b),listify(a)),
                    'random test #' + str(i) + ': ' + str(b) + ' >= ' + str(a) + ' was expecting FALSE')
            
    def test_equal_to(self):
        '''
        Run several equal-to tests.
        '''
        #
        # specific/edge cases
        #
        self.assertTrue(greater_than_or_equal_to([0],[0]))
        self.assertTrue(greater_than_or_equal_to([0,0,0,0],[0]))
        self.assertTrue(greater_than_or_equal_to([0],[0,0,0,0]))
        self.assertTrue(greater_than_or_equal_to([1,2,3],[1,2,3]))
        self.assertTrue(greater_than_or_equal_to([0,1,2,3],[1,2,3]))
        self.assertTrue(greater_than_or_equal_to([1,2,3],[0,1,2,3]))
        self.assertTrue(greater_than_or_equal_to([0,0,0,1,2,3],[1,2,3]))
        self.assertTrue(greater_than_or_equal_to([1,2,3],[0,0,0,1,2,3]))

        #
        # double-checking random values with python math
        #
        random.seed()
        num_random_tests = 1000
        max_random_num_val = 99999999999999999999

        for i in range(num_random_tests):
            a = random.randrange(max_random_num_val)
            b = a
            self.assertTrue(greater_than_or_equal_to(listify(a),listify(b)),
                'random test #' + str(i) + ': ' + str(a) + ' >= ' + str(b) + ' was expecting TRUE')


# ********************************************************************** 
def add(a, b):
    '''
    This function returns the sum (in integer list form) of two non-negative 
    integer lists.
    '''
    assert type(a) is list and all([(type(i) is int and i<=9) for i in a])
    assert type(b) is list and all([(type(i) is int and i<=9) for i in b])

    # determine lengths
    len_a = len(a)
    len_b = len(b)
    max_l = max(len_a,len_b)

    # make input int lists the same length, output length+1
    a = [0 for i in range(max_l-len_a)] + a
    b = [0 for i in range(max_l-len_b)] + b
    c = [0 for i in range(max_l+1)]

    # starting from lsd, add and carry up to msd
    for i in range(1, max_l+1):
        dgt_sum = a[-i] + b[-i] + c[-i]
        dgt_div = dgt_sum // 10
        dgt_mod = dgt_sum  % 10
        c[-i]   = dgt_mod
        c[-i-1] = dgt_div

    # chop off leading zeros
    while c[0]==0 and len(c)>1:
        c = c[1:]

    return c

class TestAddFunctions(unittest.TestCase):
    '''
    Some basic unit testing to make sure our add functions are 
    working as expected; these should be run whenever there 
    are changes to any of the functions tested below.
    '''
    def test_add(self):
        '''
        Run several add tests.
        '''
        #
        # specific/edge cases
        #
        self.assertEqual(add([0],[0]),[0])
        self.assertEqual(add([0,0,0,0],[0]),[0])
        self.assertEqual(add([0],[0,0,0,0]),[0])
        self.assertEqual(add([1,2,3],[4,5,6]),[5,7,9])
        self.assertEqual(add([0,0,1,2,3],[4,5,6]),[5,7,9])
        self.assertEqual(add([1,2,3],[0,0,4,5,6]),[5,7,9])
        self.assertEqual(add([9,9,9,9],[1]),[1,0,0,0,0])

        #
        # double-checking random values with python math
        #
        random.seed()
        num_random_tests = 1000
        max_random_num_val = 99999999999999999999

        for i in range(num_random_tests):
            a = random.randrange(max_random_num_val)
            b = random.randrange(max_random_num_val)
            c = a + b
            d = delistify(add(listify(a),listify(b)))
            self.assertEqual(c,d,
                '\nrandom test #' + str(i) + ': ' + str(a) + ' + ' + str(b) + ' = ' + str(c) + 
                '\n(but we got ' + str(d) + ')')


# **********************************************************************
def sub(a, b):
    '''
    This function returns the difference (in integer list form) of two
    non-negative integer lists, with the requirement that a>=b.
    '''
    #
    # initialize list to store our difference and cache input list 
    # lengths since we'll be using them several times
    # 
    diff = []
    a_len = len(a)
    b_len = len(b)
    max_len = max(a_len, b_len) 
    min_len = min(a_len, b_len)

    #
    # 0-pad out inputs so they're the same length and make sure that the 
    # value we're subtracting from (a) is greater than or equal to what 
    # we're subtracting (b)
    #
    ap = [(0 if max_len-i > a_len else a[a_len-max_len+i]) for i in range(max_len)]
    bp = [(0 if max_len-i > b_len else b[b_len-max_len+i]) for i in range(max_len)]

    #
    # same length & ap>=bp simplifies borrow logic, so make sure that's
    # what we got
    #
    assert len(ap) == len(bp)
    assert len(ap) == max_len
    if __debug__:
        # 0-padded longest input complicates our paranoia 
        if len(ap)>0 and len(bp)>0:
            # enumerate indices of non-zero values in ap & bp
            ap_nzis = [i for i,v in enumerate(ap) if v>0]
            bp_nzis = [i for i,v in enumerate(bp) if v>0]
            if len(ap_nzis)>0 and len(bp_nzis)>0:
                # the significant-est non-0 msd is in ap or bust
                msd = min(ap_nzis[0], bp_nzis[0])
                assert ap[msd] >= bp[msd], "assertion failed: ap[msd] >= bp[msd]\n" + \
                        str(ap[msd]) + ' < ' + str(bp[msd]) + ' (a=' + str(a) + ', b=' + str(b) + ')' 

    #
    # Walk through the two lists and subtract, digit by digit, starting
    # from the least-siginificant running to the most; ripple borrow 
    # from more significant digits in the subtractee when necessary &
    # keep track of borrows by modifying the intentionally-copied (so 
    # we don't taint the caller's stuff by reference) subtractee list 
    #
    for i in range(max_len-1, -1, -1):
        d = ap[i]-bp[i]
        if d < 0:
            ci = 1
            while ap[i-ci] == 0:
                ap[i-ci] = 9
                ci+=1
            ap[i-ci] -= 1
            d += 10
        diff = [d] + diff 

    # chop off leading zeros
    while diff[0]==0 and len(diff)>1:
        diff = diff[1:]

    return diff

class TestSubFunctions(unittest.TestCase):
    '''
    Some basic unit testing to make sure our sub functions are 
    working as expected; these should be run whenever there 
    are changes to any of the functions tested below.
    '''
    def test_sub(self):
        '''
        Run several add tests.
        '''
        #
        # specific/edge cases
        #
        self.assertEqual(sub([0],[0]),[0])
        self.assertEqual(sub([0,0,0,0],[0]),[0])
        self.assertEqual(sub([0],[0,0,0,0]),[0])
        self.assertEqual(sub([4,5,6],[1,2,3]),[3,3,3])
        self.assertEqual(sub([4,5,6],[0,0,1,2,3]),[3,3,3])
        self.assertEqual(sub([0,0,4,5,6],[1,2,3]),[3,3,3])
        self.assertEqual(sub([1,0,0,0,0],[1]),[9,9,9,9])

        #
        # double-checking random values with python math
        #
        random.seed()
        num_random_tests = 1000
        max_random_num_val = 99999999999999999999

        for i in range(num_random_tests):
            a = random.randrange(max_random_num_val)
            b = random.randrange(max_random_num_val)
            bigger, smaller = (a,b) if a>b else (b,a)
            c = bigger - smaller
            d = delistify(sub(listify(bigger),listify(smaller)))
            self.assertEqual(c,d,
                '\nrandom test #' + str(i) + ': ' + str(bigger) + ' - ' + str(smaller) + ' = ' + str(c) + 
                '\n(but we got ' + str(d) + ')')


# **********************************************************************
#
# use 'python -m unittest int_list' to make sure none of these 
# functions are busted after you make any "improvements"
#
if __name__ == '__main__':
    unittest.main()
