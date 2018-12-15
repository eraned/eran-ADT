myFileSystem: main.o map.o tree.o
	gcc main.o map.o tree.o -o myFileSystem

main.o: main.c map.h tree.h
	gcc -c main.c

map.o: map.c tree.h map.h
	gcc -c map.c

tree.o: tree.c tree.h
	gcc -c tree.c

clean:
	rm -f *.0
