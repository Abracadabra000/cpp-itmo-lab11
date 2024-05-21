#include "SearchProcessor.hpp"
#include <iostream>

namespace searcheng
{

void SearchProcessor::Search(const std::string& stringExpr) {
    ExprProcessor proc{indexPath_};
    std::cout << "Calculating expression...\n";
    Expression exprResult = proc.CalculateExpression(stringExpr);
    std::cout << "Calculated expression!\n";
    // for (auto i : *exprResult) {
    //     std::cout << i.document_ << " " << i.line_ << "\n";
    // }
    std::vector<std::string> fileNames;
    std::ifstream indexFile{indexPath_};
    indexFile.seekg(kIntSize, std::ios::beg);
    indexFile.read(buffer, kIntSize);
    indexFile.seekg(kIntSize, std::ios::cur);
    int bytesLeft = from_char_to_int(buffer);
    int nameSize;
    int fileNumber;
    std::cout << "Writing names...\n";
    while (bytesLeft > 0 && indexFile.good()) {
        indexFile.read(buffer, kIntSize * 3);
        nameSize = from_char_to_int(buffer);
        indexFile.read(buffer, nameSize);
        fileNames.push_back(from_char_to_string(buffer, nameSize));
        bytesLeft -= nameSize + kIntSize * 3;
        // std::cout << fileNames[fileNames.size() - 1] << "________________________\n";
    }
    auto comparator = [&proc](const WordPosition& a, const WordPosition& b)->bool
                      {return (proc.GetScores()[a.document_] > proc.GetScores()[b.document_]);};
    std::cout << "Wrote names!\n";
    std::sort((*exprResult).begin(), (*exprResult).end(), comparator);
    WordPosition lastPos{-1, -1};
    for (const auto& pos : *exprResult) {
        if (pos == lastPos) {
            continue;
        }
        lastPos = pos;
        std::cout << fileNames[pos.document_] << ": " << pos.line_ << "\n";
    }
    std::cout << "Finished!\n";
}

}