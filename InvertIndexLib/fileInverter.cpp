#include "fileInverter.hpp"

namespace invindex
{

int FileInverter::InvertFile(const fs::path& targetP, int fileIndex) {
    std::ifstream target{targetP};
    std::string curWord = "";
    int fileSize = 0;
    uint64_t bytesLeft = fs::file_size(targetP);
    uint64_t blockSize;
    int ind = 0;
    int line = 1;
    int curLine = line;
    while (target.good() && bytesLeft > 0) {
        blockSize = std::min(bytesLeft, static_cast<uint64_t>(kBufferSize));
        target.read(buffer, blockSize);
        bytesLeft -= blockSize;
        ind = 0;
        while (ind < blockSize) {
            while (ind < blockSize && isLetter(buffer[ind])) {
                curWord += buffer[ind];
                ++ind;
                if (buffer[ind] == '\n') {
                    ++line;
                }
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
            int h = ExpandedHash(curWord);
            ++fileSize;
            int jump = kInitialJump;
            while (hash_table_[h].index_ != 0 && hash_table_[h].word_ != curWord) {
                h += jump * jump;
                h %= kExpandedHashBase;
                ++jump;
                jump %= kExpandedHashBase;
            }
            if (hash_table_[h].index_ == 0) {
                throw bad_hash_calculation_error{};
            }
            from_int_to_char(writeBuffer, curLine);
            from_int_to_char(writeBuffer + kIntSize, fileIndex);
            indexFile_.seekp(hash_table_[h].index_, std::ios::beg);
            indexFile_.write(writeBuffer, kWriteSize);
            hash_table_[h].index_ += kWriteSize;
            curWord = "";
            curLine = line;
        }
    }
    return fileSize;
}

}