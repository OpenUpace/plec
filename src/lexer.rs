pub enum Token {
    Unknown,
    Eof,
    Identifier(String),
    Number(String),
    Func,
}

pub struct Lexer {
    input: Vec<char>,
    pos: usize,
}

impl Lexer {
    pub fn new(input: &str) -> Self {
        Self {
            input: input.chars().collect(),
            pos: 0,
        }
    }

    pub fn next_tok(&mut self) -> Token {
        self.skip_whitespace();

        if self.is_eof() {
            return Token::Eof;
        }

        let ch = self.current_char();

        match ch {
            'a'..='z' | 'A'..='Z' | '_' => self.read_identifier(),
            '0'..='9' => self.read_number(),

            _ => panic!("Unknown identifier: {}", ch),
        }
    }

    fn read_identifier(&mut self) -> Token {
        let start = self.pos;
        while !self.is_eof() && self.current_char().is_alphanumeric() {
            self.advance();
        }

        let ident: String = self.input[start..self.pos].iter().collect();

        println!("IdentifierStr: {}", ident);

        match ident.as_str() {
            "func" => return Token::Func,
            _ => return Token::Identifier(ident),
        }
    }

    fn read_number(&mut self) -> Token {
        let start = self.pos;
        while !self.is_eof() && self.current_char().is_digit(10) {
            self.advance();
        }

        if !self.is_eof() && self.current_char() == '.' {
            self.advance();
            while !self.is_eof() && self.current_char().is_digit(10) {
                self.advance();
            }
        }

        let num_str: String = self.input[start..self.pos].iter().collect();
        let num = num_str.parse::<f64>().unwrap();
        println!("NumVal: {}", num);
        Token::Number(num.to_string())
    }

    fn current_char(&self) -> char {
        self.input[self.pos]
    }

    fn advance(&mut self) {
        self.pos += 1
    }

    fn is_eof(&self) -> bool {
        self.pos >= self.input.len()
    }

    fn skip_whitespace(&mut self) {
        while !self.is_eof() && self.current_char().is_whitespace() {
            self.advance()
        }
    }
}
