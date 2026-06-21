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

type varname = string
type level = int

let generic_level = 100000000
let marked_level = -1
let current_level = ref 1
let enter_level () = incr current_level
let leave_level () = decr current_level

(* AST *)
type binop =
  | Add
  | Sub
  | Mul (* int -> int -> int *)
  | Lt
  | Gt (* int -> int -> bool *)
  | Eq (* 'a -> 'a -> bool *)
  | And
  | Or (* bool -> bool -> bool *)

type exp =
  | Var of varname
  | App of exp * exp (* e1 e2 *)
  | Lam of varname * exp (* fun x -> e *)
  | Let of varname * exp * exp (* let x = e in e2 *)
  | If of exp * exp * exp (* if e then e2 else e3 *)
  | IntLit of int
  | BoolLit of bool
  | BinOp of exp * binop * exp

type typ = TVar of tv ref | TArrow of typ * typ * levels | TInt | TBool
and tv = Unbound of string * level | Link of typ
and levels = { mutable level_old : level; mutable level_new : level }

let counter = ref 0
let to_be_level_adjusted : typ list ref = ref []

let reset_type_variables () =
  counter := 0;
  current_level := 1;
  to_be_level_adjusted := []

let gensym () : string =
  let n = !counter in
  incr counter;
  if n < 26 then String.make 1 (Char.chr (Char.code 'a' + n))
  else "t" ^ string_of_int n

let newvar () : typ = TVar (ref (Unbound (gensym (), !current_level)))

let new_arrow (ty1 : typ) (ty2 : typ) : typ =
  TArrow (ty1, ty2, { level_new = !current_level; level_old = !current_level })

let rec repr = function
  | TVar ({ contents = Link ty } as tv) ->
      let ty = repr ty in
      tv := Link ty;
      ty
  | ty -> ty

let get_level = function
  | TVar { contents = Unbound (_, l) } -> l
  | TArrow (_, _, ls) -> ls.level_new
  | _ -> assert false

let rec cycle_free = function
  | TVar { contents = Unbound _ } -> ()
  | TVar { contents = Link ty } -> cycle_free ty
  | TArrow (_, _, ls) when ls.level_new = marked_level ->
      failwith "occurs check"
  | TArrow (t1, t2, ls) ->
      let level = ls.level_new in
      ls.level_new <- marked_level;
      cycle_free t1;
      cycle_free t2;
      ls.level_new <- level
  | _ -> ()
(* For primitive types, they will not occur the case of recursive types *)
