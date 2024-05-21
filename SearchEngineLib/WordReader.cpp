#include "WordReader.hpp"
#include <iostream>

namespace searcheng
{

WordScore WordReader::FindWord(std::vector<WordPosition>& data, const std::string& word) {
    std::cout << "search: " << word << "\n";
    WordScore stats;
    // indexFile_.seekg(4, std::ios::beg);
    // indexFile_.read(buffer, 1);
    // std::cout << indexFile_.tellg() << '\n';
    // indexFile_.read(buffer + 1, 1);
    // std::cout << indexFile_.tellg() << '\n';
    // indexFile_.read(buffer + 2, 1);
    // std::cout << indexFile_.tellg() << '\n';
    // indexFile_.read(buffer + 3, 1);
    // std::cout << indexFile_.tellg() << '\n';
    indexFile_.seekg(kIntSize, std::ios::beg);
    // std::cout << indexFile_.tellg() << "\n";
    indexFile_.read(buffer, kIntSize);
    std::cout << indexFile_.tellg() << "\n";
    indexFile_.seekg(kIntSize, std::ios::cur);
    // indexFile_.seekg(2 * kIntSize, std::ios::cur);
    
    // indexFile_.seekg(kIntSize, std::ios::cur);
    std::cout << indexFile_.tellg() << "\n";
    std::cout << from_char_to_int(buffer) << "\n";
    indexFile_.seekg(from_char_to_int(buffer), std::ios::cur);
    std::cout << indexFile_.tellg() << "\n";
    int blockSize;
    int curWordSize;
    int curDocNumber;
    int curLineNumber;
    while (indexFile_.good()) {
        indexFile_.read(buffer, kIntSize + kIntSize);
        std::cout << buffer[0] << "-" << buffer[1] << "-" << buffer[2] << "-" << buffer[3] << "\n";
        std::cout << buffer[4] << "-" << buffer[5] << "-" << buffer[6] << "-" << buffer[7] << "\n";
        blockSize = from_char_to_int(buffer);
        curWordSize = from_char_to_int(buffer + kIntSize);
        std::cout << blockSize << " " << curWordSize << "\n";
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
            std::cout << data[data.size() - 1].document_ << " " << data[data.size() - 1].line_ << "\n";
        }
        std::cout << "thats it\n";
        break;
    }
    return stats;
}

}