#!/usr/bin/python3

import sys

if len(sys.argv) >= 2:
    n = int(sys.argv[1])
else:
    print("Usage: " + sys.argv[0] + " <n>")
    exit(1)

print("node f(x0,x1:int) = (last y0 : int = 0; last y1 : int = 2)")
# print("node f(x1", end="")
# for i in range(2,n+1):
#     print(",x" + str(i),end="")
# print(":int) = (y1", end="")
# for i in range(2,n+1):
#     print(",y" + str(i),end="")
# print(":int)")

print("let")
print("  automaton")
for s in range(n):
    print("    state S" + str(s))
    print("      do")
    print("        y{0} = x{0} + {1};".format(s%2,s))
    print("      until x1 = y1 then S" + str((s+1) % n))
print("  end")
print("tel")
