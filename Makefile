all: menu.o
	g++ -o share menu.o  

menu.o: menu.c
	gcc -c menu.c 

clean:
	rm -f *.o share
