open Alcotest
open Types
open Semantics

let rec pp_ty fmt = function
  | TVar { contents = Unbound (_, l) } -> Format.fprintf fmt "tv_%d" l
  | TArrow (left, right, _) ->
      let body = Format.fprintf fmt "(%a -> %a)" pp_ty left pp_ty right in
      body
  | TVar { contents = Link _ } -> assert false
  | TInt -> Format.fprintf fmt "int"
  | TBool -> Format.fprintf fmt "bool"

let rec follow = function TVar { contents = Link t } -> follow t | t -> t

let rec equal_ty ty1 ty2 =
  match (follow ty1, follow ty2) with
  | TVar { contents = Unbound (_, l1) }, TVar { contents = Unbound (_, l2) } ->
      l1 = l2
  | TArrow (tl1, tl2, _), TArrow (tr1, tr2, _) ->
      equal_ty tl1 tr1 && equal_ty tl2 tr2
  | TInt, TInt -> true
  | TBool, TBool -> true
  | _ -> false

let ty_testable = testable pp_ty equal_ty

let t1 = newvar ()
let t2 = newvar ()

let test_1 () =
  let fun_1 = Lam ("x", Var "x") in
  let f1' = top_type_check fun_1 in

  check ty_testable "identity" (new_arrow t1 t2) f1'

let test_2 () =
  let fun_1 = Lam ("x", Lam ("y", Var "x")) in
  let f1' = top_type_check fun_1 in

  check ty_testable "church true" (new_arrow t1 (new_arrow t2 t1)) f1'

let test_3 () =
  let fun_1 = Lam ("x", Lam ("y", Var "y")) in
  let f1' = top_type_check fun_1 in

  check ty_testable "church false" (new_arrow t1 (new_arrow t2 t2)) f1'

let type_tests = [ ("identity", `Quick, test_1); ("church true", `Quick, test_2); ("church false", `Quick, test_3) ]
let () = run "Plec Test" [ ("type inference", type_tests) ]
