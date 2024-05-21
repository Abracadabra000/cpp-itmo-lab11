#include "SearchProcessor.hpp"
#include <iostream>

namespace searcheng
{

void SearchProcessor::Search(const std::string& stringExpr) {
    ExprProcessor proc{indexPath_};
    Expression exprResult = proc.CalculateExpression(stringExpr);
    std::vector<std::string> fileNames;
    std::ifstream indexFile{indexPath_, std::ios::binary};
    indexFile.seekg(kIntSize, std::ios::beg);
    indexFile.read(buffer, kIntSize);
    indexFile.seekg(kIntSize, std::ios::cur);
    int bytesLeft = from_char_to_int(buffer);
    int nameSize;
    int fileNumber;
    while (bytesLeft > 0 && indexFile.good()) {
        indexFile.read(buffer, kIntSize * 3);
        nameSize = from_char_to_int(buffer);
        indexFile.read(buffer, nameSize);
        fileNames.push_back(from_char_to_string(buffer, nameSize));
        bytesLeft -= nameSize + kIntSize * 3;
    }
    auto comparator = [&proc](const WordPosition& a, const WordPosition& b)->bool
                      {return (proc.GetScores()[a.document_] > proc.GetScores()[b.document_]);};
    std::sort((*exprResult).begin(), (*exprResult).end(), comparator);
    WordPosition lastPos{-1, -1};
    if ((*exprResult).size() == 0) {
        std::cout << "No matches were found.\n";
    }
    for (const auto& pos : *exprResult) {
        if (pos == lastPos) {
            continue;
        }
        lastPos = pos;
        std::cout << fileNames[pos.document_] << ": " << pos.line_ << "\n";
    }
}

}