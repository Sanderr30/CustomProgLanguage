#include "token.h"


Token::Token() = default;


Token::Token(TokenType tok, std::string lex
            , std::size_t ln, std::size_t col) 
      : type(tok)
      , lexeme(std::move(lex))
      , line(ln)
      , column(col) 
{}