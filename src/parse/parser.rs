use crate::lex::lexer::Token;
use chumsky::prelude::*;

#[derive(Debug, Clone)]
pub enum BinaryOp {
    Add,
    Sub,
    Mul,
    Div,
    Mod,
}

#[derive(Debug, Clone)]
pub enum Expr {
    // Variable.
    Var(String),

    // Lambda: e.g. fun x -> x + 1
    Lambda {
        params: String,
        body: Box<Expr>,
    },

    // Function Apply: e.g. f x
    Apply {
        func: Box<Expr>,
        arg: Box<Expr>,
    },

    Float(f64),
    // Integer literal.
    Int(isize),

    BinOp {
        lhs: Box<Expr>,
        op: BinaryOp,
        rhs: Box<Expr>,
    },

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

            let ident = select! {Token::Ident(s) => Expr::Var(s)};

            parenthesized.or(integer).or(float_ty).or(ident)
        };

        let ident = select! {Token::Ident(s) => s};

        let lambda = just(Token::Lambda)
            .ignore_then(ident)
            .then_ignore(just(Token::Arrow))
            .then(p.clone())
            .map(|(param, body)| Expr::Lambda {
                params: param,
                body: Box::new(body),
            });

        let atom_expr = choice((atom, lambda));

        let unary = just(Token::Minus)
            .repeated()
            .foldr(atom_expr, |_op, _rhs| Expr::Neg(()));

        let binary_1 = unary.clone().foldl(
            just(Token::Multiply)
                .or(just(Token::Divide))
                .or(just(Token::Modulo))
                .then(unary)
                .repeated(),
            |lhs, (op, rhs)| match op {
                Token::Multiply => Expr::BinOp {
                    lhs: Box::new(lhs),
                    op: BinaryOp::Mul,
                    rhs: Box::new(rhs),
                },
                Token::Divide => Expr::BinOp {
                    lhs: Box::new(lhs),
                    op: BinaryOp::Div,
                    rhs: Box::new(rhs),
                },
                Token::Modulo => Expr::BinOp {
                    lhs: Box::new(lhs),
                    op: BinaryOp::Mod,
                    rhs: Box::new(rhs),
                },
                _ => unreachable!(),
            },
        );

        let binary_2 = binary_1.clone().foldl(
            just(Token::Plus)
                .or(just(Token::Minus))
                .then(binary_1)
                .repeated(),
            |lhs, (op, rhs)| match op {
                Token::Plus => Expr::BinOp {
                    lhs: Box::new(lhs),
                    op: BinaryOp::Add,
                    rhs: Box::new(rhs),
                },
                Token::Minus => Expr::BinOp {
                    lhs: Box::new(lhs),
                    op: BinaryOp::Sub,
                    rhs: Box::new(rhs),
                },
                _ => unreachable!(),
            },
        );

        binary_2
    })
}
