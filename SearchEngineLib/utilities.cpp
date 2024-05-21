#include "utilities.hpp"


namespace searcheng
{

int from_char_to_int(char* data) {
    int res = 0;
    for (int i = 0; i < 4; ++i) {
        res *= kByteNumber;
        res += (uint8_t)data[i];
    }
    return res;
}

std::string from_char_to_string(char* data, int size) {
    std::string result;
    for (int i = 0; i < size; ++i) {
        result += data[i];
    }
    return result;
}

bool AreSame(char* data, int size, const std::string& s) {
    // std::cout << s << " saming\n";
    if (size != s.size()) {
        return false;
    }
    for (int i = 0; i < size; ++i) {
        if (data[i] != s[i]) {
            // std::cout << "->" << data[i] << "<-\n";
            return false;
        }
    }
    return true;
}

bool operator<(const WordPosition& lhs, const WordPosition& rhs) {
    if (lhs.document_ == rhs.document_) {
        return lhs.line_ < rhs.line_;
    }
    return lhs.document_ < rhs.document_;
}

bool operator==(const WordPosition& lhs, const WordPosition& rhs) {
    return lhs.document_ == rhs.document_ && lhs.line_ == rhs.line_;
}

double IDF(int docAmount, int containingAmount) {
    return std::log((docAmount - containingAmount + kIDFParam) /
                    (containingAmount + kIDFParam) + 1);
}

double ScorePartFunction(int freq, double avgdl, size_t docLen) {
    double upper_part = freq * (kScoreParam1 + 1);
    double koef = (1 - kScoreParam2 + kScoreParam2 * (docLen / avgdl));
    double lower_part = freq + kScoreParam2 * koef;
    return upper_part / lower_part;
}

}