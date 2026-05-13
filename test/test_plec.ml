open Alcotest
open Format
open Types
open Semantics
open Error

let rec pp_ty fmt = function
  | Int -> Format.fprintf fmt "Int"
  | Bool -> Format.fprintf fmt "Bool"
  | Arrow (t1, t2) -> Format.fprintf fmt "(%a -> %a)" pp_ty t1 pp_ty t2

let rec equal_ty ty1 ty2 =
  match (ty1, ty2) with
  | Int, Int -> true
  | Bool, Bool -> true
  | Arrow (t1, t2), Arrow (t3, t4) -> equal_ty t1 t3 && equal_ty t2 t4
  | _ -> false

let ty_testable = testable pp_ty equal_ty

let test_type_check_1 () =
  let code = "if true then 1 else 2" in

  let buf = Sedlexing.Utf8.from_string code in
  let tokenize, lexbuf = Lexer.tokenize buf in
  let result = Parser.term tokenize lexbuf in

  match type_of StringMap.empty result with
  | Ok ty -> check ty_testable "Int with Int" ty Int
  | Error err -> print_endline ("Error: " ^ string_of_type_error err)

let () =
  run "Plec Test"
    [ ("type tests", [ test_case "Test type 1" `Quick test_type_check_1 ]) ]
