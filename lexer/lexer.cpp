#include "lexer.h"


Lexer::Lexer(const std::string& src_code) {
    this->pos = 0;
    this->src_code = src_code;

    this->keywords = {
        {"var", TokenType::VAR},
        {"is", TokenType::IS},
        {"if", TokenType::IF},
        {"then", TokenType::THEN},
        {"else", TokenType::ELSE},
        {"end", TokenType::END},
        {"while", TokenType::WHILE},
        {"for", TokenType::FOR},
        {"in", TokenType::IN},
        {"loop", TokenType::LOOP},
        {"exit", TokenType::EXIT},
        {"print", TokenType::PRINT},
        {"return", TokenType::RETURN},
        {"or", TokenType::OR},
        {"and", TokenType::AND},
        {"xor", TokenType::XOR},
        {"not", TokenType::NOT},
        {"int", TokenType::INT},
        {"real", TokenType::REAL},
        {"bool", TokenType::BOOL},
        {"string", TokenType::STRING},
        {"func", TokenType::FUNC},
        {"true", TokenType::TRUE},
        {"false", TokenType::FALSE},
        {"none", TokenType::NONE}
    };

    this->delimiters = {
        {'(', TokenType::OPEN_ROUND_BRACKET},
        {')', TokenType::CLOSE_ROUND_BRACKET},
        {'[', TokenType::OPEN_SQUARE_BRACKET},
        {']', TokenType::CLOSE_SQUARE_BRACKET},
        {'{', TokenType::OPEN_BRACE},
        {'}', TokenType::CLOSE_BRACE},
        {',', TokenType::COMMA},
        {'.', TokenType::DOT}
    };

    this->operators = {
        {":=", TokenType::ASSIGN},
        {"=>", TokenType::ARROW},
        {"<", TokenType::LT},
        {"<=", TokenType::LE},
        {">", TokenType::GT},
        {">=", TokenType::GE},
        {"=", TokenType::EQ},
        {"/=", TokenType::NEQ},
        {"+", TokenType::PLUS},
        {"-", TokenType::MINUS},
        {"*", TokenType::MUL},
        {"/", TokenType::DIV},
        {"..", TokenType::RANGE}
    };
}

Token Lexer::get_token() {
    std::string token_value = "";

    // EOF
    if (pos >= src_code.size()) {
        return Token{TokenType::END_OF_FILE, ""};
    }

    while (pos < src_code.size()) {
        // Ignore whitespaces and tabs
        if (src_code[pos] == ' ' || src_code[pos] == '\t') {
            pos++;
            continue;
        }

        // New line character
        if (src_code[pos] == '\n') {
            pos++;
            return Token{TokenType::NEWLINE, "\n"};
        }

        // Semicolon character
        if (src_code[pos] == ';') {
            pos++;
            return Token{TokenType::SEMICOLON, ";"};
        }

        // String literal
        if (src_code[pos] == '\"' || src_code[pos] == '\'') {
            return read_string();
        }

        // Number literal
        if (std::isdigit(src_code[pos])) {
            return read_number();
        }

        // Identifier or keyword
        if ((src_code[pos] >= 'a' && src_code[pos] <= 'z') 
            || (src_code[pos] >= 'A' && src_code[pos] <= 'Z') 
            || src_code[pos] == '_') {
            return read_identifier();
        }

        // Operators
        if (src_code[pos] == ':' 
            || src_code[pos] == '=' 
            || src_code[pos] == '<' 
            || src_code[pos] == '>' 
            || src_code[pos] == '+' 
            || src_code[pos] == '-' 
            || src_code[pos] == '*' 
            || src_code[pos] == '/') {
            return read_operator();
        }

        // . and ..
        if (src_code[pos] == '.') {
            if (pos + 1 < src_code.size() && src_code[pos + 1] == '.') {
                return read_operator();
            }

            return read_delimiter();
        }

        // Delimiters
        if (delimiters.find(src_code[pos]) != delimiters.end()) {
            return read_delimiter();
        }

        throw SyntaxError("Invalid character");
    }

    // If no token found, return EOF
    return Token{TokenType::END_OF_FILE, ""};
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        Token token = get_token();
        tokens.push_back(token);

        if (token.type == TokenType::END_OF_FILE) {
            break;
        }
    }

    return tokens;
}

Token Lexer::read_identifier() {
    std::string token_value = "";

    while (pos < src_code.size() && ((src_code[pos] >= 'a' && src_code[pos] <= 'z') 
            || (src_code[pos] >= 'A' && src_code[pos] <= 'Z') 
            || std::isdigit(src_code[pos]) 
            || src_code[pos] == '_')) {
        token_value += src_code[pos];
        pos++;
    }

    if (keywords.find(token_value) != keywords.end())
        return Token{keywords[token_value], token_value};
    return Token{TokenType::IDENTIFIER, token_value};
}

Token Lexer::read_number() {
    std::string token_value = "";
    bool found_dot = false;

    while (pos < src_code.size() && (std::isdigit(src_code[pos]) 
            || src_code[pos] == '.')) {
        if (src_code[pos] == '.') {
            if (found_dot) {
                throw SyntaxError("Invalid number!");
            } else {
                if (pos + 1 < src_code.size() && src_code[pos + 1] == '.') {
                    return Token{TokenType::INT_LITERAL, token_value};
                }
            }

            found_dot = true;
        }

        token_value += src_code[pos];
        pos++;
    }

    if (found_dot) {
        if (!std::isdigit(token_value[token_value.size() - 1])) throw SyntaxError("Invalid number!");

        return Token{TokenType::REAL_LITERAL, token_value};
    }
    return Token{TokenType::INT_LITERAL, token_value};
}

Token Lexer::read_string() {
    std::string token_value = "";
    token_value += src_code[pos];
    pos++;

    while (pos < src_code.size()) {
        if (src_code[pos] == token_value[0]) {
            token_value += src_code[pos];
            pos++;

            return Token{TokenType::STRING_LITERAL, token_value};
        }

        token_value += src_code[pos];
        pos++;
    }

    throw SyntaxError("Invalid string");
}

Token Lexer::read_operator() {
    std::string token_value = "";
    token_value += src_code[pos];
    pos++;

    if (pos < src_code.size()) {
        token_value += src_code[pos];
        
        if (operators.find(token_value) != operators.end()) {
            pos++;

            return Token{operators[token_value], token_value};
        }

        token_value.pop_back();
    }

    if (operators.find(token_value) != operators.end()) {
        return Token{operators[token_value], token_value};
    }

    throw SyntaxError("Invalid operator");
}

Token Lexer::read_delimiter() {
    char delim = src_code[pos];
    pos++;
    return Token{delimiters[delim], std::string(1, delim)};
}


std::string token_type_to_string(TokenType type) {
    switch (type) {
        case TokenType::VAR: return "VAR";
        case TokenType::IS: return "IS";
        case TokenType::IF: return "IF";
        case TokenType::THEN: return "THEN";
        case TokenType::ELSE: return "ELSE";
        case TokenType::END: return "END";
        case TokenType::WHILE: return "WHILE";
        case TokenType::FOR: return "FOR";
        case TokenType::IN: return "IN";
        case TokenType::LOOP: return "LOOP";
        case TokenType::EXIT: return "EXIT";
        case TokenType::PRINT: return "PRINT";
        case TokenType::RETURN: return "RETURN";
        case TokenType::OR: return "OR";
        case TokenType::AND: return "AND";
        case TokenType::XOR: return "XOR";
        case TokenType::NOT: return "NOT";
        case TokenType::INT: return "INT";
        case TokenType::REAL: return "REAL";
        case TokenType::BOOL: return "BOOL";
        case TokenType::STRING: return "STRING";
        case TokenType::FUNC: return "FUNC";

        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::ARROW: return "ARROW";
        case TokenType::LT: return "LT";
        case TokenType::LE: return "LE";
        case TokenType::GT: return "GT";
        case TokenType::GE: return "GE";
        case TokenType::EQ: return "EQ";
        case TokenType::NEQ: return "NEQ";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MUL: return "MUL";
        case TokenType::DIV: return "DIV";
        case TokenType::RANGE: return "RANGE";

        case TokenType::OPEN_ROUND_BRACKET: return "OPEN_ROUND_BRACKET";
        case TokenType::CLOSE_ROUND_BRACKET: return "CLOSE_ROUND_BRACKET";
        case TokenType::OPEN_SQUARE_BRACKET: return "OPEN_SQUARE_BRACKET";
        case TokenType::CLOSE_SQUARE_BRACKET: return "CLOSE_SQUARE_BRACKET";
        case TokenType::OPEN_BRACE: return "OPEN_BRACE";
        case TokenType::CLOSE_BRACE: return "CLOSE_BRACE";
        case TokenType::COMMA: return "COMMA";
        case TokenType::DOT: return "DOT";

        case TokenType::IDENTIFIER: return "IDENTIFIER";

        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::END_OF_FILE: return "END_OF_FILE";

        case TokenType::INT_LITERAL: return "INT_LITERAL";
        case TokenType::REAL_LITERAL: return "REAL_LITERAL";
        case TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case TokenType::TRUE: return "TRUE";
        case TokenType::FALSE: return "FALSE";
        case TokenType::NONE: return "NONE";
    }

    return "UNKNOWN";
}