from socket import *
import sys
import struct

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
    print(data)
    #, STUFF, QDCOUNT, ANCOUNT, NSCOUNT, ARCOUNT
    ID, STUFF, QDCOUNT, ANCOUNT, NSCOUNT, ARCOUNT = struct.unpack_from("!HHHHHH", data)
    #print("Data from client: " + ID)
    print(ID)
    print(STUFF)
    print(QDCOUNT)
    print(ANCOUNT)
    print(NSCOUNT)
    print(ARCOUNT)
    print("\nThis is the start of the question section")
    questionOffset = 12
    hostname = ""

    while True:
        firstByte = struct.unpack_from("!B", data, questionOffset)
        print(firstByte[0])
        questionOffset += 1

        if firstByte[0] == 0:
            break

        for i in range(firstByte[0]):
            hostletter = struct.unpack_from("!c", data, questionOffset)
            print(hostletter)
            questionOffset += 1
            #hostname += hostletter[0]
    #print(hostname)
