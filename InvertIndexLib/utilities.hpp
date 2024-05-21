#pragma once

#include <iostream>

namespace invindex
{

const int kHashBase = 228227;
const int kPolynomBase = 371;
const int kBufferSize = 1000;
const int kInitialJump = 4;
const int kWriteSize = 8;
const int kByte = 8;
const int kIntSize = 4;
const int kByteNumber = 256;
const int kDefaultDocSize = 0;
const long long kExpandedPolyBase = 1051;
const long long kExpandedHashBase = 10003039;
const std::string kInvertIndexFileName = "dirind.inv";

bool isLetter(char c);

int Hash(const std::string& word);

long long ExpandedHash(const std::string& s);

void from_int_to_char(char* out, int a);

void from_string_to_char(char* out, const std::string& s);

struct WordIndex {
    std::string word_;
    size_t index_ = 0;
};

struct WordData {
    std::string word_;
    int amount_ = 0;
};

class bad_hash_calculation_error : std::exception {
public:
    const char* what() const noexcept override {
        return "ERROR! Couldn\'t calculate hash properly.\n";
    }
};

class invert_index_file_overwrite_error : std::exception {
public:
    const char* what() const noexcept override {
        return "ERROR! File named \'dirind.inv\' already exists. Use \'--override\' or \'-ov\' to override existing file.\n";
    }
};

class cannot_find_directory_error : std::exception {
public:
    const char* what() const noexcept override {
        return "ERROR! Target directory doesnt exist.\n";
    }
};



}