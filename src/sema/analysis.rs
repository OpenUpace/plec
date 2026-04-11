use std::collections::HashMap;

use crate::prelude::{
    parser::Expr,
    type_def::{Type, TypeError, TypedExpr},
};

pub trait SemaVisitor {
    fn visit_expr(&mut self, expr: &Expr) -> Option<TypedExpr>;
    fn visit_bin_expr(&mut self, lhs: TypedExpr, rhs: TypedExpr) -> Result<Type, TypeError>;
}

pub struct SemanticAnalysis {
    symbols: HashMap<String, Type>,
}

#[allow(unused_variables)]
impl SemanticAnalysis {
    pub fn new() -> Self {
        Self {
            symbols: HashMap::new(),
        }
    }
    /// Simple Typed Checker Function.
    pub fn check_ty(&self, left: Type, right: Type) -> Result<Type, TypeError> {
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

    pub fn get_type(&mut self, expr: TypedExpr) -> Type {
        match expr {
            TypedExpr::Int { value, ty } => ty,
            TypedExpr::Float { value, ty } => ty,
            TypedExpr::BinOp { lhs, op, rhs, ty } => ty,
        }
    }
}

impl SemaVisitor for SemanticAnalysis {
    fn visit_expr(&mut self, expr: &Expr) -> Option<TypedExpr> {
        match expr.clone() {
            Expr::Int(n) => Some(TypedExpr::Int {
                value: n,
                ty: Type::Int,
            }),
            Expr::Float(n) => Some(TypedExpr::Float {
                value: n,
                ty: Type::Float,
            }),
            Expr::BinOp { lhs, op, rhs } => {
                let lhs_val = self.visit_expr(&lhs)?;
                let rhs_val = self.visit_expr(&rhs)?;

                let ty = match self.visit_bin_expr(lhs_val.clone(), rhs_val.clone()) {
                    Ok(ty) => ty,
                    Err(e) => {
                        println!("error: {:?}", e);
                        None?
                    }
                };

                Some(TypedExpr::BinOp {
                    lhs: Box::new(lhs_val),
                    op,
                    rhs: Box::new(rhs_val),
                    ty,
                })
            }
            _ => todo!(),
        }
    }

    fn visit_bin_expr(&mut self, lhs: TypedExpr, rhs: TypedExpr) -> Result<Type, TypeError> {
        let lhs_ty = self.get_type(lhs);
        let rhs_ty = self.get_type(rhs);

        let result = self.check_ty(lhs_ty, rhs_ty)?;
        Ok(result)
    }
}
