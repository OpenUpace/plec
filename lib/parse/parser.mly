%{
  open Types

  let curry vars body =
    List.fold_right (fun var body -> Lam(var, body)) vars body
%}

%token <int> TINT
%token <string> ID
%token LAMBDA
%token LET IN
%token EQ
%token LPAREN RPAREN SEMICOLON DOT
%token TRUE FALSE
%token EOF

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
  | LET; name = ID; EQ; t1 = expr; IN; t2 = expr; SEMICOLON { Let (name, t1, t2) }
  | i = TINT { IntLit i }
  | TRUE { BoolLit true }
  | FALSE { BoolLit false }
