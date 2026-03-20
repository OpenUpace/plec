use logos::Logos;

#[derive(Logos, Debug, PartialEq, Eq, Hash, Clone)]
#[logos(skip r"[ \t\n]+")]
#[logos(skip r"//[^\n\r]*?")]
#[logos(skip r"/\*([^*]|\*[^/])*\*/")]
#[logos(error = String)]
pub enum Token {
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

    #[regex("[0-9]+", |lex| lex.slice().parse::<isize>().unwrap())]
    Integer(isize),
}
