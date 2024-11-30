all: start client server

start: start.c
	gcc -o start start.c

client: client.c
	gcc -o client client.c

server: server.c monitor.c
	gcc -o server server.c monitor.c

clean:
	rm -f start client server