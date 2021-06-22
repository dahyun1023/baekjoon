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
    N_list = []
    digit = len(str(a))
    for i in range(digit):
        a%(10*(i+1))
