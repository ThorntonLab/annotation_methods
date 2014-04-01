CXX=c++
CXXFLAGS=-O2 -Wall -W -I$(HOME)/include

all:  splitall.o
	$(CXX) $(CXXFLAGS) -o splitall splitall.o -L$(HOME)/lib -lsequence -static $(LDFLAGS)