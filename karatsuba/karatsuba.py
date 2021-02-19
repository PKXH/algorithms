import int_list
import unittest
import random


def karatsuba(x, y):

    in_type = type(x)
    assert in_type == type(y)

    xs, ys = int_list.tuple_listify((x,y))
    x_len, y_len = (len(xs), len(ys))

    # we interpret an empty list as a 0
    if x_len == 0 or y_len == 0:
        return [0]

    # if we're down to one digit, then multiply it out
    elif x_len == 1 or y_len == 1:
        # delistify both and leverage kno that one is len 1 to know which one controls the loop on the ripple sum loop
        val = int_list.delistify(xs) * int_list.delistify(ys)
        return val if in_type is int else int_list.listify(val)

    else:
        m  = -(-max(x_len, y_len)//2) # upside-down floor division = ceiling :)
        m2 = m*2

        a,b = (xs[:-m], xs[-m:])
        c,d = (ys[:-m], ys[-m:])

        s1 = karatsuba(a,c)
        s2 = karatsuba(b,d)
        s1xs2 = karatsuba(int_list.add(a,b), int_list.add(c,d))

        assert int_list.delistify(s1xs2) >= int_list.delistify(s1)
        assert int_list.delistify(s1xs2) - int_list.delistify(s1) >= int_list.delistify(s2), \
                print(str(int_list.delistify(s1xs2) - int_list.delistify(s1)) + ' < ' + str(int_list.delistify(s2)) + \
                '\n(x=' + str(x) + ', y=' + str(y) + ')')
        s3 = int_list.sub(int_list.sub( s1xs2, s1 ), s2)

        s1zs = s1 + [0 for i in range(m2)]
        assert int_list.delistify(s1zs) == int_list.delistify(s1) * 10**m2

        s3zs = s3 + [0 for i in range(m)]
        assert int_list.delistify(s3zs) == int_list.delistify(s3) * 10**m

        ksum = int_list.add(int_list.add(s1zs, s3zs), s2)

        # this function can process int-lists directly so recursive calls
        # don't have so much conversion overhead; whatever kind of input
        # this call got is the kind it should return
        return int_list.delistify(ksum) if in_type is int else ksum


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
        self.assertEqual(karatsuba(73134,81168),5936140512)

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
            kval2 = karatsuba(t2,t1)

            self.assertEqual(kval1, t1t2,
                '\nrandom test #' + str(i) + ': ' + str(t1) + " * " + str(t2) + " = " + str(t1t2) + 
                '\n(karatsuba returned: ' + str(kval1)) 

            self.assertEqual(kval2, t1t2,
                '\nrandom test #' + str(i) + ': ' + str(t1) + " * " + str(t2) + " = " + str(t1t2) + 
                '\n(karatsuba returned: ' + str(kval2)) 


# **********************************************************************
#
# use 'python -m unittest karatsuba' to make sure none of these 
# functions are busted after you make any "improvements"
#
if __name__ == '__main__':
    unittest.main()
