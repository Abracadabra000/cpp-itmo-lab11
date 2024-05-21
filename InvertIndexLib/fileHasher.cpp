#include "fileHasher.hpp"

namespace invindex
{

FileHashGenerator::FileHashGenerator(const fs::path& fileP) :
    filePath{fileP},
    targetFile{fileP},
    hash_list{},
    hash_table{kHashBase},
    buffer{new char[kBufferSize]}
{}

FileHashGenerator::~FileHashGenerator() {
    delete buffer;
}

void FileHashGenerator::HashFile() {
    std::string curWord = "";
    uint64_t bytesLeft = fs::file_size(filePath);
    std::cout << bytesLeft << "\n";
    uint64_t blockSize;
    int ind = 0;
    while (targetFile.good() && bytesLeft > 0) {
        blockSize = std::min(bytesLeft, static_cast<uint64_t>(kBufferSize));
        targetFile.read(buffer, blockSize);
        bytesLeft -= blockSize;
        ind = 0;
        while (ind < blockSize) {
            while (ind < blockSize && isLetter(buffer[ind])) {
                curWord += buffer[ind];
                ++ind;
            }
            if (buffer[ind] == '\n') {
                --blockSize;
            }
            if (!isLetter(buffer[ind])) {
                ++ind;
            }
            if (curWord == "") {
                continue;
            }
            if (ind >= blockSize && bytesLeft > 0) {
                break;
            }
            int h = Hash(curWord);
            int jump = kInitialJump;
            while (hash_table[h].amount_ != 0 && hash_table[h].word_ != curWord) {
                h += jump * jump;
                ++jump;
            }
            if (hash_table[h].amount_ == 0) {
                hash_list.push_back(h);
                hash_table[h].word_ = curWord;
            }
            ++hash_table[h].amount_;
            curWord = "";
        }
    }
}

}