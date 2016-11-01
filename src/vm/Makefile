CC = g++
CXXFLAGS += -std=c++11
OBJECTS = test.o vmemory.o

test.exe: $(OBJECTS)
	$(CC) -o test.exe $(OBJECTS)

test.o: vmemory.h

vmemory.o: vmemory.h

.PHONY: clean

clean:
	rm -rf *.exe *.o
