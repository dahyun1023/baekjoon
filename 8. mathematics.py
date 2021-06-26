# the break-even point
A,B,C = map(int, input().split())
if B >= C:
    print(-1)
else:
    n = A//(C-B) + 1
    print(n)

    
# honey-bee house
N = int(input())

if N == 1:
    print(N)
else:  
    sum = 1
    i   = 1
    while True:
        a    = i*6
        sum += a
        if sum>=N:
            print(i+1)
            break
        i += 1
        
        
# numerator/denominator

N   = int(input())
i   = 2
tot = 2
while True:
    if N<i:
        j = i-N
        if tot%2 == 1:
            denominator = j
            numerator   = tot-j
        else:
            denominator = tot-j
            numerator   = j
        print(f'{numerator}/{denominator}')
        break
    else:
        i += tot
    tot += 1
    
    
# snail wants to go up
