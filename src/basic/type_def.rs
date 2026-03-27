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

/// Simple Typed Checker Function.
pub fn check_ty(left: Type, right: Type) -> Result<Type, TypeError> {
    match (left.clone(), right.clone()) {
        (Type::Int, Type::Int) => Ok(Type::Int),
        (Type::Float, Type::Float) | (Type::Float, Type::Int) | (Type::Int, Type::Float) => {
            Ok(Type::Float)
        }
        _ => Err(TypeError::InvalidType {
            lhs: left,
            rhs: right,
        }),
    }
}
