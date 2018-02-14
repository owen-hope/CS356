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
TTL = myList[1][0]
print(TTL)

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
qtype = 1
ipaddr = ""
ancount = 0
nscount = 0
rdata = ""

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
    i = 0
    while i < len(DNS_Log):
        print("here")
        print(DNS_Log[i])
        print(hostname)
        if DNS_Log[i][0] == hostname and DNS_Log[i][1] == "A":
            qtype = 1
            ipaddr += DNS_Log[i][2]
            print("trueeeeeeeeeeeeeeeee booiiiiiiiiii")
            ancount += 1
            break
        elif DNS_Log[i][0] == hostname and DNS_Log[i][1] == "CNAME":
            for x in DNS_Log:
                if x[0] == DNS_Log[i][2]:
                    cnamehost = x[0]
                    qtype = 5
                    cnameIP = x[2]
                    break
        else:
            rcode = 3
            i += 1

    #count the authority
    for x in range(len(DNS_Log)):
        if DNS_Log[x][1] == "NS":
            nscount += 1
            rdata += DNS_Log[x][2]

    # NEED TO ADD INFO FOR STUFF: QR AA RCODE
    the_message += struct.pack("!HHHHHH", ID, STUFF, QDCOUNT, ancount, nscount, ARCOUNT)
    #the_message += struct.pack("!H", ID)
    #struct.pack_into("!B", the_message, 2, 1)

    #Question Section
    #The packing and encoding for host name
    qnameResponse = hostname.split(".")
    for name in qnameResponse:
        size = len(name)
        the_message += struct.pack("!B", size)
        #the_message += struct.pack("!c", name[0].encode())
        print(the_message)
        for x in range(size):
            #test = name[0]
            #print(test)
            the_message += struct.pack("!c", name[x].encode())
    the_message += struct.pack("!B", 0)

    #Adding Qtype
    the_message += struct.pack("!H", qtype)

    #Adding Qclass
    the_message += struct.pack("!H", 1)

    #Answers Section
    #name
    for name in qnameResponse:
        size = len(name)
        the_message += struct.pack("!B", size)
        #the_message += struct.pack("!c", name[0].encode())
        print(the_message)
        for x in range(size):
            #test = name[0]
            #print(test)
            the_message += struct.pack("!c", name[x].encode())
    the_message += struct.pack("!B", 0)

    #type
    the_message += struct.pack("!H", qtype)

    #class
    the_message += struct.pack("!H", 1)

    #TTL
    the_message += struct.pack("!L", int(TTL))

    #RDLength
    the_message += struct.pack("!H", 4)

    #Rdata
    print(ipaddr)
    ipdata = ipaddr.split(".")
    print(ipdata)
    for i in range(len(ipdata)):
        ipdata[i] = int(ipdata[i])

    the_message += struct.pack("!BBBB", ipdata[0], ipdata[1], ipdata[2], ipdata[3])

    #Authority Section
    #Name
    for name in qnameResponse:
        size = len(name)
        the_message += struct.pack("!B", size)
        #the_message += struct.pack("!c", name[0].encode())
        print(the_message)
        for x in range(size):
            #test = name[0]
            #print(test)
            the_message += struct.pack("!c", name[x].encode())
    the_message += struct.pack("!B", 0)

    #type
    the_message += struct.pack("!H", qtype)

    #class
    the_message += struct.pack("!H", 1)

    #TTL
    the_message += struct.pack("!L", int(TTL))

    #RDLength calculating
    rdataList = rdata.split(".")
    len1 = len(rdataList[0])
    len2 = len(rdataList[1])
    len3 = len(rdataList[2])

    rdata = str(len1) + rdataList[0] + str(len2) + rdataList[1] + str(len3) + rdataList[2] + str(0)
    print(rdata)

    print(the_message)
    serverSocket.sendto(the_message, address)
