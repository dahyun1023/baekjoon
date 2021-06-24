# ascii code

a = input()
print(ord(a))

# sum

N = int(input())
num = int(input())
sum = 0

for i in str(num):
    sum += int(i)
print(sum)


# Finding an alphabet
word        = input()
N_list      = []
ascii_list  = list(range(97,123)) # 97 to 122
result_list = [-1]*26
count       = 0

for i in word:
    N_list.append(i)

l = len(N_list) - 1
for i in N_list.reverse():
    j = ascii_list.find(ord(i))
    result_list[j] = l
    l -= 1

# repeatitive string
T = int(input())
N_list = []
for _ in range(T):
    R,S = input().split()
    R = int(R)
    for i in word:
        N_list.append(i*3)
    print(N_list[:])

# study of words
small_ascii= list(range(97,123))
big_ascii  = list(range(65,91))
result_list = [0]*26*2 # small, bit order
word = input()
for i in word:
  
