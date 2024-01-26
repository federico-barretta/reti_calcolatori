server: server.o server_dtp.o
	gcc -o server server.o server_dtp.o

server.o: server.c server_dtp.h
	gcc -o server.o -c server.c

server_dtp.o: server_dtp.c server_dtp.h
	gcc -o server_dtp.o -c server_dtp.c

clean:
	rm -f *.o
	rm -f server
