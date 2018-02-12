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

myList = [i.split() for i in lines]
authoritative_domain = myList[0]
TTL = myList[1]

info = myList[2:]
print("\n")
print("\n")
print(myList)
print("\n")
print(info)
