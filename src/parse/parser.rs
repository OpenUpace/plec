use crate::lex::lexer::Token;
use chumsky::prelude::*;

#[derive(Debug, Clone)]
pub enum Expr {
    Float(f64),
    // Integer literal.
    Int(isize),

    BinOp(Box<Expr>, char, Box<Expr>),

    // Unary minus.
    // Note: Don't use it now.
    Neg(()),
}

/// Parser:
/// A simple parse function in chumsky.
/// Note: Just arithmetic operations now.
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

            let float_ty = select! {Token::Float(n) => Expr::Float(n)};

            parenthesized.or(integer).or(float_ty)
        };

        let unary = just(Token::Minus)
            .repeated()
            .foldr(atom, |_op, _rhs| Expr::Neg(()));

        let binary_1 = unary.clone().foldl(
            just(Token::Multiply)
                .or(just(Token::Divide))
                .or(just(Token::Modulo))
                .then(unary)
                .repeated(),
            |lhs, (op, rhs)| match op {
                Token::Multiply => Expr::BinOp(Box::new(lhs), '*', Box::new(rhs)),
                Token::Divide => Expr::BinOp(Box::new(lhs), '/', Box::new(rhs)),
                Token::Modulo => Expr::BinOp(Box::new(lhs), '%', Box::new(rhs)),
                _ => unreachable!(),
            },
        );

        let binary_2 = binary_1.clone().foldl(
            just(Token::Plus)
                .or(just(Token::Minus))
                .then(binary_1)
                .repeated(),
            |lhs, (op, rhs)| match op {
                Token::Plus => Expr::BinOp(Box::new(lhs), '+', Box::new(rhs)),
                Token::Minus => Expr::BinOp(Box::new(lhs), '-', Box::new(rhs)),
                _ => unreachable!(),
            },
        );

        binary_2
    })
}
