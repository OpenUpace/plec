grammar Calc;

prog: expr+ EOF;
expr: term (('+' | '-') term)*;

term: factor (('*' | '/') factor)*;

factor: '(' expr ')' | INT;

INT: [0-9]+;
WS: [ \t\r\n]+ -> skip;
COMMENT: '/*' .*? '*/' -> skip;
LINE_COMMENT: '//' ~[\r\n]* -> skip;