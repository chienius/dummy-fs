CC=g++
CFLAGS=-Iinclude
DESTDIR=build
SRCDIR=src
LIBDIR=$(SRCDIR)/lib
OBJECTS=$(DESTDIR)/BufferManager.o $(DESTDIR)/File.o $(DESTDIR)/FileSystem.o $(DESTDIR)/Inode.o $(DESTDIR)/SuperBlock.o $(DESTDIR)/User.o $(DESTDIR)/DummyKernel.o $(DESTDIR)/FileManager.o $(DESTDIR)/OpenFileManager.o $(DESTDIR)/Utility.o
SOURCES=$(LIBDIR)/BufferManager.cpp $(LIBDIR)/File.cpp $(LIBDIR)/FileSystem.cpp $(LIBDIR)/Inode.cpp $(LIBDIR)/SuperBlock.cpp $(LIBDIR)/User.cpp $(LIBDIR)/DummyKernel.cpp $(LIBDIR)/FileManager.cpp $(LIBDIR)/OpenFileManager.cpp $(LIBDIR)/Utility.cpp
LD=ld

.PHONY: clean DESTDIR

all: DESTDIR $(DESTDIR)/Initialize $(DESTDIR)/DummyFS

DESTDIR:
	mkdir -p $(DESTDIR)

$(DESTDIR)/BufferManager.o: $(LIBDIR)/BufferManager.cpp
	$(CC) $< -c -o $@ $(CFLAGS)

$(DESTDIR)/File.o: $(LIBDIR)/File.cpp
	$(CC) $< -c -o $@ $(CFLAGS)

$(DESTDIR)/FileSystem.o: $(LIBDIR)/FileSystem.cpp
	$(CC) $< -c -o $@ $(CFLAGS)

$(DESTDIR)/Inode.o: $(LIBDIR)/Inode.cpp
	$(CC) $< -c -o $@ $(CFLAGS)

$(DESTDIR)/SuperBlock.o: $(LIBDIR)/SuperBlock.cpp
	$(CC) $< -c -o $@ $(CFLAGS)

$(DESTDIR)/User.o: $(LIBDIR)/User.cpp
	$(CC) $< -c -o $@ $(CFLAGS)

$(DESTDIR)/DummyKernel.o: $(LIBDIR)/DummyKernel.cpp
	$(CC) $< -c -o $@ $(CFLAGS)

$(DESTDIR)/FileManager.o: $(LIBDIR)/FileManager.cpp
	$(CC) $< -c -o $@ $(CFLAGS)

$(DESTDIR)/OpenFileManager.o: $(LIBDIR)/OpenFileManager.cpp
	$(CC) $< -c -o $@ $(CFLAGS)

$(DESTDIR)/Utility.o: $(LIBDIR)/Utility.cpp
	$(CC) $< -c -o $@ $(CFLAGS)

$(DESTDIR)/Initialize: $(OBJECTS) $(SRCDIR)/Initialize.cpp
	$(CC) $^ -o $@ $(CFLAGS)

$(DESTDIR)/TestProgram: $(OBJECTS) $(SRCDIR)/TestProgram.cpp
	$(CC) $^ -o $@ $(CFLAGS)

$(DESTDIR)/DummyFS: $(OBJECTS) $(SRCDIR)/CommandUtility.cpp $(SRCDIR)/main.cpp
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm -rf $(DESTDIR)/**
