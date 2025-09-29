#include <string>

#include <lexer.h>
#include <errors/lex_errors.h>


Lexer::Lexer(std::istream& input)
    : input_code_(input)
{
    next_token_ = ScanNextTokenImpl();
}


const Token& Lexer::GetCurrentToken() const { return next_token_; }


Token Lexer::ScanNextToken() {
    Token cur = next_token_;
    next_token_ = ScanNextTokenImpl();
    return cur;
}


void Lexer::Update() {
    if (current_ == '\n') {
        ++line_;
        column_ = 0;
    } else {
        ++column_;
    }
    current_ = input_code_.get();
}


void Lexer::SkipUnnecessary() {
    while (std::isspace(current_)
            || (current_ == '/'
                && input_code_.peek() == '/'))
    {
        if (current_ == '/') {
            while (current_ != EOF && current_ != '\n') {
                Update();
            }
        } else {
            Update();
        }
    }
}


Token Lexer::ScanNextTokenImpl() {
    SkipUnnecessary();

    if (current_ == EOF) {
        return Token{TokenType::end_of_file_
                        , ""
                        , line_
                        , column_};
    }

    char curr = static_cast<char>(current_);
    char nxt = static_cast<char>(input_code_.peek());

    std::string long_operator{curr, nxt};

    if (auto it = MultiOperators.find(long_operator); it != MultiOperators.end()) {
        Update();
        Update();
        return Token{it->second, long_operator, line_, column_ - 2};
    }
    if (auto it = SingleOperators.find(curr); it != SingleOperators.end()) {
        Update();
        return Token{it->second, std::string(1, curr), line_, column_ - 1};
    }

    if (std::isalpha(current_) || current_ == '_') {
        return ScanIdentifier();
    }
    if (std::isdigit(current_) || current_ == '.') {
        return ScanNumber();
    }
    if (current_ == '"') {
        return ScanString();
    }

    const std::string s(1, static_cast<char>(current_));
    Token error_token {TokenType::identifier_, s, line_, column_ };
    throw LexerError {error_token, LexerError::kUnexpectedChar };
}


Token Lexer::ScanIdentifier() {
    std::string buffer;

    while (std::isalnum(current_) || current_ == '_') {
        buffer.push_back(static_cast<char>(current_));
        Update();
    }

    if (auto it = KeyWords.find(buffer); it != KeyWords.end()) {
        return Token{it->second, buffer, line_, column_};
    }

    return Token{TokenType::identifier_, buffer, line_, column_};
}


Token Lexer::ScanNumber() {
    std::string buffer;
    bool dot = false;

    while (std::isdigit(current_) || (!dot && current_ == '.')) {
        if (current_ == '.') dot = true;
        buffer.push_back(static_cast<char>(current_));
        Update();
    }

    if (buffer.empty()) {
        throw LexerError{Token{TokenType::number_, ""
                    , line_, column_}, LexerError::kEmptyNumberLit};
    }
    if (buffer == ".") {
        throw LexerError{Token{TokenType::number_, "."
                    , line_, column_}, LexerError::kEmptyNumberLit};
    }
    if (buffer.back() == '.') {
        throw LexerError{Token{TokenType::number_, buffer
                    , line_, column_}, LexerError::kInvalidTrailingSymb + buffer};
    }

    if (current_ == 'e' || current_ == 'E') {
        buffer.push_back(static_cast<char>(current_));
        Update();

        if (current_ == '+' || current_ == '-') {
            buffer.push_back(static_cast<char>(current_));
            Update();
        }

        if (!std::isdigit(current_)) {
            throw LexerError{Token{TokenType::number_, buffer
                    , line_, column_}, LexerError::kInvalidExponent + buffer};
        }

        while (std::isdigit(current_)) {
            buffer.push_back(static_cast<char>(current_));
            Update();
        }
    }
    return Token{TokenType::number_, buffer, line_, column_};
}


Token Lexer::ScanString() {
    Update();
    std::string buffer;
    while (current_ != '"' && current_ != EOF) {
        if (current_ == '\\') {
            Update();

            if (current_ == EOF) {
                throw LexerError{Token{TokenType::string_, buffer
                            , line_, column_}, LexerError::kUntermSequence};
            }

            char e = static_cast<char>(current_);
            buffer += (e == 'n' ? '\n' : e == 't' ? '\t' : e);
        } else {
            buffer.push_back(static_cast<char>(current_));
        }
        Update();
    }

    if (current_ != '"') {
        Token error_token{TokenType::string_, buffer, line_, column_};
        throw LexerError{error_token, LexerError::kUntermStringLit};
    }
    Update();
    return Token{TokenType::string_, buffer, line_, column_};
}
