#include "WordReader.hpp"
#include <iostream>

namespace searcheng
{

WordScore WordReader::FindWord(std::vector<WordPosition>& data, const std::string& word) {
    WordScore stats;
    indexFile_.seekg(kIntSize, std::ios::beg);
    indexFile_.read(buffer, kIntSize);
    indexFile_.seekg(kIntSize, std::ios::cur);
    indexFile_.seekg(from_char_to_int(buffer), std::ios::cur);
    int blockSize;
    int curWordSize;
    int curDocNumber;
    int curLineNumber;
    while (indexFile_.good()) {
        indexFile_.read(buffer, kIntSize + kIntSize);
        blockSize = from_char_to_int(buffer);
        curWordSize = from_char_to_int(buffer + kIntSize);
        if (blockSize == 0 && curWordSize == 0) {
            break;
        }
        indexFile_.read(buffer, curWordSize);
        if (!AreSame(buffer, curWordSize, word)) {
            indexFile_.seekg(blockSize, std::ios::cur);
            continue;
        }
        for (int i = 0; i < blockSize; i += 2 * kIntSize) {
            indexFile_.read(buffer, 2 * kIntSize);
            curLineNumber = from_char_to_int(buffer);
            curDocNumber = from_char_to_int(buffer + kIntSize);
            data.push_back({curLineNumber, curDocNumber});
            if (stats.size() == 0 || stats.back().document_ != curDocNumber) {
                stats.push_back({curDocNumber, 1});
            } else {
                ++stats.back().freq_;
            }
        }
        break;
    }
    return stats;
}

}