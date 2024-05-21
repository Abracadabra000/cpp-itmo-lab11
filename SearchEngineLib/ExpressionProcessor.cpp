#include "ExpressionProcessor.hpp"
#include <iostream>

namespace searcheng
{

ExprProcessor::ExprProcessor(const fs::path& indexP) :
        indexPath_{indexP},
        documentScores_{}
{
    std::ifstream indexFile{indexP, std::ios::binary};
    char* buffer = new char[kBufferSize];
    indexFile.read(buffer, kIntSize * 3);
    docNumber_ = from_char_to_int(buffer);
    documentScores_ = std::vector<double>(docNumber_, 0);
    int docSize = from_char_to_int(buffer + 2 * kIntSize);
    avgDocLength_ = docSize / docNumber_;
    int bytesLeft = from_char_to_int(buffer + kIntSize);
    int size;
    int wordsInFile;
    while (bytesLeft > 0) {
        indexFile.read(buffer, kIntSize * 3);
        size = from_char_to_int(buffer);
        wordsInFile = from_char_to_int(buffer + kIntSize * 2);
        documentSizes_.push_back(wordsInFile);
        indexFile.seekg(size, std::ios::cur);
        bytesLeft -= kIntSize * 3 + size;
    }
}

Expression ExprProcessor::CalculateExpression(const std::string& stringExpression) {
    Expression result;
    int pos = 0;
    WordReader reader{indexPath_};
    FindBracket(stringExpression, pos, reader, result);
    return result;
}

void ExprProcessor::FindString(const std::string& stringExpression,
                               int& position, WordReader& reader,
                               Expression& dumpExpr)
{
    std::string word = "";
    while (position < stringExpression.size() &&
           stringExpression[position] != ' ' &&
           stringExpression[position] != ')')
    {
        word += stringExpression[position];
        ++position;
    }
    int wordMentions = 0;
    int lastDoc = -1;
    WordScore localScore = reader.FindWord(*dumpExpr, word);
    for (auto i : localScore) {
        if (i.document_ != lastDoc) {
            ++wordMentions;
        }
    }
    double sumPart = 0;
    lastDoc = -1;
    int freq = 0;
    for (auto i : localScore) {
        if (lastDoc == -1 || i.document_ == lastDoc) {
            ++freq;
            lastDoc = i.document_;
            continue;
        }
        documentScores_[i.document_] +=
        IDF(docNumber_, wordMentions) *
        ScorePartFunction(freq, avgDocLength_, documentSizes_[i.document_]);
    }
}

char ExprProcessor::NextOper(const std::string& stringExpression, int& position) {
    if (position >= stringExpression.size()) {
        return ')';
    }
    if (stringExpression[position] == ')') {
        ++position;
        return ')';
    }
    if (position + 4 < stringExpression.size() &&
        stringExpression[position + 0] == ' ' &&
        stringExpression[position + 1] == 'A' &&
        stringExpression[position + 2] == 'N' &&
        stringExpression[position + 3] == 'D' &&
        stringExpression[position + 4] == ' ')
    {
        position += 5;
        return '&';
    }
    if (position + 3 < stringExpression.size() &&
        stringExpression[position + 0] == ' ' &&
        stringExpression[position + 1] == 'O' &&
        stringExpression[position + 2] == 'R' &&
        stringExpression[position + 3] == ' ')
    {
        position += 4;
        return '|';
    }
    throw bad_expression_format{};
}

void ExprProcessor::FindBracket(const std::string& stringExpression,
                                int& position, WordReader& reader,
                                Expression& dumpExpr) 
{
    char oper = '|';
    Expression rightExpr;
    while (position < stringExpression.size() &&
           stringExpression[position] != ')')
    {
        if (stringExpression[position] == '(') {
            ++position;
            FindBracket(stringExpression, position, reader, rightExpr);
            ++position;
        } else {
            FindString(stringExpression, position, reader, rightExpr);
            if (position < stringExpression.size() &&
                stringExpression[position] == ')')
            {
                ++position;
            }
        }
        if (oper == '|') {
            dumpExpr |= rightExpr;
        } else {
            dumpExpr &= rightExpr;
        }
        rightExpr.clear();
        oper = NextOper(stringExpression, position);
    }
}

std::vector<double>& ExprProcessor::GetScores() {
    return documentScores_;
}

}