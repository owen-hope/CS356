#Owen Hope
#oeh3
#Section 002

from socket import *
from datetime import datetime
import time
import calendar

import os.path

serverIP = ""
serverPort = 9007
dataLen = 1000000
dataFromClientArray = []
dataFromClient = ''
ifModifiedSince = ''
seconds = 0
count = 1
notMod = True


contentType = 'Content-Type: text/html; charset=UTF-8\r\n'

# Create a TCP "welcoming" socket.
serverSocket = socket(AF_INET, SOCK_STREAM)

# Assign IP address and port number to socket
serverSocket.bind((serverIP, serverPort))

# Listen for incoming connection requests
serverSocket.listen(4)
# Accept incoming connection requests, and allocate a new socket for data communication
connectionSocket, address = serverSocket.accept()
print("Socket created for client " + address[0] + ", " + str(address[1]))

#serverSocket.settimeout(3)

print("The server is ready to receive on port: " + str(serverPort))

# Loop forever listening for incoming connection requests on "welcoming" socket
while True:

    # Receive and print the client data in bytes from "data" socket
    data = connectionSocket.recv(dataLen)
    print("Data from client: " + data.decode())
    dataFromClient += data.decode()
    dataFromClientArray = dataFromClient.split('\n')


    for value in dataFromClientArray:
        if value[:17] == 'If-Modified-Since':
            ifModifiedSince = value[19:] + '\n'
            #print(ifModifiedSince)
            time = time.strptime(ifModifiedSince, '%a, %d %b %Y %H:%M:%S %Z\r\n')
            seconds += calendar.timegm(time)

    try:
        file = data.split()[1]
        f = open(file[1:])
        fileData = f.read()
        f.close()
        lenFile = str(len(fileData)) + '\r\n'
        # Determine file's modification time
        secs = os.path.getmtime(file[1:])

    except IOError:
        # Send response for file not found
        fileNotFoundMessage = 'HTTP/1.1 404 Not Found\r\n\r\n'
        connectionSocket.send(fileNotFoundMessage.encode())
        connectionSocket.close()


    dateTime = datetime.utcnow()
    date = dateTime.strftime("%a, %d %b %Y %H:%M:%S %Z\r\n")



    if int(secs) != seconds:
        notMod = True
    else:
        notMod = False



    if notMod:
        # Convert above time to UTC/GMT (Gives a tuple)

        t = time.gmtime(secs)

        # Convert above time tuple to a string in HTTP format
        last_mod_time = time.strftime("%a, %d %b %Y %H:%M:%S %Z\r\n", t)

    notModMessage = 'HTTP/1.1 304 Not Modified\r\n' + 'Date: ' + date + '\r\n'
    okayMessage = 'HTTP/1.1 200 OK\r\n' + 'Date: ' + date + 'Last-Modified: ' + last_mod_time + 'Content-Length: ' \
                  + lenFile + contentType + '\r\n'

    if int(secs) == seconds:
        print(notModMessage)
        connectionSocket.send(notModMessage.encode())
    else:
        print(okayMessage)
        connectionSocket.send(okayMessage.encode())

        resp = ''
        # Send what is in the requested file
        for i in range(0, len(fileData)):
            resp += fileData[i]

        connectionSocket.send(resp.encode())


serverSocket.close()