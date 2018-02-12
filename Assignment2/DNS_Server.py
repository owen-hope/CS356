from socket import *

lines = []
with open("dns-master.txt", "r") as file:
    for line in file:
        line = line.strip()
        lines.append(line)

print(lines)
