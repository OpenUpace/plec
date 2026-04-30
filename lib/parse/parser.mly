%{
  open Types
%}

%token <int> TINT
%token <string> ID
%token LAMBDA
%token IF THEN ELSE
%token TRUE FALSE
%token INT BOOL ARROW
%token LPAREN RPAREN COLON DOT
%token EOF

%start <Types.term> term

%%
term:
  | e = expr EOF { e }

ty:
  | t = atom_ty { t }
  | t1 = atom_ty; ARROW; t2 = ty { Arrow (t1, t2) }

atom_ty:
  | INT { Int }
  | BOOL { Bool }

expr:
  | e = expr_apply { e }

expr_apply:
  | e = atom_expr { e }
  | e1 = expr_apply; e2 = atom_expr { App (e1, e2) }

atom_expr:
  | LPAREN; e = expr; RPAREN { e }
  | n = ID { Var n }
  | LAMBDA; fn = ID; COLON; t = ty; DOT; body = expr { Lam (fn, t, body) }
  | TRUE { BoolLit true }
  | FALSE { BoolLit false }
  | i = TINT { IntLit i }
  | IF; cond = expr; THEN; when_true = expr; ELSE; when_false = expr { If (cond, when_true, when_false) }
