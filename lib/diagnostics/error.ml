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
