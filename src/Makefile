CC=g++
CFLAGS=-std=c++11 -Wall 
LIBS=
PARSER=parser
SCANNER=scanner
OBJS=$(PARSER).o #$(SCANNER).o 
HEADFILES=lib/*.hpp
PROGRAM=res
DEBUGER=lldb

TEST=Test

$(PROGRAM) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROGRAM) 

$(OBJS) : $(PARSER).cpp $(HEADFILES)
	$(CC) $(CFLAGS) $(PARSER).cpp -c

r :
	./$(PROGRAM)

d :
	$(CC) $(CFLAGS) -g $(PARSER).cpp -o $(PROGRAM).out
	$(DEBUGER) $(PROGRAM).out 

t :
	$(CC) $(CFLAGS) $(TEST).cpp -o $(TEST).out -g && ./$(TEST).out

clean :
	rm -rf *.o *.out *.out.dSYM
