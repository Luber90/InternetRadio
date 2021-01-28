all: serv

serv: Serwer/RadioServer.cpp Serwer/Server.cpp Serwer/Server.h Serwer/RoomInfo.h Serwer/KlientInfo.h Serwer/Queue.h
	g++ --std=c++17 -Wall -O0 -g -pthread -o serv Serwer/RoomInfo.cpp Serwer/KlientInfo.cpp Serwer/RadioServer.cpp Serwer/Server.cpp Serwer/Queue.cpp

