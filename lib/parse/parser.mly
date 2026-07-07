%{
  open Types

  let curry vars body =
    List.fold_right (fun var body -> Lam(var, body)) vars body

  let let_multiple_parameters xs e1 =
    if xs = [] then e1 else curry xs e1

  let apply_left e =
    match e with
    | [] -> assert false
    | f :: args -> List.fold_left (fun hd tl -> App (hd, tl)) f args
%}

%token <int> TINT
%token <string> ID
%token LAMBDA
%token IF THEN ELSE
%token LET IN ASSIGN
%token EQ
%token PLUS MINUS TIMES LT GT AND OR
%token LPAREN RPAREN SEMICOLON DOT
%token TRUE FALSE
%token EOF

%left     OR
%left     AND
%left     EQ LT GT
%left     PLUS MINUS
%left     TIMES

%start <Types.exp> exp

%%

exp:
  | e = expr EOF { e }

expr:
  | e = binop { e }

binop:
  | e1 = binop OR e2 = binop { BinOp (e1, Or, e2) }
  | e1 = binop AND e2 = binop { BinOp (e1, And, e2) }
  | e1 = binop EQ e2 = binop { BinOp (e1, Eq, e2) }
  | e1 = binop LT e2 = binop { BinOp (e1, Lt, e2) }
  | e1 = binop GT e2 = binop { BinOp (e1, Gt, e2) }
  | e1 = binop PLUS e2 = binop { BinOp (e1, Add, e2) }
  | e1 = binop MINUS e2 = binop { BinOp (e1, Sub, e2) }
  | e1 = binop TIMES e2 = binop { BinOp (e1, Mul, e2) }
  | e = apply_expr { e }

apply_expr:
  | es = nonempty_list(atom) { apply_left es }

atom:
  | LPAREN; e = expr; RPAREN { e }
  | n = ID { Var n }
  | LAMBDA; vars = binders; DOT; body = expr { curry vars body }
  | LET; name = ID; ASSIGN; t1 = expr; IN; t2 = expr { Let (name, t1, t2) }
  | LET; name = ID; xs = binders; ASSIGN; e1 = expr; IN; e2 = expr { Let (name, let_multiple_parameters xs e1, e2) }
  | i = TINT { IntLit i }
  | TRUE { BoolLit true }
  | FALSE { BoolLit false }
  | IF; e1 = expr; THEN; e2 = expr; ELSE; e3 = expr { If (e1, e2, e3) }

binders:
  | x = ID { [x] }
  | x1 = ID; x2 = binders { x1 :: x2 }