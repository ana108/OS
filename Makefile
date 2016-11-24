all:  main.o
	g++ -o share main.o

main: main.o
	gcc -c  main.c

send: send.o
	g++ -o send send.o

send.o: send.c
	gcc -c send.c

clean:
	rm -f *.o
