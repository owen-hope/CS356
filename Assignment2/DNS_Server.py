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
for index in range(len(myList)):
    for i in range(len(index)):
        if index[i] == '':
            del myList[index][i]
print("\n")
print("\n")
print(myList)
