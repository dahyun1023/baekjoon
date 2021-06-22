# min/max

N   = int(input())
N_list = list(map(int,input().split()))

print(f'{min(N_list)} {max(N_list)}')

# max value
N_list = []
for _ in range(9):
    a = int(input())
    N_list.append(a)
    
max_value = max(N_list)
max_index = N_list.index(max_value)+1
print(f'{max_value}\n{max_index}')
      
 # How many numbers?
a = 1
for _ in range(3):
    a = a*int(input())

N_list = [0]*10
for _ in range(len(str(a))):
    b = a%10
    N_list[b] = N_list[b] + 1
    a = int(a/10)
    
for i in range(10):
    print(N_list[i])

      
# remainder
r_list = []
for _ in range(10):
    a = int(input())
    r_list = append(r_list,a%42)
r_list = set(r_list)
r_list = list(r_list) # print위한 list변형 필요?체크하자
print(len(r_list))
# average
N = int(input())
N_list = map(int, input().split())
avr = sum(N_list)/N
print(avr)
# OX quiz
N = int(input())
for i in range(N):
      score = 0
      bonus = 0
      result = input()
     
      for _ in range(len(result)):
        if result[i] == 'O':
            bonus += 1
            score = score + bonus
        else:
            bonus = 0
      print(score)
# Am I better than the average?
C = int(input())
for i in range(C):
      score_list = input().split()
      score_list = list(score_list)
      n = score_list[0]
      ave = sum(score_list[1:])
      good = 0
      for j in range(n):
          if score_list[j+1]>ave:
              good += 1
          else:
              pass
      a = good/n*100
      print(f'{a:.3f}')
