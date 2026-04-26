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
