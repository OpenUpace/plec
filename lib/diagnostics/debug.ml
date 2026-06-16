(*
   Copyright 2026 Ai-Upace

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

let rec string_of_term = function
  | Var x -> x
  | Lam (fn, arg) -> " <fun> " ^ fn ^ ". " ^ string_of_term arg
  | App (e1, e2) -> " <app> " ^ string_of_term e1 ^ string_of_term e2
  | Let (bind_name, t1, t2) ->
      " <let> " ^ bind_name ^ " = " ^ string_of_term t1 ^ " in "
      ^ string_of_term t2
  | IntLit n -> string_of_int n
  | BoolLit b -> string_of_bool b
