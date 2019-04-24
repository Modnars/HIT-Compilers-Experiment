CC=g++
CFLAGS=-std=c++11 -Wall
LIBS=
PARSER=parsers
SCANNER=scanner
TRANSLATOR=translator
MAIN=main
OBJ1=$(SCANNER).o SymbolTable.o Token.o Base.o
OBJ2=$(PARSER).o BasicFunc.o Item.o Production.o $(TRANSLATOR).o
OBJS=$(OBJ1) $(OBJ2)
HEADFILES=lib/*.hpp
PROGRAM=res
DEBUGER=lldb

m : $(MAIN).out

$(MAIN).out : $(OBJ1) $(OBJ2) $(MAIN).o
	$(CC) $(CFLAGS) $(OBJ1) $(OBJ2) $(MAIN).o -o $(MAIN).out 

$(MAIN).o : $(MAIN).cpp lib/Token.hpp lib/SymbolTable.hpp lib/Base.hpp \
        lib/Item.hpp lib/Production.hpp lib/BasicFunc.hpp lib/parser.hpp \
        lib/scanner.hpp lib/translator.hpp
	$(CC) $(CFLAGS) $(MAIN).cpp -c

s : $(SCANNER)

$(SCANNER) : $(OBJ1)
	$(CC) $(CFLAGS) $(OBJ1) -o $(SCANNER)

$(OBJ1) : $(SCANNER).cpp lib/Token.hpp lib/SymbolTable.hpp lib/Base.hpp \
        Token.cpp SymbolTable.cpp Base.cpp
	$(CC) $(CFLAGS) $(SCANNER).cpp Token.cpp SymbolTable.cpp Base.cpp -c

p : $(PARSER)

$(PARSER) : $(OBJ2)
	$(CC) $(CFLAGS) $(OBJ2) -o $(PARSER)

$(OBJ2) : $(PARSER).cpp lib/Item.hpp lib/Production.hpp lib/BasicFunc.hpp \
        Item.cpp Production.cpp BasicFunc.cpp translator.cpp lib/translator.hpp
	$(CC) $(CFLAGS) $(PARSER).cpp $(TRANSLATOR).cpp Item.cpp Production.cpp BasicFunc.cpp -c

d :
	$(CC) $(CFLAGS) *.cpp -o $(MAIN).dbg -g
	$(DEBUGER) $(MAIN).dbg

r : 
	./main.out -n ../test/test.cpp

ui : 
	python3 compiler.py

clean :
	rm -rf *.o *.out *.dbg *.dbg.dSYM
