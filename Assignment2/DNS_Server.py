from socket import *

lines = []
with open("dns-master.txt", "r") as file:
    for line in file:
        if not line.strip():
            continue
        elif line.startswith("#"):
            continue
        else:
            line = line.strip()
            lines.append(line)

print(lines)

myList = [i.split('\t') for i in lines]
for i in range(len(myList)):
    for j in range(len(myList[i])):
        if myList[i][j] == '':
            del myList[i][j]
print("\n")
print("\n")
print(myList[2][3])
