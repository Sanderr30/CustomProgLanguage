#pragma once

#include <string>
#include <cstddef>


enum class TokenType {
    end_of_file_, identifier_, number_,

    string_, boolean_, nil_,

    if_, then_, else_,

    end_, while_, for_,

    in_, function_, return_,

    and_, or_, not_,

    break_, continue_, plus_,

    minus_, star_, slash_,

    percent_, degree_, assign_,

    plus_eq_, minus_eq_, star_eq_,

    slash_eq_, percent_eq_, degree_eq_,

    double_eq_, not_eq_, less_,

    less_eq_, greater_, greater_eq_,

    l_paren_, r_paren_, l_bracket_,

    r_bracket_, comma_, colon_
};

struct Token {
    Token(TokenType, std::string
        , std::size_t, std::size_t);

    Token();

    TokenType type;
    std::string lexeme;
    std::size_t line;
    std::size_t column;
};
