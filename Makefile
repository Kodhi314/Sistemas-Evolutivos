all:
	g++ predadores.cpp main.cpp -o main -Wall -Wextra -pthread -std=c++0x
run:
	./main