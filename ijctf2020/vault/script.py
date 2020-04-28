#!/usr/bin/python2
import random


num_list = []
num_list.append(['0', '1'])
num_list.append(['4', '5', '6'])
num_list.append(['5', '6', '7', '8', '9'])
num_list.append(['1', '2', '3', '4'])
num_list.append(['0', '1', '2', '5', '8', '9'])
num_list.append(['0', '1'])


for l in num_list:
    code = ''
    for i in range(2000):
            code += random.choice(l)
    print code
