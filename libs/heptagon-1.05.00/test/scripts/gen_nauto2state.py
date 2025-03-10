#!/usr/bin/python3

import sys

if len(sys.argv) >= 2:
    n = int(sys.argv[1])
else:
    print("Usage: " + sys.argv[0] + " <n>")
    exit(1)

print("node f(x0", end="")
for i in range(1,n):
    print(",x" + str(i),end="")
print(":int) = (y0", end="")
for i in range(1,n):
    print(",y" + str(i),end="")
print(":int)")

print("let")
for a in range(n):
    print("  automaton")
    for s in range(2):
        print("    state S" + str(s))
        print("      do")
        print("        y{0} = x{0} + {1};".format(a,a+s))
        print("      until x{0} = y{0} then S{1}".format((a+1)%n,1-s))
    print("  end;")
print("tel")
