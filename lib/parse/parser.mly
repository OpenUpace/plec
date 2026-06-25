%{
  open Types

  let curry vars body =
    List.fold_right (fun var body -> Lam(var, body)) vars body
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
%nonassoc LT GT EQ
%left     PLUS MINUS
%left     TIMES

%start <Types.exp> exp

%%
exp:
  | e = expr EOF { e }

expr:
  | e = expr_apply { e }

expr_apply:
  | e = atom_expr { e }
  | e1 = expr_apply; e2 = atom_expr { App (e1, e2) }

binders:
  | x = ID { [x] }
  | x1 = ID; x2 = binders { x1 :: x2 }

atom_expr:
  | LPAREN; e = expr; RPAREN { e }
  | n = ID { Var n }
  | LAMBDA; vars = binders; DOT; body = expr { curry vars body }
  | LET; name = ID; ASSIGN; t1 = expr; IN; t2 = expr { Let (name, t1, t2) }
  | i = TINT { IntLit i }
  | TRUE { BoolLit true }
  | FALSE { BoolLit false }
  | IF; e1 = expr; THEN; e2 = expr; ELSE; e3 = expr { If (e1, e2, e3) }
  (* hack *)
  | expr EQ expr { BinOp ($1, Eq, $3)}
  | expr AND expr { BinOp ($1, And, $3)}
  | expr OR expr { BinOp ($1, Or, $3)}
  | expr LT expr { BinOp ($1, Lt, $3)}
  | expr GT expr { BinOp ($1, Gt, $3)}
  | expr PLUS expr { BinOp ($1, Add, $3)}
  | expr MINUS expr { BinOp ($1, Sub, $3)}
  | expr TIMES expr { BinOp ($1, Mul, $3)}

