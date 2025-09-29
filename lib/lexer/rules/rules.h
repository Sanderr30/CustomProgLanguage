#pragma once

#include <unordered_map>

#include <token/token.h>


inline const std::unordered_map<std::string, TokenType> MultiOperators {
    {
        {"==", TokenType::double_eq_}
        , {"!=", TokenType::not_eq_}
        , {"<=", TokenType::less_eq_}
        , {">=", TokenType::greater_eq_}
        , {"+=", TokenType::plus_eq_}
        , {"-=", TokenType::minus_eq_}
        , {"*=", TokenType::star_eq_}
        , {"/=", TokenType::slash_eq_}
        , {"%=", TokenType::percent_eq_}
        , {"^=", TokenType::degree_eq_}
    }
};

inline const std::unordered_map<char, TokenType> SingleOperators
{
    {
        {'+', TokenType::plus_}
        , {'-', TokenType::minus_}
        , {'*', TokenType::star_}
        , {'/', TokenType::slash_}
        , {'%', TokenType::percent_}
        , {'^', TokenType::degree_}
        , {'=', TokenType::assign_}
        , {'<', TokenType::less_}
        , {'>', TokenType::greater_}
        , {'(', TokenType::l_paren_}
        , {')', TokenType::r_paren_}
        , {'[', TokenType::l_bracket_}
        , {']', TokenType::r_bracket_}
        , {',', TokenType::comma_}
        , {':', TokenType::colon_}
    }
};

inline const std::unordered_map<std::string, TokenType> KeyWords
{
    {
        {"nil", TokenType::nil_}
        , {"true", TokenType::boolean_}
        , {"false", TokenType::boolean_}
        , {"if", TokenType::if_}
        , {"then", TokenType::then_}
        , {"else", TokenType::else_}
        , {"end", TokenType::end_}
        , {"while", TokenType::while_}
        , {"for", TokenType::for_}
        , {"in", TokenType::in_}
        , {"function", TokenType::function_}
        , {"return", TokenType::return_}
        , {"and", TokenType::and_}
        , {"or", TokenType::or_}
        , {"not", TokenType::not_}
        , {"break", TokenType::break_}
        , {"continue", TokenType::continue_}
    }
};
