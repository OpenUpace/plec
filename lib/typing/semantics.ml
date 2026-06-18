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
open Infer

(* Static semantics *)
(* type environment *)
type env = (varname * typ) list

(* Sound generalization *)
let force_delayed_adjustments () =
  let rec loop acc level ty =
    match repr ty with
    | TVar ({ contents = Unbound (name, l) } as tvr) when l > level ->
        tvr := Unbound (name, level);
        acc
    | TArrow (_, _, ls) when ls.level_new = marked_level ->
        failwith "occurs check"
    | TArrow (_, _, ls) as ty ->
        if ls.level_new > level then ls.level_new <- level;
        adjust_one acc ty
    | _ -> acc
  and adjust_one acc = function
    | TArrow (_, _, ls) as ty when ls.level_old <= !current_level -> ty :: acc
    | TArrow (_, _, ls) when ls.level_old = ls.level_new -> acc
    | TArrow (ty1, ty2, ls) ->
        let level = ls.level_new in
        ls.level_new <- marked_level;
        let acc = loop acc level ty1 in
        let acc = loop acc level ty2 in
        ls.level_new <- level;
        ls.level_old <- level;
        acc
    | _ -> assert false
  in
  to_be_level_adjusted := List.fold_left adjust_one [] !to_be_level_adjusted

let gen ty =
  force_delayed_adjustments ();
  let rec loop ty =
    match repr ty with
    | TVar ({ contents = Unbound (name, l) } as tvr) when l > !current_level ->
        tvr := Unbound (name, generic_level)
    | TArrow (ty1, ty2, ls) when ls.level_new > !current_level ->
        let ty1 = repr ty1 and ty2 = repr ty2 in
        loop ty1;
        loop ty2;
        let l = max (get_level ty1) (get_level ty2) in
        ls.level_old <- l;
        ls.level_new <- l
    | _ -> ()
  in
  loop ty

let inst ty =
  let rec loop subst = function
    | TVar { contents = Unbound (name, l) } when l = generic_level ->
        begin match List.assoc_opt name subst with
        | Some ty -> (ty, subst)
        | None ->
            let tv = newvar () in
            (tv, (name, tv) :: subst)
        end
    | TVar { contents = Link ty } -> loop subst ty
    | TArrow (ty1, ty2, ls) when ls.level_new = generic_level ->
        let ty1, subst = loop subst ty1 in
        let ty2, subst = loop subst ty2 in
        (new_arrow ty1 ty2, subst)
    | ty -> (ty, subst)
  in
  fst (loop [] ty)

let rec type_of (env : env) = function
  | Var x -> inst (List.assoc x env)
  | Lam (x, e) ->
      let ty_x = newvar () in
      let ty_e = type_of ((x, ty_x) :: env) e in
      new_arrow ty_x ty_e
  | App (e1, e2) ->
      let ty_fun = type_of env e1 in
      let ty_arg = type_of env e2 in
      let ty_res = newvar () in
      unify ty_fun (new_arrow ty_arg ty_res);
      ty_res
  | Let (x, e, e2) ->
      enter_level ();
      let ty_e = type_of env e in
      leave_level ();
      gen ty_e;
      type_of ((x, ty_e) :: env) e2
  | IntLit _ -> TInt
  | BoolLit _ -> TBool

(* Type-check the top-level expresstion*)
let top_type_check exp =
  reset_type_variables ();
  let ty = type_of [] exp in
  cycle_free ty;
  ty
