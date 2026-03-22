Welcome to ple! This is a small programming language.

> **Warning!!!**
> 
> Ple Compiler(plec) is currently a **personal experimental** project. The code and APIs are **unstable**. **DO NOT** use in production environments!!!

## Compile
You should install `rustc`, `cargo`, and `git` then:
```bash
$ git clone https://github.com/OpenUpace/plec.git
$ cd plec
$ cargo build && cargo run -- '42+1'
```
> Note: plec doesn't have test now, so only in `cargo run` works :-(

## Features
- A simple calculator DSL now.
- Compiles to LLVM IR(using Inkwell).
- Supports unsigned divison and modulo operators.

## Bugs
- Module is not clear.
- Many **TODO lists** and **hacks**(see source code).
- The code is not clear or efficient.

---

## Contributing
We thank you for contributing our code. Please see the CONTRIBUTING.md before the real contributing.

## License
To better contribute, we use the Apache License 2.0.
