
- this is used to make the include external header in a .c file while building it 
`gcc -Icommon lab/gen.c -o lab/gen`


- the command to use for building 
```bash
gcc -std=c11 -Wall -Wextra -Werror -Wshadow -Wconversion -g \
    -fsanitize=address,undefined \
    -Icommon host/gen.c common/protocol.c -o build/gen
```

| Flag / Part                    | Meaning             | What it does                                                    |
| ------------------------------ | ------------------- | --------------------------------------------------------------- |
| `gcc`                          | GNU C Compiler      | Compiles C source code                                          |
| `-std=c11`                     | C standard          | Compile using the **C11** standard                              |
| `-Wall`                        | Warnings            | Enables a broad set of useful warnings                          |
| `-Wextra`                      | Extra warnings      | Enables additional warnings beyond `-Wall`                      |
| `-Werror`                      | Warnings → errors   | Treats compiler warnings as errors; build fails                 |
| `-Wshadow`                     | Shadowing warning   | Warns when a variable hides another variable with the same name |
| `-Wconversion`                 | Conversion warnings | Warns about potentially unsafe implicit type conversions        |
| `-g`                           | Debug info          | Adds debugging information for **GDB/LLDB**                     |
| `-fsanitize=address,undefined` | Runtime sanitizers  | Detects memory errors and many types of undefined behavior      |
| `-Icommon`                     | Include path        | Tells GCC to search `common/` for `.h` files                    |
| `host/gen.c`                   | Source file         | Compiles your `gen.c` program                                   |
| `common/protocol.c`            | Source file         | Compiles your protocol implementation                           |
| `-o build/gen`                 | Output              | Creates the executable as `build/gen`                           |


```
Strict C11 + lots of warnings + debugging + runtime bug detection
        ↓
compile gen.c + protocol.c
        ↓
       build/gen    
       

- to run it 
./build/gen