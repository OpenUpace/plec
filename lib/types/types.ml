module StringMap = Map.Make (String)

(* AST *)
type ty = Int | Bool | Arrow of ty * ty

type term =
  | Var of string
  | Lam of string * ty * term
  | App of term * term
  | BoolLit of bool
  | IntLit of int
  | If of term * term * term

type context = ty StringMap.t
