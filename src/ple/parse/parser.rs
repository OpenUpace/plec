use crate::Token;
use chumsky::prelude::*;

#[derive(Debug)]
pub enum Expr {
    // Integer literal.
    Int(isize),

    // Unary minus.
    Neg(Box<Expr>),

    // Binary operators.
    Add(Box<Expr>, Box<Expr>),
    Sub(Box<Expr>, Box<Expr>),
    Mul(Box<Expr>, Box<Expr>),
    Div(Box<Expr>, Box<Expr>),
    Mod(Box<Expr>, Box<Expr>),
}

pub fn parser<'src>()
-> impl Parser<'src, &'src [Token], Expr, chumsky::extra::Err<chumsky::error::Simple<'src, Token>>>
{
    recursive(|p| {
        let atom = {
            let parenthesized = p
                .clone()
                .delimited_by(just(Token::LParen), just(Token::RParen));

            let integer = select! {
                Token::Integer(n) => Expr::Int(n),
            };

            parenthesized.or(integer)
        };

        let unary = just(Token::Minus)
            .repeated()
            .foldr(atom, |_op, rhs| Expr::Neg(Box::new(rhs)));

        let binary_1 = unary.clone().foldl(
            just(Token::Multiply)
                .or(just(Token::Divide))
                .or(just(Token::Modulo))
                .then(unary)
                .repeated(),
            |lhs, (op, rhs)| match op {
                Token::Multiply => Expr::Mul(Box::new(lhs), Box::new(rhs)),
                Token::Divide => Expr::Div(Box::new(lhs), Box::new(rhs)),
                Token::Modulo => Expr::Mod(Box::new(lhs), Box::new(rhs)),
                _ => unreachable!(),
            },
        );

        let binary_2 = binary_1.clone().foldl(
            just(Token::Plus)
                .or(just(Token::Minus))
                .then(binary_1)
                .repeated(),
            |lhs, (op, rhs)| match op {
                Token::Plus => Expr::Add(Box::new(lhs), Box::new(rhs)),
                Token::Minus => Expr::Sub(Box::new(lhs), Box::new(rhs)),
                _ => unreachable!(),
            },
        );

        binary_2
    })
}

impl Expr {
    pub fn eval(&self) -> isize {
        match self {
            Expr::Int(n) => *n,
            Expr::Neg(rhs) => -rhs.eval(),
            Expr::Add(lhs, rhs) => lhs.eval() + rhs.eval(),
            Expr::Sub(lhs, rhs) => lhs.eval() - rhs.eval(),
            Expr::Mul(lhs, rhs) => lhs.eval() * rhs.eval(),
            Expr::Div(lhs, rhs) => lhs.eval() / rhs.eval(),
            Expr::Mod(lhs, rhs) => lhs.eval() % rhs.eval(),
        }
    }
}
