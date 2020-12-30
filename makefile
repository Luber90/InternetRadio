all: serv client

serv: Serwer/RadioServer.cpp Serwer/Server.cpp Serwer/Server.h Serwer/RoomInfo.h Serwer/KlientInfo.h
	g++ --std=c++17 -Wall -O0 -g -pthread -o serv Serwer/RoomInfo.cpp Serwer/KlientInfo.cpp Serwer/RadioServer.cpp Serwer/Server.cpp
	
client: Klient/Client/main.cpp Klient/Client/mainwindow.cpp Klient/Client/mainwindow.h Klient/Client/mainwindow.ui
	cmake Klient/Client/CMakeLists.txt
	cp Klient/build-Client-Desktop-Debug/Client client
