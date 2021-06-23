# sum of N integers
def solve (a):
    sum = 0
    for i in range(len(a)):
        sum += a[i]
    return sum

# self number
def self_num(a):
    result = a
    digit = len(str(a))
    for i in range(digit):
        b = a%10
        result += b
        a = int(a/10)
    return result

N_list = []
for i in range(1,10000+1):
    N_list.append(self_num(i))
N_list = list(set(N_list) )
a      = list(range(1,10000+1))

for i in range(0,100000):
    x = N_list[i]
    if x > 10000:
        break
    else:
        a.remove(x)

for i in range(len(a)):
    print(a[i])
# self number (better solution) 
# https://github.com/wook2124/Algorithm-Test/tree/master/%EB%B0%B1%EC%A4%80%20%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98/%EA%B5%AC%ED%98%84
natural = set(range(1,10000))
gen_num= set()

for i in range(1,1001):
    for j in str(i):
        i += int(j)
    gen_num.add(i)
self_num = sorted(natural - gen_num)

for i in self_num:
    print(i)
    
# 한수
N = int(input())

def han(N):
    a        = 0
    N_list   = []
    set_list = []
    
    for j in str(N):
        N_list.append(int(j))
    
    for i in range( len(N_list) - 1 ):
        set_list.append( N_list[i+1] - N_list[i] )
    
    if len(set(set_list)) == 1:
        a = 1
        
    return a

if N < 100:
    print(N)
else:
    a = 99 
    for i in range(100,N+1):
        a += han(i)
    print(a)
