pub enum TokenType {
    Unknown,
    Eof,
    Identifier(String),
    Number(String),
    Func,
}

pub struct Token {
    tok: TokenType,
    ident: Vec<char>,
    num_val: f64,
    position: usize,
}

pub struct Lexer {
    cur_char: Token,
}

impl Lexer {
    pub fn new(input: &str) -> Self {
        Self {
            cur_char: Token {
                tok: TokenType::Unknown,
                ident: input.chars().collect(),
                num_val: 0.0,
                position: 0,
            },
        }
    }

    pub fn next_tok(&mut self) -> TokenType {
        self.skip_whitespace();

        if self.is_eof() {
            return TokenType::Eof;
        }

        let ch = self.current_char();

        match ch {
            'a'..='z' | 'A'..='Z' | '_' => self.read_identifier(),
            '0'..='9' => self.read_number(),

            _ => panic!("Unknown identifier: {}", ch),
        }
    }

    fn read_identifier(&mut self) -> TokenType {
        let start = self.cur_char.position;
        while !self.is_eof() && self.current_char().is_alphanumeric() {
            self.advance();
        }

        let ident: String = self.cur_char.ident[start..self.cur_char.position]
            .iter()
            .collect();

        println!("IdentifierStr: {}", ident);

        match ident.as_str() {
            "func" => TokenType::Func,
            _ => return TokenType::Identifier(ident),
        }
    }

    fn read_number(&mut self) -> TokenType {
        let start = self.cur_char.position;
        while !self.is_eof() && self.current_char().is_digit(10) {
            self.advance();
        }

        if !self.is_eof() && self.current_char() == '.' {
            self.advance();
            while !self.is_eof() && self.current_char().is_digit(10) {
                self.advance();
            }
        }

        let num_str: String = self.cur_char.ident[start..self.cur_char.position]
            .iter()
            .collect();
        self.cur_char.num_val = num_str.parse::<f64>().unwrap();
        println!("NumVal: {}", self.cur_char.num_val);
        TokenType::Number(self.cur_char.num_val.to_string())
    }

    fn current_char(&self) -> char {
        self.cur_char.ident[self.cur_char.position]
    }

    fn advance(&mut self) {
        self.cur_char.position += 1
    }

    fn is_eof(&self) -> bool {
        self.cur_char.position >= self.cur_char.ident.len()
    }

    fn skip_whitespace(&mut self) {
        while !self.is_eof() && self.current_char().is_whitespace() {
            self.advance()
        }
    }
}
