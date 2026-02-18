mod lexer;

fn main() {
    let mut l = lexer::Lexer::new("func");
    l.next_tok();
}
