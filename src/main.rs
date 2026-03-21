mod ple;
use chumsky::Parser;
use inkwell::context::Context;
use logos::Logos;
use ple::ir::ir_gen::Visistor;
use ple::lex::lexer::Token;
use ple::parse::parser::parser;
use std::env;

use crate::ple::ir::ir_gen::IRCodegen;

fn main() {
    //reads the input expression from the command line
    let input = env::args()
        .nth(1)
        .expect("Expected expression argument (e.g. `1 + 7 * (3 - 4) / 5`)");

    //creates a lexer instance from the input
    let lexer = Token::lexer(&input);

    //splits the input into tokens, using the lexer
    let mut tokens = vec![];
    for (token, span) in lexer.spanned() {
        match token {
            Ok(token) => tokens.push(token),
            Err(e) => {
                println!("lexer error at {:?}: {}", span, e);
                return;
            }
        }
    }

    //parses the tokens to construct an AST
    let ast = match parser().parse(&tokens).into_result() {
        Ok(expr) => {
            println!("[AST]\n{:#?}", expr);
            expr
        }
        Err(e) => {
            println!("parse error: {:#?}", e);
            return;
        }
    };

    let context = Context::create();

    let mut codegen = IRCodegen::new(&context, "main");

    let i64_type = context.i64_type();
    let fn_type = i64_type.fn_type(&[], false);
    let main_fn = codegen.module.add_function("main", fn_type, None);
    let entry = context.append_basic_block(main_fn, "entry");
    codegen.builder.position_at_end(entry);

    let result = codegen.visit_expr(&ast);

    let _ = codegen.builder.build_return(Some(&result));

    codegen.module.print_to_stderr();
}
