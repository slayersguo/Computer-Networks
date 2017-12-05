all: Webserver.cpp
	g++ Webserver.cpp -o webserver

clean:
	rm -rf webserver *~
