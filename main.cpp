#include "lexer/lexer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>


namespace fs = std::filesystem;


int main() {
    fs::path tests_dir = "tests";
    fs::path results_dir = "results";

    // recursively iterate through all subdirectories and files
    for (const auto& entry : fs::recursive_directory_iterator(tests_dir)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".cuti") {
            continue;
        }


        // construct path for file with test results
        fs::path relative_path = fs::relative(entry.path(), tests_dir);
        fs::path result_path = results_dir / relative_path;
        result_path.replace_extension(".txt");
        fs::create_directories(result_path.parent_path());


        std::ifstream file(entry.path());
        if (!file.is_open()) {
            std::cerr << "Failed to open: " << entry.path() << std::endl;
            continue;
        }


        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string source = buffer.str();


        // remove "Expected result: ..." from the end of file
        size_t comment_pos = source.find("//");
        if (comment_pos != std::string::npos) {
            source = source.substr(0, comment_pos);
        }
        comment_pos = source.find("/*");
        if (comment_pos != std::string::npos) {
            source = source.substr(0, comment_pos);
        }


        std::ofstream result(result_path);
        if (!result.is_open()) {
            std::cerr << "Failed to create: " << result_path << std::endl;
            continue;
        }


        try {
            Lexer lexer(source);
            std::vector<Token> tokens = lexer.tokenize();

            for (const auto& token : tokens) {
                result << token_type_to_string(token.type) << "(";

                if (token.type == TokenType::NEWLINE) {
                    result << "\\n";
                } else if (token.type == TokenType::END_OF_FILE) {
                    result << "EOF";
                } else {
                    result << token.value;
                }

                result << ")" << std::endl;
            }
        } catch (SyntaxError& e) {
            result << "Syntax Error" << std::endl;
        }

        std::cout << entry.path() << " -> " << result_path << std::endl;
    }

    return 0;
}