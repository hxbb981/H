.PHONY:all
all:tcpserver tcpclient

tcpclient:Tcpclient.cc
	g++ -o $@ $^ -std=c++17 -ljsoncpp -lpthread

tcpserver:Tcpserver.cc
	g++ -o $@ $^ -std=c++17 -ljsoncpp -lpthread
.PHONY:clean
clean:
	rm -f tcpserver tcpclient