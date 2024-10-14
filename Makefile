
compil: 
	g++ src/*.cpp -o main -Wall -g -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

exec: compil
	./main
