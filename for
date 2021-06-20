# 1. googoodan

N = int(input())

for i in range(9):
    n = N*(i+1)
    print(f'{N} * {i+1} = {n}')

# 2. A + B - 3

T = int(input())

for i in range(T):
    A,B = input().split()

    A = int(A)
    B = int(B)
    print(A+B)
  
# 3. sum

n = int(input())
sum = 0

for i in range(1,n+1):
    sum += i
print(sum)

# 4. quick A+B

import sys

T = int(input())

for i in range(T):
    a,b = map(int, sys.stdin.readline().split())
    print(a+b)

# 5. print 1 to N

N = int(input())

for i in range(N):
    print(i+1)
  
# 6. print N to 1

N = int(input())

for i in range(0,N):
    print(N-i)
  
# 7. A+B-7

T = int(input())
for i in range(T):
    a,b = input().split()
    a = int(a)
    b = int(b)
    print("Case #%d: %d" %(i+1,a + b))

# 8. A+B-8

T = int(input())
for i in range(T):
    a,b = input().split()
    a = int(a)
    b = int(b)
    c = a + b
    print("Case #%d: %d + %d = %d" %(i+1,a,b,c))
  
# 9. star1

s = int(input())
for i in range(1,s+1):
    print('*'*i)

# 10. star2

N = int(input())
for i in range(1,N+1):  
    print(' '*(N-i)+'*'*i)
  
  
# 11. smaller number than X
N,X = map(int, input().split())
a = list(map(int, input().split()))

for i in range(len(a)):
    if a[i] < X:
        print(a[i], end=' ')

