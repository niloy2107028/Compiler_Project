# Demo Instructions for Week 1

## Commands to Run

### 1. Go to project folder

```powershell
cd "E:\3.2\Compiler LAB\PROJECT\Compiler_Project"
```

### 2. Build the lexer (one time only)

```powershell
.\flex\win_flex.exe lexer.l
gcc -g -Wall lex.yy.c -o bangla_lexer.exe
```

### 3. Run the lexer

```powershell
.\bangla_lexer.exe test_program.txt
```

---

## What to Show Teacher

1. **Show the test program** (test_program.txt):
   - Open it and show the Bangla code

2. **Run the lexer** and show the output:
   - It prints all the tokens

3. **Show lexer.l file**:
   - Point out the Bangla keyword patterns
   - Show the token printing

4. **Explain what you did**:
   - "I made a lexical analyzer using Flex"
   - "It reads Bangla code and breaks it into tokens"
   - "I can identify all 10 keywords, numbers, and operators"
   - "Next week I'll add the parser"

---

## Expected Output

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

## If Teacher Asks Questions

**Q: What does the lexer do?**
A: It's the first phase of the compiler. It reads the source code and converts it into tokens.

**Q: Why Bangla keywords?**
A: To make programming more accessible in our language. The syntax follows C++.

**Q: Where's the parser?**
A: That's for next week. Week 1 is just lexical analysis.

**Q: Can it run programs?**
A: Not yet. Right now it just tokenizes. Full compilation will come after adding parser, semantic analyzer, and code generator.

**Q: What tools did you use?**
A: Flex for the lexer and GCC to compile.

---

That's it! Keep it simple.
