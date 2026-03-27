pub mod basic;
pub mod ir;
pub mod lex;
pub mod parse;
pub mod sema;

pub mod prelude {
    pub use crate::basic::type_def::*;
    pub use crate::ir::ir_gen::*;
    pub use crate::lex::lexer::*;
    pub use crate::parse::parser::*;
    pub use crate::sema::analysis::*;
}
