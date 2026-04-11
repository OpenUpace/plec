use crate::{
    prelude::parser::BinaryOp,
    prelude::type_def::{Type, TypedExpr},
};
use inkwell::{
    builder::Builder,
    context::Context,
    module::Module,
    values::{BasicValueEnum, FloatValue},
};

/// Visitor: Simple trait to impl.
pub trait IRGeneratorVisistor<'ctx> {
    fn visit_expr(&mut self, expr: &TypedExpr) -> BasicValueEnum<'ctx>;
}

/// LLVM IR Codegen.
pub struct IRGenerator<'ctx> {
    pub context: &'ctx Context,
    pub builder: Builder<'ctx>,
    pub module: Module<'ctx>,
}

impl<'ctx> IRGenerator<'ctx> {
    pub fn new(context: &'ctx Context, module_name: &str) -> Self {
        let module = context.create_module(module_name);
        let builder = context.create_builder();

        Self {
            context,
            builder,
            module,
        }
    }

    fn to_float_value(&self, val: BasicValueEnum<'ctx>) -> FloatValue<'ctx> {
        match val {
            BasicValueEnum::IntValue(int_val) => self
                .builder
                .build_signed_int_to_float(int_val, self.context.f64_type(), "int2float")
                .expect("fail!"),
            BasicValueEnum::FloatValue(float_val) => float_val,
            _ => panic!("Not number type"),
        }
    }
}

impl<'ctx> IRGeneratorVisistor<'ctx> for IRGenerator<'ctx> {
    fn visit_expr(&mut self, expr: &TypedExpr) -> BasicValueEnum<'ctx> {
        match expr {
            TypedExpr::Int {
                value,
                ty: Type::Int,
            } => self
                .context
                .i64_type()
                .const_int(*value as u64, false)
                .into(),
            TypedExpr::Float {
                value,
                ty: Type::Float,
            } => self.context.f64_type().const_float(*value as f64).into(),
            TypedExpr::BinOp { lhs, op, rhs, ty } => {
                let lhs_val = self.visit_expr(lhs);
                let rhs_val = self.visit_expr(rhs);

                match ty {
                    Type::Int => {
                        let l = lhs_val.into_int_value();
                        let r = rhs_val.into_int_value();
                        let result = match op {
                            BinaryOp::Add => self.builder.build_int_add(l, r, "addtmp"),
                            BinaryOp::Sub => self.builder.build_int_sub(l, r, "subtmp"),
                            BinaryOp::Mul => self.builder.build_int_mul(l, r, "multmp"),
                            BinaryOp::Div => self.builder.build_int_unsigned_div(l, r, "divtmp"),
                            BinaryOp::Mod => self.builder.build_int_unsigned_rem(l, r, "modtmp"),
                        };
                        // TODO: Use safe code to refactor.
                        return BasicValueEnum::IntValue(result.unwrap());
                    }
                    Type::Float => {
                        let l = self.to_float_value(lhs_val);
                        let r = self.to_float_value(rhs_val);
                        let result = match op {
                            BinaryOp::Add => self.builder.build_float_add(l, r, "addtmp"),
                            BinaryOp::Sub => self.builder.build_float_sub(l, r, "subtmp"),
                            BinaryOp::Mul => self.builder.build_float_mul(l, r, "multmp"),
                            BinaryOp::Div => self.builder.build_float_div(l, r, "divtmp"),
                            BinaryOp::Mod => self.builder.build_float_rem(l, r, "modtmp"),
                        };
                        return BasicValueEnum::FloatValue(result.unwrap());
                    }
                    _ => todo!(),
                }
            }
            _ => todo!(),
        }
    }
}
