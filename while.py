# A + B -5

while True:
    a,b = map(int, input().split())
    if a == 0 and b ==0:
        break
    else:
        print(a+b)

# A + B -4

while True:
    try:
        a,b = map(int, input().split())
        print(a+b)
    except:
        break
        
# summing cycle

N = int(input())
n = N
i = 0
while True:
    a = n//10
    b = n%10
    c = (a + b)%10
    n = b*10+c
    i += 1
    if n == N:
        break
        
print(i)