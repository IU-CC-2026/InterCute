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


struct Token {
    TokenType type;
    std::string value;
};


class Lexer {
public:
    unsigned int pos;
    std::string src_code;

    std::unordered_map<std::string, TokenType> keywords;
    std::unordered_map<char, TokenType> delimiters;
    std::unordered_map<std::string, TokenType> operators;

    Lexer(const std::string& src_code);

    Token get_token();

    std::vector<Token> tokenize();

    Token read_identifier();

    Token read_number();

    Token read_string();

    Token read_operator();

    Token read_delimiter();
};


class SyntaxError: public std::runtime_error {
public:
    SyntaxError(const std::string& message) : std::runtime_error(message) {};
};

std::string token_type_to_string(TokenType type);