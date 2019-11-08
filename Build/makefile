TARGET = example

all: $(TARGET) 

clean:
	rm -rf ./*.o 

lib.o: ./../alloc.h ./../alloc.c ./../libs.h
	gcc -c -o lib.o ./../alloc.c
main.o: ./../main.c ./../libs.h
	gcc -c -o main.o ./../main.c
$(TARGET): main.o lib.o
	gcc -o $(TARGET) main.o lib.o

