open Plec
open Types
open Semantics
open Eval
open Error

(* For Debug *)
let rec string_of_term = function
  | Var x -> x
  | Lam (fn, ty, arg) -> "<fun> " ^ fn ^ string_of_ty ty ^ string_of_term arg
  | App (fn, arg) -> "<abs> " ^ string_of_term fn ^ string_of_term arg
  | BoolLit b -> string_of_bool b
  | If (cond, when_true, when_false) ->
      "<if> " ^ string_of_term cond ^ "<then> " ^ string_of_term when_true
      ^ "<else> " ^ string_of_term when_false

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

let () = test sample_program
