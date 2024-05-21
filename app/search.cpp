// #include "../ArgParserLib/ArgParser.h"
#include "../InputReader/reader.hpp"
#include "../SearchEngineLib/SearchProcessor.hpp"
#include <filesystem>

namespace fs = std::filesystem;
namespace srch = searcheng;
// namespace argp = ArgumentParser;

int main(int argc, char** argv) {
    std::string expression;
    std::string targetDir;
    // argp::ArgParser parser("Input target directory and expression. Be sure to have inverted index in there!");
    // parser.AddStringArgument('d', "target-dir").StoreValue(targetDir);
    // parser.AddStringArgument("expression").MultiValue().Positional().StoreValues(dividedExpression);
    // parser.Parse(argc, argv);
    // for (const auto& i : dividedExpression) {
    //     std::cout << i << "\n";
    //     finalExpression += i;
    // }
    try
    {
    
    Reader reader;
    reader.Read(argc, argv);
    targetDir = reader.FindValue("target-dir");
    expression = reader.FindPositional();
    fs::path targetPath{targetDir};
    srch::SearchProcessor searcher(targetPath / srch::kInvertIndexFileName);
    std::cout << expression << "\n";
    searcher.Search(expression);

    }
    catch (const command_not_found_error& error)
    {

    std::cout << error.what();

    }
    catch (const srch::bad_expression_format& error)
    {
    
    std::cout << error.what();

    }
    catch (...)
    {
    
        std::cout << "Unknown error occured!\n";

    }
}