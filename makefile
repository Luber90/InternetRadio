all: serv client

serv: RadioServer.cpp Server.cpp Server.h
	g++ --std=c++17 -Wall -O0 -g -pthread -o serv RadioServer.cpp Server.cpp
client: KlientTest.cpp
	g++ --std=c++17 -Wall -O0 -g -pthread -o client KlientTest.cpp