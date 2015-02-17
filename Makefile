Agent:main.o 
	g++ main.o -o Agent
main.o: main.cpp
	g++ main.cpp -c
