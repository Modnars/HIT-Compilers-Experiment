CC=g++
CFLAGS=-std=c++11 -Wall
LIBS=
PARSER=parser
SCANNER=scanner
OBJS=$(PARSER).o #$(SCANNER).o
HEADFILES=lib/*.hpp
PROGRAM=res.out

TEST=Test

$(PROGRAM) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROGRAM)

$(OBJS) : $(PARSER).cpp $(HEADFILES)
	$(CC) $(CFLAGS) $(PARSER).cpp -c

run :
	./$(PROGRAM)

test :
	./$(PROGRAM) ../test/test.cpp

t :
	$(CC) $(CFLAGS) $(TEST).cpp -o $(TEST).out && ./$(TEST).out

clean :
	rm *.o *.out
