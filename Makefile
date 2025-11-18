GXX = g++
CLCSRC = $(PWD)/src/clc.cpp
BDIR = $(PWD)/build/
INCDIR = $(PWD)/include/
CFLAGS = -lX11 -lGL -lXrandr -I $(INCDIR)

all: clc.o
	$(GXX) clc.o -o clc $(CFLAGS)  
	rm clc.o
clc.o:
	$(GXX) -c -fPIC  $(CLCSRC) -o clc.o $(CFLAGS)
	 
