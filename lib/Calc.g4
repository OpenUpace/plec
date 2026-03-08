grammar Calc;

prog: expr+ EOF;
expr: term (op = ('+' | '-') term)*;

term: factor (op = ('*' | '/') factor)*;

factor: '(' expr ')' | INT;

INT: [0-9]+;
WS: [ \t\r\n]+ -> skip;
COMMENT: '/*' .*? '*/' -> skip;
LINE_COMMENT: '//' ~[\r\n]* -> skip;