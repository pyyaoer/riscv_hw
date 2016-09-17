CC = g++
OBJECTS= test.o read_elf.o

test.exe: $(OBJECTS)
	$(CC) -o test.exe $(OBJECTS)

test.o: read_elf.h

read_elf.o: read_elf.h

.PHONY: clean

clean:
	rm -rf test *.o
