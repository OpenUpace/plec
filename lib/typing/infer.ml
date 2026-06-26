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

let update_level : level -> typ -> unit =
 fun l -> function
  | TVar ({ contents = Unbound (n, l') } as tvr) ->
      assert (not (l' = generic_level));
      if l < l' then tvr := Unbound (n, l)
  | TArrow (_, _, ls) as ty ->
      assert (not (ls.level_new = generic_level));
      if ls.level_new = marked_level then failwith "occurs check";
      if l < ls.level_new then begin
        if ls.level_new = ls.level_old then
          to_be_level_adjusted := ty :: !to_be_level_adjusted;
        ls.level_new <- 1
      end
  | _ -> ()

let rec unify (t1 : typ) (t2 : typ) : unit =
  if t1 == t2 then ()
  else
    match (t1, t2) with
    | ( (TVar ({ contents = Unbound (_, l1) } as tv1) as t1),
        (TVar ({ contents = Unbound (_, l2) } as tv2) as t2) ) ->
        if tv1 == tv2 then ()
        else if l1 > l2 then tv1 := Link t2
        else tv2 := Link t1
    | TVar ({ contents = Unbound (_, l) } as tv), t'
    | t', TVar ({ contents = Unbound (_, l) } as tv) ->
        update_level l t';
        tv := Link t'
    | TArrow (tyl1, tyl2, ll), TArrow (tyr1, tyr2, lr) ->
        if ll.level_new = marked_level || lr.level_new = marked_level then
          failwith "cycle: occurs check";
        let min_level = min ll.level_new lr.level_new in
        ll.level_new <- marked_level;
        lr.level_new <- marked_level;
        unify_lev min_level tyl1 tyl2;
        unify_lev min_level tyr1 tyr2;
        ll.level_new <- min_level;
        lr.level_new <- min_level
    | TInt, TInt | TBool, TBool -> ()
    | _ ->
        Printf.eprintf "cannot unify type %s with type %s\n" (string_of_ty t1)
          (string_of_ty t2)

and unify_lev l ty1 ty2 =
  let ty1 = repr ty1 in
  update_level l ty1;
  unify ty1 ty2
