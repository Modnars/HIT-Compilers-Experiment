CC          =g++
CFLAGS      =-std=c++11 -Wall
SCANNER     =scanner
PARSER      =parsers
TRANSLATOR  =translator
MAIN        =main
OBJ1        =$(SCANNER).o Base.o SymbolTable.o Token.o
OBJ2        =$(PARSER).o BasicFunc.o Item.o Production.o 
OBJ3        =$(TRANSLATOR).o ASTNode.o Code.o
OBJS        =$(OBJ1) $(OBJ2) $(OBJ3)
SRC1        =$(SCANNER).cpp Base.cpp SymbolTable.cpp Token.cpp
SRC2        =$(PARSER).cpp BasicFunc.cpp Item.cpp Production.cpp
SRC3        =$(TRANSLATOR).cpp ASTNode.cpp Code.cpp
SRCS        =$(SRC1) $(SRC2) $(SRC3) $(MAIN).cpp
LIB1        =lib/Base.hpp lib/SymbolTable.hpp lib/Token.hpp lib/scanner.hpp 
LIB2        =lib/BasicFunc.hpp lib/Item.hpp lib/Production.hpp lib/parser.hpp
LIB3        =lib/ASTNode.hpp lib/Code.hpp lib/translator.hpp
LIBS        =$(LIB1) $(LIB2) $(LIB3)
PROGRAM     =result
DEBUGER     =lldb

m : $(PROGRAM)

$(PROGRAM) : $(OBJS) $(MAIN).o
	$(CC) $(CFLAGS) $(OBJS) $(MAIN).o -o $(PROGRAM)

$(MAIN).o : $(MAIN).cpp $(LIBS)
	$(CC) $(CFLAGS) $(MAIN).cpp -c

s : $(SCANNER)

$(SCANNER) : $(OBJ1)
	$(CC) $(CFLAGS) $(OBJ1) -o $(SCANNER)

$(OBJ1) : $(SRC1) $(LIB1) 
	$(CC) $(CFLAGS) $(SRC1) -c

p : $(PARSER)

$(PARSER) : $(OBJ2)
	$(CC) $(CFLAGS) $(OBJ2) -o $(PARSER)

$(OBJ2) : $(SRC2) $(LIB2)
	$(CC) $(CFLAGS) $(SRC2) -c

$(OBJ3) : $(SRC3) $(LIB3)
	$(CC) $(CFLAGS) $(SRC3) -c 

d :
	$(CC) $(CFLAGS) $(SRCS) -o $(MAIN).dbg -g
	$(DEBUGER) $(MAIN).dbg

r : 
	./$(PROGRAM) -n ../test/test.cpp

ui : 
	python3 compiler.py

clean :
	rm -rf *.o *.out *.dbg *.dbg.dSYM $(PROGRAM)
