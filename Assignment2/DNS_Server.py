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
QUESTIONOFFSET = 12
the_message = bytes([])

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

    hostname = ""
    count = 1
    test = True
    while test:
        firstByte = struct.unpack_from("!B", data, QUESTIONOFFSET)
        print(firstByte[0])
        QUESTIONOFFSET += 1

        if firstByte[0] == 0:
            print("equal 0")
            break
        else:
            for i in range(firstByte[0]):
                hostname += struct.unpack_from("!c", data, QUESTIONOFFSET)[0].decode()
                print(hostname)

                QUESTIONOFFSET += 1

            hostname += "."
    #QTYPE = struct.unpack_from("!B", data, QUESTIONOFFSET)
    hostname = hostname[:-1]
    print("out of while loop")
    print(hostname)
    for i in DNS_Log:
        print("here")
        print(i[0])
        print(hostname)
        if i[0] == hostname and i[1] == "A":
            qtype = 1
            ipaddr = i[2]
        elif i[0] == hostname and i[1] == "CNAME":
            for x in DNS_Log:
                if x[0] == i[2]:
                    cnamehost = x[0]
                    cnameQtype = x[1]
                    cnameIP = x[2]
        else:
            rcode = 3

            print("trueeeeeeeeeeeeeeeee booiiiiiiiiii")
            break
    #the_message += struct.pack("!H", ID)
    #the_message += struct.pack("!H", STUFF)

    # NEED TO ADD INFO FOR STUFF: QR AA RCODE
    the_message += struct.pack("!HHHHHH", ID, STUFF, QDCOUNT, ANCOUNT, NSCOUNT, ARCOUNT)
    qnameResponse = hostname.split(".")
    for name in qnameResponse:
        size = len(qnameResponse[name])
        the_message += struct.pack("!B", size)
        for x in size:
            the_message += struct.pack("!c", size)
    print(the_message)
    serverSocket.sendto(the_message, address)
