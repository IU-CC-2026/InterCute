#pragma once


#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>


enum class TokenType {
    // Keywords
    VAR,
    IS,
    IF,
    THEN,
    ELSE,
    END,
    WHILE,
    FOR,
    IN,
    LOOP,
    EXIT,
    PRINT,
    RETURN,
    OR,
    AND,
    XOR,
    NOT,
    INT,
    REAL,
    BOOL,
    STRING,
    FUNC,

    // Operators
    ASSIGN,
    ARROW,
    LT,
    LE,
    GT,
    GE,
    EQ,
    NEQ,
    PLUS,
    MINUS,
    MUL,
    DIV,
    RANGE,

    // Delimiters
    OPEN_ROUND_BRACKET,
    CLOSE_ROUND_BRACKET,
    OPEN_SQUARE_BRACKET,
    CLOSE_SQUARE_BRACKET,
    OPEN_BRACE,
    CLOSE_BRACE,
    COMMA,
    DOT,

    // Identifiers
    IDENTIFIER,

    // Separators
    SEMICOLON,
    NEWLINE,
    END_OF_FILE,

    // Whitespaces are ignored

    // Literals
    INT_LITERAL,
    REAL_LITERAL,
    STRING_LITERAL,
    TRUE,
    FALSE,
    NONE
};


// representation of the fragment of the text for further process.
struct Token {
    TokenType type; // tells what was encountered: e.g., name, number, operator and so on.
    std::string value; // stores textual value.

    unsigned int line;
    unsigned int column; // line and column point to the position of the original program.
};


class Lexer {
public:
    unsigned int pos; // index of the current symbol inside src-code.
    unsigned int line; // number of the current string.
    unsigned int column; // number of the current position in the string.

    std::string src_code; // contains the whole source code of the program
    TokenType previous_token_type;

    std::unordered_map<std::string, TokenType> keywords;
    std::unordered_map<char, TokenType> delimiters;
    std::unordered_map<std::string, TokenType> operators;

    Lexer(const std::string& src_code); 

    Token get_token(); // returns next token. E.g., in var x := 2 + 3 * 4 it does not calculate 2 + 3 * 4 and doesn't check, whether the variable x was declared earlier.

    Token remember_token(Token token);

    std::vector<Token> tokenize();

    Token read_identifier(unsigned int start_column);

    Token read_number(unsigned int start_column);

    Token read_string(unsigned int start_column);

    Token read_operator(unsigned int start_column);

    Token read_delimiter(unsigned int start_column);

    void skip_comment();
};


class SyntaxError: public std::runtime_error {
public:
    SyntaxError(
        unsigned int line,
        unsigned int column,
        const std::string& message
    ) : std::runtime_error(
        "Lexer error at line " + std::to_string(line) +
        ", column " + std::to_string(column) +
        ": " + message
    ) {}
};

std::string token_type_to_string(TokenType type);