from socket import *

import time

serverName = '192.168.49.2'
serverPort = 31478
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName,serverPort))
sentence = 'hello'
start_time=time.time()
clientSocket.send(sentence.encode())
modifiedSentence = clientSocket.recv(1024)
end_time=time.time()

print("total time: ",(end_time-start_time))

#print ('From Server:', modifiedSentence.decode())
clientSocket.close()
