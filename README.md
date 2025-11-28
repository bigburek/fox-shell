# fox-shell
A simple Unix-like shell implemented in C.

Make sure you have a C compiler installed (`gcc` or `clang`).

### Compile:

```sh
gcc -o fs main.c builtin.c uthash.h
```

### Features:
- Unlimited input and parsing (whitespaces only for now)
- Starting programs
- Builtins (hashmap implementation)

### TODOs and Improvements:
- Quotes
- Backlash escaping
- Piping
- Input/output redirection
- Globbing
- Environment variable expansion
- Command history
- Startup script (export env vars)
