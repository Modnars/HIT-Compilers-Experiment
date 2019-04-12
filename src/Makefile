CC=g++
CFLAGS=-std=c++11 -Wall
LIBS=
PARSER=parser
SCANNER=scanner
OBJS=$(SCANNER).o
#HEADFILES=lib/ASTNode.hpp lib/Token.hpp lib/SymbolTable.hpp
HEADFILES=lib/*.hpp
PROGRAM=res.out

all : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROGRAM)

$(OBJS) : $(SCANNER).cpp $(HEADFILES)
	$(CC) $(CFLAGS) $(SCANNER).cpp -c

run :
	./$(PROGRAM)

clean :
	rm *.o *.out
