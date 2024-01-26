client: client.o client_dtp.o
	gcc -o client client.o client_dtp.o

client.o: client.c client_dtp.h
	gcc -o client.o -c client.c

client_dtp.o: client_dtp.c client_dtp.h
	gcc -o client_dtp.o -c client_dtp.c

clean:
	rm -f *.o
	rm -f *.txt
	rm -f client
