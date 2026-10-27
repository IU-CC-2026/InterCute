#include "../lexer/lexer.h"
#include "adapter.h"
#include "parser.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>


namespace fs = std::filesystem;


int main() {
    fs::path tests_dir = "tests";
    fs::path results_dir = "parser/results";

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


        std::ofstream result(result_path);
        if (!result.is_open()) {
            std::cerr << "Failed to create: " << result_path << std::endl;
            continue;
        }


        try {
            Lexer lexer(source);
            set_lexer(&lexer);

            std::string parser_error;
            Program* root = nullptr;
            int parse_result = yyparse(root, parser_error);

            if (parse_result == 0 && root != nullptr) {
                result << root->print(0) << '\n';
            } else {
                result << (parser_error.empty() ? "Parser Error" : parser_error) << '\n';
            }

            delete root;
        } catch (const std::exception& e) {
            result << e.what() << std::endl;
        }

        std::cout << entry.path() << " -> " << result_path << std::endl;
    }

    return 0;
}
