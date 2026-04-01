![license](https://img.shields.io/badge/License-Apache_2.0+-green)

Welcome to ple! This is a small programming language.

> **Warning!!!**
> 
> Ple Compiler(plec) is currently a **personal experimental** project. The code and APIs are **unstable**. **DO NOT** use in production environments!!!

## Compile
You should install `rustc`, `cargo`, and `git` then:
```bash
$ git clone https://github.com/OpenUpace/plec.git
$ cd plec
$ cargo build && cargo run # repl, then input code.
```
> Note: plec doesn't have test now, so only in `cargo run` works :-(

## Features
- A simple calculator DSL now.
- Compiles to LLVM IR(using Inkwell).
- Supports unsigned divison and modulo operators.
- Supports two types: int, float.(but they are not keyword now!).
- Has the REPL(but have some bugs and hacks yet).

### REPL
The plec has a sample REPL to put the LLVM IR in terminal since the 0.1.1 version,. You can:
```
$ cargo run
>> 1+1
...
>> 1.0+1
...
```
to test the code.

## Bugs
- Module is not clear.
- Many **TODO lists** and **hacks**(see source code).
- The code is not clear or efficient.

---

## Contributing
We thank you for contributing our code. Please see the CONTRIBUTING.md before the real contributing.

## License
Plec uses the Apache License 2.0 to pubilc.
