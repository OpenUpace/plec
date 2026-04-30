let digit = [%sedlex.regexp? '0' .. '9']
let alpha = [%sedlex.regexp? 'a' .. 'z' | 'A' .. 'Z']
let id = [%sedlex.regexp? alpha | Star (alpha | digit | '_')]

let rec token buf =
  match%sedlex buf with
  | Plus digit -> Parser.TINT (int_of_string (Sedlexing.Utf8.lexeme buf))
  | "fun" -> Parser.LAMBDA
  | "if" -> Parser.IF
  | "then" -> Parser.THEN
  | "else" -> Parser.ELSE
  | "true" -> Parser.TRUE
  | "false" -> Parser.FALSE
  | "int" -> Parser.INT
  | "bool" -> Parser.BOOL
  | "->" -> Parser.ARROW
  | '(' -> Parser.LPAREN
  | ')' -> Parser.RPAREN
  | ':' -> Parser.COLON
  | '.' -> Parser.DOT
  | Plus white_space -> token buf
  | id -> Parser.ID (Sedlexing.Utf8.lexeme buf)
  | eof -> Parser.EOF
  | _ -> failwith ("Unexpected character: " ^ Sedlexing.Utf8.lexeme buf)

(* Approach 1: classic API — wrap into [Lexing.lexbuf -> token] *)
let tokenize buf =
  let lexing_lexbuf = Lexing.from_string "" in
  let tokenize _lexing_lexbuf =
    let tok = token buf in
    let start_pos, end_pos = Sedlexing.lexing_positions buf in
    _lexing_lexbuf.Lexing.lex_start_p <- start_pos;
    _lexing_lexbuf.Lexing.lex_curr_p <- end_pos;
    tok
  in
  (tokenize, lexing_lexbuf)

(* Approach 2: incremental API — use [Sedlexing.with_tokenizer] *)
let tokenize_incremental buf = Sedlexing.with_tokenizer token buf
