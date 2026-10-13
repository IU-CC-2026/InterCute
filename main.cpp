#include "lexer/lexer.h"
#include "parser/adapter.h"
#include "parser/parser.hpp"
#include "ast/ast_dump.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

// Restore cout even if the existing source printer throws an exception.
class OutputRedirect {
    std::streambuf* previous;
public:
    explicit OutputRedirect(std::ostream& out) : previous(std::cout.rdbuf(out.rdbuf())) {}
    ~OutputRedirect() { std::cout.rdbuf(previous); }
};

bool parse_file(const fs::path& path, std::ostream& out, bool ast_mode) {
    std::ifstream file(path);
    if (!file) {
        out << "Failed to open: " << path << '\n';
        return false;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    try {
        Lexer lexer(buffer.str());
        set_lexer(&lexer);
        Program* root = nullptr;
        const int status = yyparse(root);
        // Existing AST classes do not recursively delete their children yet.
        std::unique_ptr<Program> program(root);
        if (status != 0 || !program) {
            out << "Parser Error\n";
            return false;
        }
        if (ast_mode) dump_ast(program.get(), out);
        else {
            OutputRedirect redirect(out);
            program->print();
        }
        return true;
    } catch (const std::exception& e) {
        out << e.what() << '\n';
        return false;
    }
}

int main(int argc, char* argv[]) {
    bool ast_mode = false;
    fs::path input;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--ast") ast_mode = true;
        else if (arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [--ast] [file.cuti]\n"
                      << "Without a file: process tests/ into results/ or results_ast/.\n";
            return 0;
        } else if (arg.rfind("--", 0) == 0 || !input.empty()) {
            std::cerr << "Invalid arguments. Use --help.\n";
            return 2;
        } else input = arg;
    }
    if (!input.empty()) return parse_file(input, std::cout, ast_mode) ? 0 : 1;

    const fs::path tests_dir = "tests";
    const fs::path results_dir = ast_mode ? "results_ast" : "results";
    try {
        for (const auto& entry : fs::recursive_directory_iterator(tests_dir)) {
            if (!entry.is_regular_file() || entry.path().extension() != ".cuti") continue;
            fs::path result_path = results_dir / fs::relative(entry.path(), tests_dir);
            result_path.replace_extension(".txt");
            fs::create_directories(result_path.parent_path());
            std::ofstream result(result_path);
            if (!result) {
                std::cerr << "Failed to create: " << result_path << '\n';
                return 1;
            }
            // Batch mode also contains deliberately invalid test programs.
            parse_file(entry.path(), result, ast_mode);
            std::cout << entry.path() << " -> " << result_path << '\n';
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
