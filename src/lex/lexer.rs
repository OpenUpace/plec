use logos::Logos;

#[derive(Logos, Debug, PartialEq, Clone)]
#[logos(skip r"[ \t\n]+")]
#[logos(skip r"//[^\n\r]*?")]
#[logos(skip r"/\*([^*]|\*[^/])*\*/")]
#[logos(error = String)]
pub enum Token {
    #[token("fun")]
    Lambda,

    #[token("+")]
    Plus,

    #[token("-")]
    Minus,

    #[token("*")]
    Multiply,

    #[token("/")]
    Divide,

    #[token("%")]
    Modulo,

    #[token("(")]
    LParen,

    #[token(")")]
    RParen,

    #[token("->")]
    Arrow,

    #[regex(r"[0-9]+\.[0-9]*|\.[0-9]+", |lex| lex.slice().parse::<f64>().unwrap())]
    Float(f64),

    #[regex("[0-9]+", |lex| lex.slice().parse::<isize>().unwrap())]
    Integer(isize),

    #[regex("[a-zA-Z_][a-zA-Z0-9_]*", |lex| lex.slice().to_string())]
    Ident(String),
}
