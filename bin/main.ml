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
open Ast

(* Main *)
let usage_msg = "plec [--verbose] <file>"
let verbose = ref false
let filename = ref None

let speclist =
  [ ("--verbose", Arg.Set verbose, "Display this list of options") ]

let anon_fun s =
  match !filename with
  | None -> filename := Some s
  | Some _ ->
      Printf.eprintf "Error: See: \n%s\n" usage_msg;
      exit 1

let read_file_to_string filename =
  In_channel.with_open_text filename (fun ch -> In_channel.input_all ch)

let main filename =
  try
    let content = read_file_to_string filename in
    let buf = Sedlexing.Utf8.from_string content in
    let tokenize, lexbuf = Lexer.tokenize buf in
    let result = Parser.prog tokenize lexbuf in
    print_endline (string_of_stmt result)
  with Sys_error msg -> Printf.printf "Error: %s\n" msg

let () =
  Arg.parse speclist anon_fun usage_msg;

  let filename =
    match !filename with
    | Some f -> f
    | None ->
        Printf.eprintf "Error: See: \n%s\n" usage_msg;
        exit 1
  in

  main filename
