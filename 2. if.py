# 1. if

a,b = input().split()
a = int(a)
b = int(b)
if a>b:
    print(">")
elif a<b:
    print("<")
else:
    print("==")
    
# 2. exam score

score = int(input())

if score >= 90:
    print("A")
elif score >= 80:
    print("B")
elif score >= 70:
    print("C")
elif score >= 60:
    print("D")
else:
    print("F")
    
# 3. leap year
year = int(input()) 

if (year%4 == 0) and (year%100 != 0):
    print("1")
elif year%400 == 0:
    print("1")
else:
    print("0")
    
# 4. quadrant

x = int(input())
y = int(input())

if x>0 and y>0:
    print("1")
elif x<0 and y>0:
    print("2")
elif x<0 and y<0:
    print("3")
else:
    print("4")
    
# 5. alarm clock

H, M = input().split()

time = 60*int(H) + int(M) - 45 # change the time unit into minute
if time <0:
    time += 60*24
H    = time//60
M    = time%60

print(H,M)
