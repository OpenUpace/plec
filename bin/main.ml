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

open Plec
open Types
open Eval
open Error
open Debug

(* main *)
(* Small sample *)
let () =
  let input = "fun x: int. x" in

  let buf = Sedlexing.Utf8.from_string input in
  let tokenize, lexbuf = Lexer.tokenize buf in
  let result = Parser.term tokenize lexbuf in
  print_endline (string_of_term result)
