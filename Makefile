
compil: 
	g++ src/*.cpp -o build/socket -Wall -g -lsfml-graphics -lsfml-window -lsfml-system

exec: compil
	./build/socket
