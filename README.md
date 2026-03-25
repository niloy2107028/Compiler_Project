# 🇧🇩 Bangla Programming Language Compiler

> Write code in Bangla! A complete compiler that lets you program using Bengali keywords and syntax.

## 📖 What is This?

This is a **fully functional compiler** for a programming language that uses **Bangla (Bengali) keywords**. Instead of writing `if`, `while`, and `print`, you can write `যদি`, `যতক্ষণ`, and `দেখাও`!

**Example Program:**

```bengali
শুরু
পুন x = ১০;
পুন y = ২০;
পুন sum = x + y;
দেখাও sum;
শেষ
```

**Output:** `30`

---

## ✨ Features

- ✅ **Bangla Keywords** - Use Bengali words to write code
- ✅ **Bangla Numerals** - Support for Bengali digits (০-৯)
- ✅ **Complete Compiler Pipeline** - Lexer → Parser → AST → Semantic Analysis → TAC Generation
- ✅ **Type Checking** - Catches type errors before execution
- ✅ **Control Flow** - If/else statements, while loops, for loops
- ✅ **Functions** - Define and call functions
- ✅ **Intermediate Code** - Generates Three-Address Code (TAC) for optimization
- ✅ **Error Messages** - Clear error reporting with line numbers

---

## 🚀 Quick Start

### Prerequisites

- **GCC** (C compiler)
- **Flex** (lexer generator) - `win_flex` on Windows
- **Bison** (parser generator) - `win_bison` on Windows

### Build the Compiler

```powershell
# Step 1: Generate parser
win_bison -d -Wall parser.y

# Step 2: Generate lexer
win_flex lexer.l

# Step 3: Compile
gcc -std=c11 -Wall -Wextra -pedantic -I./src src/main.c src/ast.c src/symbol_table.c src/semantic.c src/ir.c parser.tab.c lex.yy.c -o bangla_compiler.exe
```

### Run Your First Program

```powershell
# Run the compiler
.\bangla_compiler.exe test_program.txt output.tac

# Run test programs
.\bangla_compiler.exe tests/valid_core.bn
.\bangla_compiler.exe tests/valid_loop.bn
.\bangla_compiler.exe tests/valid_function.bn
```

---

## 📝 Language Reference

### Keywords

| Bangla     | English  | Purpose               |
| ---------- | -------- | --------------------- |
| **শুরু**   | begin    | Start a block         |
| **শেষ**    | end      | End a block           |
| **পুন**    | int      | Integer variable      |
| **ভগ**     | float    | Float variable        |
| **যদি**    | if       | Conditional statement |
| **যদিনা**  | else     | Else statement        |
| **যতক্ষণ** | while    | While loop            |
| **লুপ**    | for      | For loop              |
| **ইনপুট**  | input    | Read input            |
| **দেখাও**  | print    | Print output          |
| **ফাংশন**  | function | Function definition   |
| **ফেরত**   | return   | Return statement      |

### Data Types

- **পুন** (Integer) - Whole numbers: `০`, `১`, `১০`, `-৫`
- **ভগ** (Float) - Decimal numbers: `৩.১৪`, `২.৫`, `-১.৫`

### Operators

**Arithmetic:** `+`, `-`, `*`, `/`
**Comparison:** `>`, `<`, `>=`, `<=`, `==`, `!=`

---

## 📚 Example Programs

### 1. Hello World (Print a Number)

```bengali
শুরু
দেখাও ৪২;
শেষ
```

### 2. Variables and Arithmetic

```bengali
শুরু
পুন a = ৫;
পুন b = ৩;
পুন sum = a + b;
দেখাও sum;
শেষ
```

**Output:** `8`

### 3. If-Else Statement

```bengali
শুরু
পুন age = ১৮;
যদি (age >= ১৮)
শুরু
    দেখাও ১;
শেষ
যদিনা
শুরু
    দেখাও ০;
শেষ
শেষ
```

**Output:** `1`

### 4. While Loop

```bengali
শুরু
পুন i = ১;
যতক্ষণ (i <= ৫)
শুরু
    দেখাও i;
    i = i + ১;
শেষ
শেষ
```

**Output:** `1 2 3 4 5`

### 5. Function Definition

```bengali
শুরু
ফাংশন পুন calculate()
শুরু
    ফেরত ২১ + ২১;
শেষ

পুন result = calculate();
দেখাও result;
শেষ
```

**Output:** `42`

---

## 🏗️ Project Structure

```
Compiler_Project/
├── src/                    # Source code
│   ├── main.c             # Main entry point
│   ├── ast.c/h            # Abstract Syntax Tree
│   ├── symbol_table.c/h   # Symbol table (variable tracking)
│   ├── semantic.c/h       # Semantic analysis & execution
│   └── ir.c/h             # Intermediate code generation (TAC)
│
├── lexer.l                # Flex lexer (tokenization)
├── parser.y               # Bison parser (syntax analysis)
├── token.h                # Token definitions
│
├── tests/                 # Test programs
│   ├── valid_core.bn      # Variables, if/else
│   ├── valid_loop.bn      # Loops
│   └── valid_function.bn  # Functions
│
├── Docs/                  # Documentation
└── understanding.md       # Complete tutorial (12,000+ lines!)
```

---

## 🔬 Compiler Stages

This compiler implements a complete compilation pipeline:

```
Source Code (.bn)
    ↓
1. Lexical Analysis (Flex)
   → Converts text to tokens
    ↓
2. Syntax Analysis (Bison)
   → Validates grammar, builds parse tree
    ↓
3. AST Construction
   → Creates Abstract Syntax Tree
    ↓
4. Semantic Analysis
   → Type checking, variable validation
    ↓
5. Execution
   → Runs the program (interpreter mode)
    ↓
6. TAC Generation
   → Generates Three-Address Code (IR)
    ↓
Output: Console output + TAC file
```

---

## 🧪 Test Programs

We include three comprehensive test programs:

### 1. **valid_core.bn** - Core Features

Tests: Variables, arithmetic, type coercion, if/else

```powershell
.\bangla_compiler.exe tests/valid_core.bn
# Output: 12.500000
```

### 2. **valid_loop.bn** - Loop Constructs

Tests: While loops, for loops

```powershell
.\bangla_compiler.exe tests/valid_loop.bn
# Output: 1 2 3 1 2 3
```

### 3. **valid_function.bn** - Functions

Tests: Function definition, calling, return

```powershell
.\bangla_compiler.exe tests/valid_function.bn
# Output: 42
```

---

## 📊 What Gets Generated?

### Console Output

The program execution results:

```
11
Compilation and execution successful. TAC written to output.tac
```

### TAC File (Intermediate Representation)

Three-Address Code for optimization and further compilation:

```
decl int ক
decl int খ
ক = 5
t1 = ক + 6
খ = t1
print খ
```

---

## 🎯 Supported Features

### ✅ Implemented

- [x] Variable declarations (`পুন x;`, `ভগ y;`)
- [x] Assignments (`x = ১০;`)
- [x] Arithmetic expressions (`a + b * c`)
- [x] Comparison operators (`x > y`, `a == b`)
- [x] If/Else statements
- [x] While loops
- [x] For loops
- [x] Functions (zero parameters)
- [x] Input/Output
- [x] Type checking
- [x] Bangla numerals (০-৯)
- [x] Symbol table with scoping
- [x] Error reporting
- [x] TAC generation
- [x] Constant folding optimization

### 🚧 Current Limitations

- Functions support **zero parameters only**
- No arrays or strings yet
- No logical operators (`&&`, `||`)
- No modulo operator (`%`)

---

## 📖 Documentation

- **understanding.md** - Complete 12,000+ line tutorial covering every part of the compiler
- **Docs/README-Project-Deep-Dive.md** - Technical deep dive
- **Docs/README-Teacher-Demo.md** - Teacher presentation guide
- **Docs/Rubric-Compliance-Matrix.md** - Rubric compliance details

---

## 🛠️ Building from Source

### Windows

```powershell
# Generate parser and lexer
win_bison -d -Wall parser.y
win_flex lexer.l

# Compile
gcc -std=c11 -Wall -Wextra -pedantic -I./src src/main.c src/ast.c src/symbol_table.c src/semantic.c src/ir.c parser.tab.c lex.yy.c -o bangla_compiler.exe

# Run
.\bangla_compiler.exe test_program.txt output.tac
```

### Linux/Mac

```bash
# Generate parser and lexer
bison -d -Wall parser.y
flex lexer.l

# Compile
gcc -std=c11 -Wall -Wextra -pedantic -I./src src/main.c src/ast.c src/symbol_table.c src/semantic.c src/ir.c parser.tab.c lex.yy.c -o bangla_compiler

# Run
./bangla_compiler test_program.txt output.tac
```

---

## 🎓 Educational Value

This project demonstrates:

1. **Compiler Design Principles** - Complete pipeline from source to IR
2. **Lexical Analysis** - Token recognition with UTF-8 support
3. **Syntax Analysis** - Grammar design and parsing
4. **Semantic Analysis** - Type systems and symbol tables
5. **Code Generation** - Intermediate representation
6. **Optimization** - Constant folding in AST
7. **Software Engineering** - Modular design, clean code

Perfect for:

- 📚 Compiler design courses
- 🎯 Understanding compilation process
- 🌐 Internationalization in programming languages
- 🧪 Experimenting with language design

---

## 🤝 Contributing

Want to add features? Here are some ideas:

- [ ] Add function parameters
- [ ] Add arrays
- [ ] Add strings
- [ ] Add logical operators (`&&`, `||`, `!`)
- [ ] Add modulo operator (`%`)
- [ ] Generate assembly code
- [ ] Add more optimizations
- [ ] Better error messages

---

## 📜 License

This project was created as part of a Compiler Design course.

---

## 🙏 Acknowledgments

Built using:

- **Flex** - Fast Lexical Analyzer
- **Bison** - GNU Parser Generator
- **GCC** - GNU Compiler Collection

---

## 📬 Contact

For questions or feedback about this project, please refer to the documentation in the `Docs/` folder.

---

**এখন বাংলায় কোড লিখুন! (Now write code in Bangla!)** 🇧🇩

---

_Last Updated: March 2024_
