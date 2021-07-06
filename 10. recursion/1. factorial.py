N = int(input())

if N == 0:
    print(1)
else:
    a = 1
    while N > 1:
        a = a*N
        N -= 1
    print(a)
