#pragma once

#include <fstream>
#include <vector>
#include <list>
#include <filesystem>
#include "utilities.hpp"

namespace fs = std::filesystem;

namespace invindex
{

class FileInverter {
public:

    FileInverter(std::vector<WordIndex>& initial_ht,
                 std::list<long long>& initial_hl,
                 std::ofstream& indexP) :
        hash_table_{initial_ht},
        hash_list_{initial_hl},
        indexFile_{indexP},
        buffer{new char[kBufferSize]},
        writeBuffer(new char[kWriteSize])
    {}

    ~FileInverter() {
        delete buffer;
        delete writeBuffer;
    }

    int InvertFile(const fs::path& targetP, int fileIndex);


private:
    std::vector<WordIndex>& hash_table_;
    std::list<long long>& hash_list_;
    std::ofstream& indexFile_;
    char* buffer;
    char* writeBuffer;

};

}