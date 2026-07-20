type id = string
type literal = IntLit of int | BoolLit of bool | UnitLit of unit
type binop = Add | Sub | Mul | Gt | Lt

type expr =
  | Lit of literal
  | Var of id
  | BinOp of binop * expr * expr
  | Assign of id * expr (* x = e *)

type stmt =
  | Skip
  | ExprStmt of expr
  | If of expr * stmt * stmt
  | While of expr * stmt
  | Block of stmt list

type prog = stmt

(* Printer *)
let string_of_literal = function
  | IntLit n -> string_of_int n
  | BoolLit b -> string_of_bool b
  | UnitLit u -> "()"

let string_of_binop = function
  | Add -> "+"
  | Sub -> "-"
  | Mul -> "*"
  | Gt -> ">"
  | Lt -> "<"

let rec string_of_expr = function
  | Lit lit -> string_of_literal lit
  | Var id -> id
  | BinOp (op, e1, e2) ->
      string_of_expr e1 ^ " " ^ string_of_binop op ^ " " ^ string_of_expr e2
  | Assign (id, e) -> id ^ " = " ^ string_of_expr e

let rec string_of_stmt = function
  | Skip -> " skip "
  | ExprStmt e -> string_of_expr e
  | If (e, s1, s2) -> string_of_expr e ^ string_of_stmt s1 ^ string_of_stmt s2
  | While (e, s) -> " while " ^ string_of_expr e ^ " do " ^ string_of_stmt s
  | Block ls -> " begin " ^ string_of_stmt_list ls ^ " end "

and string_of_stmt_list lst =
  let elements = List.map string_of_stmt lst in
  "[" ^ String.concat "; " elements ^ "]"
