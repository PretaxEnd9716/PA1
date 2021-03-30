all: main

main: URL_Parser.o
	g++ URL_Parser.o -o main

URL_Parser.o: URL_Parser.cpp
	g++ -c URL_Parser.cpp

clean:
	rm *.o main
