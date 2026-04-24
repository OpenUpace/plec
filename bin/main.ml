open Plec
open Types
open Semantics
open Eval
open Error
open Debug

let test term =
  try
    let v = eval term in
    print_endline ("Result " ^ string_of_term v);

    match type_of StringMap.empty term with
    | Ok ty -> print_endline ("Type: " ^ string_of_ty ty)
    | Error err -> print_endline ("Type error: " ^ string_of_type_error err)
  with e -> print_endline ("Error: " ^ Printexc.to_string e)

(* main *)
(* Small sample *)
let identity_bool = Lam ("x", Bool, Var "x")

let sample_program =
  If (BoolLit true, App (identity_bool, BoolLit true), BoolLit false)

let () = print_endline (string_of_term sample_program)
