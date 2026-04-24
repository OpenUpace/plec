(* Copyright 2026 Ai-Upace

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*)

open Types
open Error
open Semantics
open Eval

let rec string_of_term = function
  | Var x -> x
  | Lam (fn, ty, arg) ->
      " <fun> " ^ fn ^ " : " ^ string_of_ty ty ^ " -> " ^ string_of_term arg
  | App (fn, arg) -> " <app> " ^ string_of_term fn ^ string_of_term arg
  | BoolLit b -> string_of_bool b
  | IntLit i -> string_of_int i
  | If (cond, when_true, when_false) ->
      " <if> " ^ string_of_term cond ^ " <then> " ^ string_of_term when_true
      ^ " <else> " ^ string_of_term when_false

let test term =
  try
    let v = eval term in
    print_endline ("Result " ^ string_of_term v);

    match type_of StringMap.empty term with
    | Ok ty -> print_endline ("Type: " ^ string_of_ty ty)
    | Error err -> print_endline ("Type error: " ^ string_of_type_error err)
  with e -> print_endline ("Error: " ^ Printexc.to_string e)
