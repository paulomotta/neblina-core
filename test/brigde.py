#!/usr/bin/env python3.8
import sys
import time
from datetime import datetime

from neblina import *

float_ = 2

init_engine(0)

print("test_vec_add")

n = 3
vec_f = vector_new(n, float_)
vec_2 = vector_new(n, float_)
print("test_vec_add2")

for i in range(n):
    vector_set(vec_f, i,
               1.0, 0.0)
    vector_set(vec_2,
               i, 1.0, 0.0)
print("test_vec_add3")

#move_vector_device(vec_f)
#move_vector_device(vec_2)

res = vec_add(vec_f, vec_2)
print("test_vec_add4")

move_vector_host(res)
print("test_vec_add5|")

for i in range(n):
    print(vector_get(res, i))

print("test_vec_add6")

stop_engine()