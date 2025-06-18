#include "utilities.hpp"

namespace invindex
{

int Hash(const std::string& word) {
    long long res = 0;
    for (char c : word) {
        res *= kPolynomBase;
        res %= kHashBase;
        res += c;
        res %= kHashBase;
    }
    return res;
}

long long ExpandedHash(const std::string& s) {
    long long res = 0;
    for (char c : s) {
        res *= kExpandedPolyBase;
        res %= kExpandedHashBase;
        res += c;
        res %= kExpandedHashBase;
    }
    return res;
}

bool isLetter(char c) {
    if (c == ' ') {
        return false;
    }
    if (c == '<' || c == '>') {
        return false;
    }
    if (c == '(' || c == ')') {
        return false;
    }
    if (c == '\n' || c == '\t') {
        return false;
    }
    if (c == ':') {
        return false;
    }
    return true;
}

void from_int_to_char(char* out, int a) {
    for (int i = 3; i >= 0; --i) {
        out[i] = a % kByteNumber;
        a >>= kByte;
    }
}

void from_string_to_char(char* out, const std::string& s) {
    for (int i = 0; i < s.size(); ++i) {
        out[i] = s[i];
    }
}

}