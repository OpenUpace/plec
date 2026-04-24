open Types
open Semantics

(* Dynamic semantics *)
let is_value = function
  | Lam _ | BoolLit _ -> true
  | IntLit _ -> true
  | _ -> false

let rec subst name replacement term =
  match term with
  | Var var when String.equal var name -> replacement
  | Var _ | BoolLit _ | IntLit _ -> term
  | Lam (param, param_ty, body) when String.equal param name ->
      Lam (param, param_ty, body)
  | Lam (param, param_ty, body) ->
      Lam (param, param_ty, subst name replacement body)
  | App (fn, arg) -> App (subst name replacement fn, subst name replacement arg)
  | If (cond, when_true, when_false) ->
      If
        ( subst name replacement cond,
          subst name replacement when_true,
          subst name replacement when_false )

let rec eval = function
  | App (fn, arg) -> (
      let fn' = eval fn in
      let arg' = eval arg in
      match (fn', arg') with
      | Lam (param, _, body), value when is_value value ->
          eval (subst param value body)
      | value_fn, value_arg -> App (value_fn, value_arg))
  | If (cond, when_true, when_false) -> (
      match eval cond with
      | BoolLit true -> eval when_true
      | BoolLit false -> eval when_false
      | cond' -> If (cond', when_true, when_false))
  | term -> term
