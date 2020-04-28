#!/usr/bin/python2
import random


def generateCode(number_list, length):
    code = ''
    for i in range(500):
        for j in range(length):
            code += random.choice(number_list)
    print code[:2000]

a = ['0', '1']
b = ['4', '5', '6']
c = ['5', '6', '7', '8', '9']
d = ['1', '2', '3', '4']
e = ['0', '1', '2', '5', '8', '9']


generateCode(a, 7)
generateCode(b, 6)
generateCode(c, 4)
generateCode(d, 5)
generateCode(e, 4)
generateCode(a, 11)
