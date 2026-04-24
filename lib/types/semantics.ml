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

(* Static semantics *)
type type_error =
  | Unbound_variable of string
  | Not_a_function of ty
  | Mismatch of { expected : ty; found : ty }

let rec type_of (ctx : context) (term : term) : (ty, type_error) result =
  match term with
  | Var name -> (
      match StringMap.find_opt name ctx with
      | Some ty -> Ok ty
      | None -> Error (Unbound_variable name))
  | Lam (param, param_ty, body) ->
      let ctx' = StringMap.add param param_ty ctx in
      Result.map (fun body_ty -> Arrow (param_ty, body_ty)) (type_of ctx' body)
  | App (fn, arg) -> (
      match type_of ctx fn with
      | Error err -> Error err
      | Ok (Arrow (param_ty, result_ty)) -> (
          match type_of ctx arg with
          | Ok arg_ty when param_ty = arg_ty -> Ok result_ty
          | Ok arg_ty ->
              Error (Mismatch { expected = param_ty; found = arg_ty })
          | Error err -> Error err)
      | Ok ty -> Error (Not_a_function ty))
  | BoolLit _ -> Ok Bool
  | IntLit _ -> Ok Int
  | If (cond, when_true, when_false) -> (
      match type_of ctx cond with
      | Error err -> Error err
      | Ok Bool -> (
          match (type_of ctx when_true, type_of ctx when_false) with
          | Ok left_ty, Ok right_ty when left_ty = right_ty -> Ok left_ty
          | Ok left_ty, Ok right_ty ->
              Error (Mismatch { expected = left_ty; found = right_ty })
          | Error err, _ | _, Error err -> Error err)
      | Ok cond_ty -> Error (Mismatch { expected = Bool; found = cond_ty }))
