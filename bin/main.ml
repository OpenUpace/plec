open Plec
open Types
open Eval
open Error
open Debug

(* main *)
(* Small sample *)
let identity_bool = Lam ("x", Bool, Var "x")

let sample_program =
  If (BoolLit true, App (identity_bool, BoolLit true), BoolLit false)

let () = print_endline (string_of_term sample_program)
