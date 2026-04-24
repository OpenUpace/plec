open Types
open Semantics

let rec string_of_ty = function
  | Int -> "Int"
  | Bool -> "Bool"
  | Arrow (t1, t2) -> "(" ^ string_of_ty t1 ^ " -> " ^ string_of_ty t2 ^ ")"

let rec string_of_type_error = function
  | Unbound_variable str -> "unbound variable: " ^ str
  | Not_a_function ty ->
      "trying to apply a non-function value of type " ^ string_of_ty ty
  | Mismatch { expected; found } ->
      "expected " ^ string_of_ty expected ^ " found " ^ string_of_ty found
