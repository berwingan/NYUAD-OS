output: main.o common.o
	gcc main.o common.o -o adtar  
main.o: main.c common.h
	gcc -c main.c
common.o: common.c common.h
	gcc -c common.c
clean:
	rm -f *.o
	rm -f *.ad
	rm -f adtar
