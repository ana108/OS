all:  main.o FileManager.o
	g++-6 -o share main.o FileManager.o

main: main.o
	g++-6 -c  main.c

send: send.o FileManager.o
	g++-6 -o send send.o FileManager.o

send.o: send.c
	g++-6 -c send.c

FileManager.o: FileManager.cc
	g++-6 -c FileManager.cc	

clean:
	rm -f *.o share send
