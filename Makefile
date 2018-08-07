TARGET 		= Main

CC 			= gcc
CXX 		= g++

INCLUDES 	=
CFLAGS 		= -g -Wall $(INCLUDES)
CXXFLAGS 	= -g -Wall $(INCLUDES) -std=c++11 -O2

LDFLAGS 	= -g
LDLIBS 		= -lsfml-audio -lsfml-system

$(TARGET): Analyzable.o

Analyzable.o:

.PHONY: clean
clean:
	rm -rf a.out core *.o $(TARGET)
