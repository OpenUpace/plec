pub mod basic;
pub mod ir;
pub mod lex;
pub mod parse;
pub mod repl;
pub mod sema;

pub mod prelude {
    pub use crate::basic::*;
    pub use crate::ir::*;
    pub use crate::lex::*;
    pub use crate::parse::*;
    pub use crate::repl::*;
    pub use crate::sema::*;
}
