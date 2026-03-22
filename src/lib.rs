pub mod ir;
pub mod lex;
pub mod parse;

pub mod prelude {
    pub use crate::ir::ir_gen::*;
    pub use crate::lex::lexer::*;
    pub use crate::parse::parser::*;
}
