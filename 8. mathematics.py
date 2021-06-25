# the break-even point
A,B,C = map(int, input().split())
if B >= C:
    print(-1)
else:
    n = A//(C-B) + 1
    print(n)
