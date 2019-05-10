# HIT-Compilers-Experiment

## 哈工大《编译原理》实验

### Lab1 Lexical Analysis
### 实验一 词法分析

#### Needed file:

| # | File | Language | Note |
| - | ---- | -------- | ---- |
| 1 | [src/lib/Base.hpp](./src/lib/Base.hpp) | C++11 | |
| 2 | [src/lib/Token.hpp](./src/lib/Token.hpp) | C++11 | |
| 3 | [src/lib/SymbolTable.hpp](./src/lib/SymbolTable.hpp) | C++11 | |
| 4 | [src/lib/scanner.hpp](./src/lib/scanner.hpp) | C++11 | |
| 5 | [src/Base.cpp](./src/Base.cpp) | C++11 | |
| 6 | [src/SymbolTable.cpp](./src/SymbolTable.cpp) | C++11 | |
| 7 | [src/Token.cpp](./src/Token.cpp) | C++11 | |
| 8 | [src/scanner.cpp](./src/scanner.cpp) | C++11 | |
| 9 | [src/compiler.py](./src/compiler.py) | Python3.7 | Simple UI |

### Lab2 Gramma Analysis
### 实验二 语法分析

#### Needed file:

|  # | File | Language | Note |
| -- | ---- | -------- | ---- |
|  1 | [src/lib/Base.hpp](./src/lib/Base.hpp) | C++11 | |
|  2 | [src/lib/BasicFunc.hpp](./src/lib/BasicFunc.hpp) | C++11 | |
|  3 | [src/lib/Item.hpp](./src/lib/Item.hpp) | C++11 | |
|  4 | [src/lib/Production.hpp](./src/lib/Production.hpp) | C++11 | |
|  5 | [src/lib/SymbolTable.hpp](./src/lib/SymbolTable.hpp) | C++11 | |
|  6 | [src/lib/Token.hpp](./src/lib/Token.hpp) | C++11 | |
|  7 | [src/lib/parser.hpp](./src/lib/parser.hpp) | C++11 | |
|  8 | [src/lib/scanner.hpp](./src/lib/scanner.hpp) | C++11 | |
|  9 | [src/Base.cpp](./src/Base.cpp) | C++11 | |
| 10 | [src/BasicFunc.cpp](./src/BasicFunc.cpp) | C++11 | |
| 11 | [src/Item.cpp](./src/Item.cpp) | C++11 | |
| 12 | [src/Production.cpp](./src/Production.cpp) | C++11 | |
| 13 | [src/SymbolTable.cpp](./src/SymbolTable.cpp) | C++11 | |
| 14 | [src/Token.cpp](./src/Token.cpp) | C++11 | |
| 15 | [src/main.cpp](./src/main.cpp) | C++11 | |
| 16 | [src/scanner.cpp](./src/scanner.cpp) | C++11 | |
| 17 | [src/parser.cpp](./src/parser.cpp) | C++11 | SLR(1) |
| 18 | [src/parsers.cpp](./src/parsers.cpp) | C++11 | LR(1) |
| 19 | [src/compiler.py](./src/compiler.py) | Python3.7 | Simple UI |

### Lab3 Semantic Analysis
### 实验三 语义分析

#### Needed file:

|  # | File | Language | Note |
| -- | ---- | -------- | ---- |
|  1 | [src/lib/ASTNode.hpp](./src/lib/ASTNode.hpp) | C++11 | |
|  2 | [src/lib/Base.hpp](./src/lib/Base.hpp) | C++11 | |
|  3 | [src/lib/BasicFunc.hpp](./src/lib/BasicFunc.hpp) | C++11 | |
|  4 | [src/lib/Code.hpp](./src/lib/Code.hpp) | C++11 | |
|  5 | [src/lib/Item.hpp](./src/lib/Item.hpp) | C++11 | |
|  6 | [src/lib/Production.hpp](./src/lib/Production.hpp) | C++11 | |
|  7 | [src/lib/SymbolTable.hpp](./src/lib/SymbolTable.hpp) | C++11 | |
|  8 | [src/lib/Token.hpp](./src/lib/Token.hpp) | C++11 | |
|  9 | [src/lib/parser.hpp](./src/lib/parser.hpp) | C++11 | |
| 10 | [src/lib/scanner.hpp](./src/lib/scanner.hpp) | C++11 | |
| 11 | [src/lib/translator.hpp](./src/lib/translator.hpp) | C++11 | |
| 12 | [src/lib/ASTNode.cpp](./src/lib/ASTNode.cpp) | C++11 | |
| 13 | [src/Base.cpp](./src/Base.cpp) | C++11 | |
| 14 | [src/BasicFunc.cpp](./src/BasicFunc.cpp) | C++11 | |
| 15 | [src/Item.cpp](./src/Item.cpp) | C++11 | |
| 16 | [src/Production.cpp](./src/Production.cpp) | C++11 | |
| 17 | [src/SymbolTable.cpp](./src/SymbolTable.cpp) | C++11 | |
| 18 | [src/Token.cpp](./src/Token.cpp) | C++11 | |
| 19 | [src/main.cpp](./src/main.cpp) | C++11 | |
| 20 | [src/scanner.cpp](./src/scanner.cpp) | C++11 | |
| 21 | [src/parsers.cpp](./src/parsers.cpp) | C++11 | LR(1) |
| 22 | [src/translator.cpp](./src/translator.cpp) | C++11 | |
| 23 | [src/compiler.py](./src/compiler.py) | Python3.7 | Simple UI |

#### Note :

The file ASTVisitor.cpp and Printer.cpp are another method's source file to implement 
semantic analysis. I'm sorry that I didn't implement the method ever. 

The idea of using ASTVisitor is establish the AST first, and then use 'Visitor Pattern'
to traversing all the AST to get the target code. Fisrt of all, using the method under 
ASTNode.cpp to establish the AST, and then implement the ASTVisitor and Printer, which 
is designed to using Function Pointer to implement generating target code.

I have tried to implement the idea for three days, while my experiment is too hurry to
let me finish it. Maybe I will implement it one day. Here, I finished the basic functions
with backpatch, which I learned from book. Maybe this method is much easier.

During the experiment, I have read some good repos about compiler. I want to share 
several repos here:

[luciferldy/Compiler\_Experiment](https://github.com/luciferldy/Compiler_Experiment)

[soshimozi/Compiler](https://github.com/soshimozi/Compiler)

Thanks the authors here.

For the details relationship between the source files, please see Makefile.

Welcome make it better!

2019/04/28 Modnar
