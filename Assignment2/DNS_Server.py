from socket import *
import sys

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

myList = [i.split() for i in lines]
authoritative_domain = myList[0]
TTL = myList[1]

DNS_Log = myList[2:]


print("\n")
print("\n")
print(myList)
print("\n")
print("\n")
print(DNS_Log)
print("\n")

serverIP = ""
serverPort = 9007
dataLen = 1000000

# Create a UDP socket
serverSocket = socket(AF_INET, SOCK_DGRAM)
# Addign IP address and port number to socket
serverSocket.bind((serverIP, serverPort))

print("The server is ready to receve on port: " + str(serverPort))
print("\n")

while True:
    # Receive the information from dig
    data, address = serverSocket.recvfrom(dataLen)

    print("Data from client: " + struct.unpack('B', data))
