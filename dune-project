(lang dune 3.22)

(name plec)

(generate_opam_files true)

(source
 (github OpenUpace/plec))

(authors "Ai-Upace <jntmngmhahayo@outlook.com>")

(maintainers "Ai-Upace <jntmngmhahayo@outlook.com>")

(license Apache-2.0)

(documentation https://url/to/documentation)

(using menhir 3.0)

(package
 (name plec)
 (synopsis "Ple compiler")
 (description "A compiler")
 (depends ocaml
          dune
          sedlex
          menhirLib
          ppxlib
          (ppx_expect :with-test)
          ))
