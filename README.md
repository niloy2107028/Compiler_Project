# Bangla Programming Language Compiler

## About This Project

I'm building a compiler for a programming language that uses Bangla keywords instead of English. The goal is to translate Bangla source code into executable form. The syntax is based on C++.

---

## Week 1 Progress (First 5%)

### ✅ Completed Tasks

1. **Lexical Analyzer (Flex) - `lexer.l`**
   - Recognizes all Bangla keywords: পুন, ভগ, যদি, যদিনা, যতক্ষণ, লুপ, ইনপুট, দেখাও, শুরু, শেষ
   - Handles Bangla numbers (০-৯) and English numbers (0-9)
   - Recognizes Bangla and English identifiers
   - Tokenizes operators: +, -, \*, /, =, >, <, >=, <=, ==, !=
   - Handles delimiters: ( ) { } ; ,

2. **Parser (Bison) - `parser.y`**
   - Grammar rules for variable declarations
   - Grammar rules for arithmetic expressions
   - Grammar rules for conditional statements (if-else)
   - Grammar rules for loops (while, for)
   - Grammar rules for input/output operations

3. **Token Header - `token.h`**
   - Defines all token types
   - Token structure definition

4. **Build System - `Makefile`**
   - Automated compilation process
   - Clean and test targets

---

## Supported Features

### Keywords Mapping

| C++ Keyword | Bangla Keyword | Purpose          |
| ----------- | -------------- | ---------------- |
| int         | পুন            | Integer variable |
| float       | ভগ             | Floating-point   |
| if          | যদি            | Condition        |
| else        | যদিনা          | Else block       |
| while       | যতক্ষণ         | While loop       |
| for         | লুপ            | For loop         |
| cin         | ইনপুট          | Input            |
| cout        | দেখাও          | Output           |
| {           | শুরু           | Block start      |
| }           | শেষ            | Block end        |

### What the Language Can Do (Planned)

- Declare integer and float variables
- Do math calculations
- If-else conditions
- While and for loops
- Input/output
- Comparisons

---

## How to Build and Run

### What You Need

- Flex (for lexical analysis)
- GCC (C compiler)

### Installation

**Windows:**

1. Install MinGW or Cygwin
2. Get Flex from [GnuWin32](http://gnuwin32.sourceforge.net/packages/flex.htm)

**Linux:**

```bash
sudo apt-get install flex gcc
```

---

### Building

```bash
cd Compiler_Project
.\flex\win_flex.exe lexer.l
gcc -g -Wall lex.yy.c -o bangla_lexer.exe
```

If make doesn't work:

```bash
flex lexer.l
gcc -g -Wall lex.yy.c -o bangla_lexer
```

---

### Running

```bash
./bangla_lexer test_program.txt
```

On Windows:

```bash
bangla_lexer.exe test_program.txt
```

### Example Bangla Program

Create a file `test_program.txt` with the following content:

```
শুরু
পূর্ণসংখ্যা ক;
ক = ১০;
দেখাও ক;
শেষ
```

This is equivalent to:

```c++
{
    int k;
    k = 10;
    cout << k;
}
```

---

## Sample Programs (for future testing)

### 1. Simple Variable

```
শুরু
পুন ক;
ক = ১০;
দেখাও ক;
শেষ
```

### 2. Math Expression

```
শুরু
পুন ক, খ;
ক = ৫;
খ = ক + ৩ * ২;
দেখাও খ;
শেষ
```

### 3. If-Else

```
শুরু
পুন ক;
ক = ৭;
যদি (ক > ৫)
দেখাও ক;
যদিনা
দেখাও ০;
শেষ
```

### 4. Loop

```
শুরু
পুন ক;
ক = ১;
যতক্ষণ (ক <= ৫)
দেখাও ক;
ক = ক + ১;
শেষ
```

---

## Files in This Project

```
Compiler_Project/
├── lexer.l              - The lexical analyzer I made
├── token.h              - Token definitions
├── test_program.txt     - Test file
├── flex/                - Flex tool (win_flex.exe)
└── README.md            - This file
```

---

## Testing

Run the lexer:

```bash
.\bangla_lexer.exe test_program.txt
```

Clean up:

```bash
make clean
```

### What You'll See

Output from test_program.txt:

```
Starting lexical analysis...

Token: BEGIN
Token: INT
Token: BANGLA_IDENTIFIER (ক)
Token: COMMA
Token: BANGLA_IDENTIFIER (খ)
Token: SEMICOLON
Token: BANGLA_IDENTIFIER (ক)
Token: ASSIGN
Token: BANGLA_NUMBER (৫)
Token: SEMICOLON
Token: BANGLA_IDENTIFIER (খ)
Token: ASSIGN
Token: BANGLA_IDENTIFIER (ক)
Token: PLUS
Token: BANGLA_NUMBER (৩)
Token: MULTIPLY
Token: BANGLA_NUMBER (২)
Token: SEMICOLON
Token: OUTPUT
Token: BANGLA_IDENTIFIER (খ)
Token: SEMICOLON
Token: END

Lexical analysis completed.
```

---

## Technical Notes

**Week 1 - Lexical Analysis**

- Takes Bangla code as input
- Outputs tokens to console
- Handles UTF-8 Bangla characters
- Status: Done ✓

**Week 2-3 - Parser (Not started yet)**

- Will use Bison
- Grammar rules
- Syntax checking

**Token Types:**

- 10 keywords
- 11 operators
- 6 delimiters
- Identifiers (Bangla & English)
- Numbers (Bangla & English)

---

## Future Plan

1. **Parser** (Week 2-3)
   - Add Bison parser
   - Grammar rules
2. **Semantic Analysis** (Week 4-5)
   - Type checking
   - Scope validation

3. **Code Generation**
   - Translate to C code
4. **Optimization** (maybe)
   - If time permits

---

## Current Limitations

Right now only the lexer works. Still need to add:

- Parser (syntax checking)
- Semantic analyzer
- Code generator
- Better error messages
- Functions
- Arrays

---

## References

1. Compilers: Principles, Techniques, and Tools (Aho, Lam, Sethi, Ullman)
2. Flex documentation
3. Course materials from Compiler Lab

---

**Week 1 - January 2026**
