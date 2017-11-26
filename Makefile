TARGET 		= audio-vis

CC 			= gcc
CXX 		= g++

INCLUDES 	=
CFLAGS 		= -g -Wall $(INCLUDES)
CXXFLAGS 	= -g -Wall $(INCLUDES) -std=c++11

LDFLAGS 	= -g
LDLIBS 		= -lsfml-audio -lsfml-system

$(TARGET): analyzable.o

analyzable.o:

.PHONY: clean
clean:
	rm -rf a.out core *.o $(TARGET)
