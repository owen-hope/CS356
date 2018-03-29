#Owen Hope
#oeh3
#Section 002

import sys
import re
from socket import *

argv = sys.argv
webUrl = argv[1]
count = 2
responseArray = []
lastModified = ''
fileNot = True

# localhost:9007/filename.html
info = re.split('[:/]', webUrl)

hostName = info[0]
if (hostName == 'localhost'):
    host = '127.0.0.1'
else:
    host = hostName
port = int(info[1])
nameOfFile = info[2]

getMethod = 'GET /' + nameOfFile + ' HTTP/1.1\r\nHost: ' + host + ':' + info[1] + '\r\n\r\n'

# Data to be sent
data = 'X'

# Create TCP client socket
clientSocket = socket(AF_INET, SOCK_STREAM)

# Create a TCP connection to server
print("Connecting to " + host + "," + str(port))
clientSocket.connect((host, port))

# Printing contents of the header in the HTTP Request
print("Contents of header for HTTP request: \n" + getMethod)

# Send data through TCP connection
clientSocket.send(getMethod.encode())

# Receive the server response
resp = ''
receive = clientSocket.recv(100000000)
resp += receive.decode()


if int(resp[9:12]) == 404:
    fileNot = False

responseArray = resp.split("\n")

# Display the server response as an output
print("Received from server:\n" + resp)

if fileNot:
    for value in responseArray:
        if value[:13] == 'Last-Modified':
            lastModified = value[15:-1]


    conditionalGetRequest = 'GET /' + nameOfFile + ' HTTP/1.1\r\nHost: ' + host + ':' + info[1] + '\r\n' + \
                            'If-Modified-Since: ' + lastModified + '\r\n\r\n'

    print('Contents of header for Conditional GET request:\n' + conditionalGetRequest)
    clientSocket.send(conditionalGetRequest.encode())

    conditionalGetResponse = ''
    receive = clientSocket.recv(1000000)
    conditionalGetResponse += receive.decode()
    # Display the server response as an output
    print("Conditional Get Response:\n" + conditionalGetResponse)

# Close the socket
clientSocket.close()