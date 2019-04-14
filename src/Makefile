CC=g++
CFLAGS=-std=c++11 -Wall
LIBS=
PARSER=parser
SCANNER=scanner
OBJS=$(SCANNER).o
HEADFILES=lib/*.hpp
PROGRAM=res.out

$(PROGRAM) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROGRAM)

$(OBJS) : $(SCANNER).cpp $(HEADFILES)
	$(CC) $(CFLAGS) $(SCANNER).cpp -c

run :
	./$(PROGRAM)

test :
	./$(PROGRAM) ../test/test.cpp

clean :
	rm *.o *.out
