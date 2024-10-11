
compil: 
	g++ src/main.cpp -o build/main -Wall -g -lsfml-graphics -lsfml-window -lsfml-system

exec: compil
	./build/main
