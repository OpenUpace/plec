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

let pretty_name index =
  if index < 26 then "'" ^ String.make 1 (Char.chr (Char.code 'a' + index))
  else "'t" ^ string_of_int index

let rec string_of_ty ty =
  let rec go prec names next ty =
    match repr ty with
    | TVar { contents = Unbound (name, _) } -> (
        match List.assoc_opt name names with
        | Some pretty -> (pretty, names, next)
        | None ->
            let pretty = pretty_name next in
            (pretty, (name, pretty) :: names, next + 1))
    | TArrow (left, right, _) ->
        let left_text, names, next = go 1 names next left in
        let right_text, names, next = go 0 names next right in
        let body = left_text ^ " -> " ^ right_text in
        let text = if prec > 0 then "(" ^ body ^ ")" else body in
        (text, names, next)
    | TInt ->
        let text = "int" in
        let text = if prec > 0 then "(" ^ text ^ ")" else text in
        (text, names, next)
    | TBool ->
        let text = "bool" in
        let text = if prec > 0 then "(" ^ text ^ ")" else text in
        (text, names, next)
    | TVar { contents = Link _ } -> assert false
  in
  let text, _, _ = go 0 [] 0 ty in
  text
