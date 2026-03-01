use crate::lexer;

pub struct PrototypeAST;

pub enum ExprAST {
    NumberExprAST(f32),
    VariableExprAST(String),
    BinaryExprAST(Box<ExprAST>, char, Box<ExprAST>),
    CallExprAST(String, Vec<Box<ExprAST>>),
    PrototypeAST(String, Vec<String>),
    FunctionAST(Box<PrototypeAST>, Box<ExprAST>),
}

pub struct Parser {
    lexer: lexer::Lexer,
    tok: lexer::Token,
}

impl Parser {}
