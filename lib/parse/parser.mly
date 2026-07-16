%{
  open Ast
%}

%token <int> INT
%token <string> ID
%token EOF
%token PLUS MINUS TIMES GT LT
%token ASSIGN
%token SKIP
%token TRUE FALSE
%token IF THEN ELSE
%token WHILE DO
%token BEGIN END
%token LPAREN RPAREN SEMICOLON

%right ASSIGN
%nonassoc GT LT
%left PLUS MINUS
%left TIMES

%start <Ast.prog> prog

%%
prog:
  | s = stmt EOF { s }

stmt:
  | e = expr SEMICOLON { ExprStmt e }
  | SKIP { Skip }
  | IF e = expr THEN s1 = stmt ELSE s2 = stmt { If (e, s1, s2) }
  | WHILE e = expr DO s = stmt { While (e, s) }
  | BEGIN lst = list(stmt) END { Block (lst) }

expr:
  | LPAREN e = expr RPAREN { e }
  | lit = literal { Lit lit }
  | id = ID { Var id }
  | e1 = expr PLUS e2 = expr { BinOp (Add, e1, e2) }
  | e1 = expr MINUS e2 = expr { BinOp (Sub, e1, e2) }
  | e1 = expr TIMES e2 = expr { BinOp (Mul, e1, e2) }
  | e1 = expr GT e2 = expr { BinOp (Gt, e1, e2) }
  | e1 = expr LT e2 = expr { BinOp (Lt, e1, e2) }
  | id = ID ASSIGN e = expr { Assign (id, e) }

literal:
  | n = INT { IntLit n }
  | TRUE { BoolLit true }
  | FALSE { BoolLit false }
  | LPAREN RPAREN { UnitLit () }
