.PHONY:all
all:ServerNetCald client_netcal 

ServerNetCald:Tcpserver.cc
	g++ -o $@ $^ -std=c++17 
client_netcal:Tcpclient.cc
	g++ -o $@ $^ -std=c++17 

.PHONY:clean
clean:
	rm -rf ServerNetCald client_netcal output output.tgz