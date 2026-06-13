![license](https://img.shields.io/badge/License-Apache_2.0-green)

Welcome to ple! This is a small programming language.

> **Warning!!!**
> 
> Ple Compiler(plec) is currently a **personal experimental** project now. The code and APIs are **unstable**. **DO NOT** use in production environments!!!

## Compile
You should install `git` and `opam` then:
```bash
$ git clone https://github.com/OpenUpace/plec.git
$ cd plec
$ opam install . --with-test --deps-only -y
$ dune exec plec -- -c "\\x.x" # Run the programming
```

## Features
- Curried lambda expression
- Hindley-Milner Type System (Remy's Algorithm)
- Sample CLI

---

## Acknowledgments
Thanks to the tutorial on Hindley-Milner of Prof. Okeg Kiselyov, and other Hindley-Milner Type System implementation. They help plec becomes stronger.

## Contributing
We thank you for contributing plec. Please read the CONTRIBUTING.md before contributing.

## License
Plec is released under the Apache 2.0 license.
