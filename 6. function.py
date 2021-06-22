# sum of N integers
def fun_sum (a):
    sum = 0
    for i in range(len(a)):
        sum += a[i]
    return sum
a = list(map(int, input()))
print(fun_sum(a))


# self number
def self_num(a):
    result = a
    digit = len(str(a))
    for i in range(digit):
        b = a%10
        result += b
        a = int(a/10)
    return result

for i in range(1,10*4+1):
    if self_num(i)==i:
        print(i)
    else:
        pass
    
# 한수
