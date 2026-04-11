use rustyline::error::ReadlineError;
use rustyline::{DefaultEditor, Result};
use std::path::Path;

use crate::prelude::{analysis::*, ir_gen::*, lexer::*, parser::*};
use chumsky::Parser;
use inkwell::context::Context;
use inkwell::targets::{FileType, Target, TargetMachine};
use logos::Logos;

pub enum CompileStates {
    Normal,
    Debug,
    Assmebly,
}

pub fn repl() -> Result<()> {
    // `()` can be used when no completer is required
    let mut rl = DefaultEditor::new()?;
    if rl.load_history("history.txt").is_err() {
        println!("No previous history.");
    }
    loop {
        let readline = rl.readline(">> ");
        match readline {
            Ok(line) => {
                rl.add_history_entry(line.as_str())?;
                run(line, CompileStates::Debug);
            }
            Err(ReadlineError::Interrupted) => {
                println!("CTRL-C");
                break;
            }
            Err(ReadlineError::Eof) => {
                println!("CTRL-D");
                break;
            }
            Err(err) => {
                println!("Error: {:?}", err);
                break;
            }
        }
    }
    let _ = rl.save_history("history.txt");
    Ok(())
}

pub fn run(input: String, state: CompileStates) {
    //reads the input expression from the repl.
    let input = input;

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

    let mut sema = SemanticAnalysis::new();
    let res = match sema.visit_expr(&ast) {
        Some(val) => val,
        None => {
            return;
        }
    };

    let context = Context::create();

    let mut codegen = IRGenerator::new(&context, "main");

    let i64_type = context.i64_type();
    let fn_type = i64_type.fn_type(&[], false);
    let main_fn = codegen.module.add_function("main", fn_type, None);
    let entry = context.append_basic_block(main_fn, "entry");
    codegen.builder.position_at_end(entry);

    let result = codegen.visit_expr(&res);

    let _ = codegen.builder.build_return(Some(&result));

    Target::initialize_native(&Default::default()).expect("Cannot initialize target.");

    let target_triple = TargetMachine::get_default_triple();

    let target = Target::from_triple(&target_triple).unwrap();

    let target_machine = target
        .create_target_machine(
            &target_triple,
            "generic",
            "",
            inkwell::OptimizationLevel::Default,
            inkwell::targets::RelocMode::Default,
            inkwell::targets::CodeModel::Default,
        )
        .unwrap();

    match state {
        CompileStates::Normal => (),
        CompileStates::Debug => codegen.module.print_to_stderr(),
        CompileStates::Assmebly => target_machine
            .write_to_file(&codegen.module, FileType::Assembly, Path::new("output.s"))
            .unwrap(),
    };
}
