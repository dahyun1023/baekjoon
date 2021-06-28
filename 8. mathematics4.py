# Fly me to the alpha centauri
T   = int(input())
for _ in range(T):
    x,y = map(int, input().split())

    diff  = y - x
    count = 0
    num   = 1
    while True:
        if diff <= num:
            count += 1
            print(count)
            break
        elif diff <= 2*num:
            count += 2
            print(count)
            break
        else:
            diff  -= 2*num
            count += 2        
            num   += 1
