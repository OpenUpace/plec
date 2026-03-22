Welcome use ple! This is a small programming language for education.

> **Warning!!!**
> 
> Ple Compiler(plec) is currently only a **personal experimental** project now. The code and APIs are **unstable**. **DO NOT** use in production environments!!!

## Compile
You should install the `rustc`, `cargo`, `git` then:
```bash
$ git clone https://github.com/OpenUpace/plec.git
$ cd plec
$ cargo build && cargo run -- '42+1'
```
> Note: plec doesn't have test now, so only in `cargo run` :-(

## Features
- A simple calculator DSL now.
- Compile to LLVM IR(use Inkwell).
- Support unsigned divide and modulor operators.

## Bugs
- Module is not clear.
- Many **TODO lists** and **hacks**(see source code).
- The code is not clear and efficient.

---

## Contributing
We thank you to contribute our codes. Please see the CONTRIBUTING.md before the real contributing.

## License
To better contributing, we use the Apache License 2.0.
