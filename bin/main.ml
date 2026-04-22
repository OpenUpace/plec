open Plec
open Types
open Semantics
open Eval

(* Small sample *)
let identity_bool = Lam ("x", Bool, Var "x")

let sample_program =
  If (BoolLit true, App (identity_bool, BoolLit true), BoolLit false)

let sample_type = type_of StringMap.empty sample_program

(* Hack: cannot print the eval result *)
let () =
  let _ = eval sample_program in
  print_endline "End."
