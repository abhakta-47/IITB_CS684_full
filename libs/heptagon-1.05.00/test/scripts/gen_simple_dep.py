#!/usr/bin/python3

import sys

if len(sys.argv) >= 2:
    n = int(sys.argv[1])
else:
    print("Usage: " + sys.argv[0] + " <n>")
    exit(1)

print("node f(x1", end="")
for i in range(2,n+1):
    print(",x" + str(i),end="")
print(":int) = (y1", end="")
for i in range(2,n+1):
    print(",y" + str(i),end="")
print(":int)")

print("let")
for i in range(1,n):
    print("  y{0} = x{0} + y{1};".format(i,i+1))
print("  y{0} = x{0};".format(n))
print("tel")
