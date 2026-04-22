module StringMap = Map.Make (String)

(* AST *)
type ty = Bool | Arrow of ty * ty

type term =
  | Var of string
  | Lam of string * ty * term
  | App of term * term
  | BoolLit of bool
  | If of term * term * term

type context = ty StringMap.t
