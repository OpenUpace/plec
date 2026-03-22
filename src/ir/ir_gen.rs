use crate::parse::parser::Expr;
use inkwell::{builder::Builder, context::Context, module::Module, values::BasicValueEnum};

pub trait Visistor<'ctx> {
    fn visit_expr(&mut self, expr: &Expr) -> BasicValueEnum<'ctx>;
}

pub struct IRCodegen<'ctx> {
    pub context: &'ctx Context,
    pub builder: Builder<'ctx>,
    pub module: Module<'ctx>,
}

impl<'ctx> IRCodegen<'ctx> {
    pub fn new(context: &'ctx Context, module_name: &str) -> Self {
        let module = context.create_module(module_name);
        let builder = context.create_builder();

        Self {
            context,
            builder,
            module,
        }
    }
}

impl<'ctx> Visistor<'ctx> for IRCodegen<'ctx> {
    fn visit_expr(&mut self, expr: &Expr) -> BasicValueEnum<'ctx> {
        match expr {
            Expr::Int(n) => self.context.i64_type().const_int(*n as u64, false).into(),
            Expr::BinOp(lhs, op, rhs) => {
                let lhs_val = self.visit_expr(lhs);
                let rhs_val = self.visit_expr(rhs);

                match (lhs_val, rhs_val) {
                    (BasicValueEnum::IntValue(l), BasicValueEnum::IntValue(r)) => {
                        let int_result = match op {
                            '+' => self.builder.build_int_add(l, r, "addtmp"),
                            '-' => self.builder.build_int_sub(l, r, "subtmp"),
                            '*' => self.builder.build_int_mul(l, r, "multmp"),
                            '/' => self.builder.build_int_unsigned_div(l, r, "divtmp"),
                            '%' => self.builder.build_int_unsigned_rem(l, r, "modtmp"),
                            _ => todo!(),
                        };
                        // TODO: Use safe code to refactor.
                        BasicValueEnum::IntValue(int_result.unwrap())
                    }
                    _ => todo!(),
                }
            }
            _ => todo!(),
        }
    }
}
