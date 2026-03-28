#[derive(Debug, Clone)]
pub enum Type {
    Int,
    Bool,
    Float,
}

#[derive(thiserror::Error, Debug)]
pub enum TypeError {
    #[error("expected '{lhs:?}', found '{rhs:?}'.")]
    InvalidType { lhs: Type, rhs: Type },
}

#[derive(Debug, Clone)]
pub enum TypedExpr {
    Int {
        value: isize,
        ty: Type,
    },
    Float {
        value: f64,
        ty: Type,
    },
    BinOp {
        lhs: Box<TypedExpr>,
        op: char,
        rhs: Box<TypedExpr>,
        ty: Type,
    },
}
