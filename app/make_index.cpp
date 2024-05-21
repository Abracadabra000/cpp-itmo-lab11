#include "../InvertIndexLib/directoryInverter.hpp"
// #include "../ArgParserLib/ArgParser.h"
#include "../InputReader/reader.hpp"

namespace inv = invindex;
// namespace argp = ArgumentParser;

int main(int argc, char** argv) {
    std::string targetDir;
    // argp::ArgParser parser("Input target directory.");
    // parser.AddStringArgument('d', "target-dir").StoreValue(targetDir);
    // parser.Parse(argc, argv);
    Reader reader;
    reader.Read(argc, argv);
    targetDir = reader.FindValue("target-dir");
    fs::path targetPath{targetDir};
    inv::DirectoryInverter inverter{targetPath};
    inverter.MakeInvertedIndex();
}