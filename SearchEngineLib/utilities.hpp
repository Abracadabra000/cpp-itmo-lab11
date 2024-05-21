#pragma once

#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <cmath>

namespace searcheng
{

const int kBufferSize = 1000;
const int kIntSize = 4;
const int kByteNumber = 256;
const double kScoreParam1 = 2.0;
const double kScoreParam2 = 0.75;
const double kIDFParam = 0.5;
const std::string kInvertIndexFileName = "dirind.inv";

namespace fs = std::filesystem;

int from_char_to_int(char* data);

std::string from_char_to_string(char* data, int size);

double IDF(int docAmount, int containingAmount);

double ScorePartFunction(int freq, double avgdl, size_t docLen);

bool AreSame(char* data, int size, const std::string& s);

struct WordPosition {
    int line_;
    int document_;
};

bool operator<(const WordPosition& lhs, const WordPosition& rhs);

bool operator==(const WordPosition& lhs, const WordPosition& rhs);

struct RangingStats {
    int document_;
    int freq_;
};

struct ScoreStats {
    int document_;
    double score_;
};

using WordScore = std::vector<RangingStats>;

struct RangeInfo {
    WordScore score_;
    int docAmount_ = 0;
    double avgdl_ = 0;
    std::vector<int> docSizes_;
};

class bad_expression_format : std::exception {
public:
    const char* what() const noexcept override {
        return "ERROR! Expression format is incomputible!\n";
    }
};

}