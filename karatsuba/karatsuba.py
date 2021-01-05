import int_list
import unittest

x = 3141592653589793238462643383279502884197169399375105820974944592
y = 2718281828459045235360287471352662497757247093699959574966967627

def karatsuba(x, y):

    # they def need to be padded out to the same length or the Karatsuba formula
    # wom't work right?

    in_type = type(x)
    assert(in_type == type(y))

    xs, ys = int_list.tuple_listify((x,y))
    x_len, y_len = (len(xs), len(ys))

    # if either number has gone to zero don't waste time
    if xs[0] == 0 or ys[0] == 0:
        return [0]

    # when we get down to single digits, just multiply them
#    if x_len==1 and y_len==1:
#        val = x[0] * y[0] 
#        return val if in_type is int else int_list.listify(val) 

    # pad it to be big enough to be split into 1/4ths, each with even size
    longest_input_length = max(x_len, y_len)

    if longest_input_length <= 2:
        # we are nearing single digits; we don't want to keep popping back
        # up to 4-length lists forever.
        #padded_input_length = 2
        val = int_list.delistify(xs) * int_list.delistify(ys)
        return val if in_type is int else int_list.listify(val)

    else:
        mod4 = longest_input_length % 4
        padded_input_length = longest_input_length + (0 if mod4==0 else (4 - longest_input_length % 4))

    xp = [] # x, but zero-padded to /4'able length
    yp = [] # y, but zero-padded to /4'able length
    for n in range(padded_input_length, 0, -1):
        xp += [xs[-n]] if n <= x_len else [0]
        yp += [ys[-n]] if n <= y_len else [0] 

    L_dvby_2 = padded_input_length//2
    L_dvby_4 = padded_input_length//4

#    assert(L_dvby_4 * 2 == L_dvby_2 || padded_input_length == 2)
#    assert(L_dvby_2 * 2 == padded_input_length)

    a,b = (xp[:L_dvby_2], xp[L_dvby_2:])
    c,d = (yp[:L_dvby_2], yp[L_dvby_2:])

    s1 = karatsuba(a,c)
    s2 = karatsuba(b,d) 
    s1xs2 = karatsuba(int_list.add(a,b), int_list.add(c,d))
    s3 = int_list.sub(int_list.sub( s1xs2, s1 ), s2)    

    #
    # need to verify the lengths of these 0-appends
    # find some way of doing a nice proof in-commentary? Or maybe some
    # other direct way? Can we do something with asserts that would give us 
    # confidence that it's the right calculation? Yes; do the shift and
    # assert that it's the same as the mathematical calculation???
    #
    s1zeros = s1 + [0 for i in range(L_dvby_2)]
    assert(int_list.delistify(s1zeros) == int_list.delistify(s1) * 10**L_dvby_2)

# so this is getting fucked-up at this point when we're down to two-digit stuff....
# do we have to just do an conventional multiplication at that point? Or maybe just 
# say we do a normal multiplication when we get down to 2x2? What would it take to 
# make this think work all the way down to 1x1?
    ksum = int_list.add(int_list.add(s1zeros, s2), s3 + [0 for i in range(L_dvby_4)])

    # this function can process int-lists directly so recursive calls
    # don't have so much conversion overhead; whatever kind of input
    # this call got is the kind it should return
    return int_list.deslistify(ksum) if in_type is int else ksum 

# So I think the strategy right now is to walk thorugh a simple version,
# and verify it piece by piece with a by-hand work-through

tsum = karatsuba(2222,2)
tsum = karatsuba(12344,2)
assert(tsum>0)

class TestKaratsubaFunctions(unittest.TestCase):

    def test_karatsuba_functions(self):
        #
        # specific / edge cases
        #
        self.assertEqual(karatsuba(0,0), 0)
        self.assertEqual(karatsuba(1,1), 1)
        self.assertEqual(karatsuba(9,4), 36)
        self.assertEqual(karatsuba(13,3), 39) 
        self.assertEqual(karatsuba(3,13),39)
        self.assertEqual(karatsuba(12,12), 144)
        self.assertEqual(karatsuba(456,18), 8208)
        self.assertEqual(karatsuba(18,456), 8208)
        self.assertEqual(karatsuba(1234,5678),7006652)
        self.assertEqual(karatsuba(5678,1234),7006652)

        #
        # random multiplication checks 
        #
        random.seed()
        num_random_tests = 1000
        max_random_num_val = 99999999999999999999
       
        for i in range(num_random_tests):

            t1 = random.randrange(max_random_num_val)
            t2 = random.randrange(max_random_num_val)
            t1t2 = t1 * t2

            kval1 = karatsuba(t1,t2)
            kval2 = karatusba(t2,t1)

            self.assertEqual(kval, t1t2,
                '\nrandom test #' + str(i) + ': ' + str(t1) + " * " + str(t2) + " = " + str(t1t2) + 
                '\n(karatsuba returned: ' + str(kval)) 

            self.assertEqual(kval, t1t2,
                '\nrandom test #' + str(i) + ': ' + str(t1) + " * " + str(t2) + " = " + str(t1t2) + 
                '\n(karatsuba returned: ' + str(kval)) 


# **********************************************************************
#
# use 'python -m unittest karatsuba' to make sure none of these 
# functions are busted after you make any "improvements"
#
if __name__ == '__main__':
    unittest.main()
