CC=g++
CFLAGS=-I ./includes
DESTDIR=build

.PHONY: clean

all: DestDirMkDir Initialize

DestDirMkDir:
	mkdir $(DESTDIR)

SuperBlock.o: ./src/SuperBlock.cpp
	$(CC) ./src/SuperBlock.cpp -c -o $(DESTDIR)/SuperBlock.o $(CFLAGS)

Initialize: SuperBlock.o ./src/Initialize.cpp
	$(CC) ./src/Initialize.cpp $(DESTDIR)/SuperBlock.o -o $(DESTDIR)/Initialize $(CFLAGS)

clean: .phony
	rm -rf $(DESTDIR)
