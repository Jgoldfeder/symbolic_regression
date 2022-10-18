import random, math
import time
import matplotlib.pyplot as plt
import numpy as np
import ast,copy
import sys
from sympy import *


A = [4,2,5,6,2,5,8,4,3,3,4]

class Test():
    def __init__(self,num):
        self.num = num
        pass

one = Test(1)
two = Test(2)
three = Test(3)
four = Test(4)

B = [one,two,three,four]

B.sort(key=lambda x:x.num,reverse=True)

for i in range(0,len(B)):
    print(B[i].num)

A.sort(reverse=False)
for i in range(0,len(A)):
    print(A[i])

'''
c=0
for i in range(0,10):
    c+=1
    print(c)
'''