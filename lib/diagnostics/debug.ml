open Types
open Error

let rec string_of_term = function
  | Var x -> x
  | Lam (fn, ty, arg) -> " <fun> " ^ fn ^ " : " ^ string_of_ty ty ^ " -> " ^ string_of_term arg
  | App (fn, arg) -> " <app> " ^ string_of_term fn ^ string_of_term arg
  | BoolLit b -> string_of_bool b
  | IntLit i -> string_of_int i
  | If (cond, when_true, when_false) ->
      " <if> " ^ string_of_term cond ^ " <then> " ^ string_of_term when_true
      ^ " <else> " ^ string_of_term when_false
