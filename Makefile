all:
	cd src && g++ -Wall -pedantic -std=c++11 dns.cpp paramParse.cpp createSock.cpp parseDns.cpp -o ../dns
run:
	./dns
