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
#    xs = [0]+xs if len(xs)%2>0 else xs
#    ys = [0]+ys if len(ys)%2>0 else ys

    x_len, y_len = (len(xs), len(ys))
    longest_input_length = max(x_len, y_len)
    padded_input_length = 
        longest_input_length + (0 if longest_input_length % 2 == 0 else 1)
 
    xp = [] # x, but zero-padded to even length
    yp = [] # y, but zero-padded to even length
    for n in range(padded_input_length, 0, -1):
        xp += x[-n] if n <= x_len else 0
        yp += y[-n] if n <= y_len else 0 

    i,j = (x_len//2, y_len//2)
    a,b = (xs[:i], xs[i:])
    c,d = (ys[:j], ys[j:])

    s1 = karatsuba(a,c) # + [0 for i in range(len(xs))]  
    s2 = karatsuba(b,d) 
    s1xs2 = karatsuba(int_list.add(a,b), int_list.add(c,d))
    s3 = int_list.sub( int_list.sub( s1xs2 ), s1 ), s2)    

    #
    # need to verify the lengths of these 0-appends
    # find some way of doing a nice proof in-commentary? Or maybe some
    # other direct way? Can we do something with asserts that would give us 
    # confidence that it's the right calculation? Yes; do the shift and
    # assert that it's the same as the mathematical calculation???
    #
    s1zeros = s1 + [0 for k in range(i))]
    assert(s1zeros == s1zeros * 10**i)

    ksum = int_list_add([s1 + [0 for i in range(len(xs))], s2, s3 + [0 for i in range(len(xs)/2)]) 

    return int_list.deslistify(ksum) if in_type == 'int' else ksum 



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
                '\nrandom test #' + str(i) + ': ' + str(t1) + " * " + str(t2) + " = " str(t1t2) + 
                '\n(karatsuba returned: ' + str(kval) 

            self.assertEqual(kval, t1t2,
                '\nrandom test #' + str(i) + ': ' + str(t1) + " * " + str(t2) + " = " str(t1t2) + 
                '\n(karatsuba returned: ' + str(kval) 


# **********************************************************************
#
# use 'python -m unittest karatsuba' to make sure none of these 
# functions are busted after you make any "improvements"
#
if __name__ == '__main__':
    unittest.main()
