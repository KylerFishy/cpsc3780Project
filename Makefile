# Makefile for the socket programming tutorial
#

server = ServerSocket.o Socket.o serverB.o ServerSocket.h Socket.h SocketException.h
client = ClientSocket.o Socket.o clientB.o ServerSocket.h Socket.h SocketException.h


all : server client

server: $(server)
	g++ -pthread -o server $(server)


client: $(client)
	g++ -o client $(client)


Socket: Socket.cpp
ServerSocket: ServerSocket.cpp
ClientSocket: ClientSocket.cpp
server: serverB.cpp
client: clientB.cpp


clean:
	rm -f *.o simple_server simple_client
