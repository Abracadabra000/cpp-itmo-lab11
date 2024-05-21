#include "utilities.hpp"

namespace searcheng
{

class WordReader {
public:

    WordReader(const fs::path& indexP):
        indexFile_{indexP, std::ios::binary},
        indexPath_{indexP},
        buffer{new char[kBufferSize]}
    {}

    WordScore FindWord(std::vector<WordPosition>& data, const std::string& word);

private:

    std::ifstream indexFile_;
    fs::path indexPath_;
    char* buffer;
};

}