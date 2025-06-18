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
    try
    {
    
        Reader reader;
        reader.Read(argc, argv);
        targetDir = reader.FindValue("target-dir");
        expression = reader.FindPositional();
        fs::path targetPath{targetDir};
        srch::SearchProcessor searcher(targetPath / srch::kInvertIndexFileName);
        std::vector<srch::LineMatch> result = searcher.Search(expression);
        for (const auto& l : result) {
            std::cout << l.document_ << ": " << l.line_ << "\n";
        }

    }
    catch (const command_not_found_error& error)
    {

        std::cout << error.what(); 

    }
    catch (const srch::bad_expression_format& error)
    {
    
        std::cout << error.what();

    }
    catch (const srch::no_index_file_found& error)
    {

        std::cout << error.what();

    }
    catch (const std::exception& error)
    {

        std::cout << error.what();

    }
    catch (...)
    {
    
        std::cout << "Unknown error occured!\n";

    }
}