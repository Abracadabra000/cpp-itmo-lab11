#include "directoryInverter.hpp"
#include "directoryHasher.hpp"
#include "fileInverter.hpp"

namespace invindex
{

void DirectoryInverter::WriteFileNames() {
    std::string name;
    int num = 0;
    int namesLen = 0;
    std::cout << "Writing file names...\n";
    indexFile_.seekp(0, std::ios::beg);
    indexFile_.write(writeBuffer, kIntSize * 3);
    int docAmount = 0;
    for (const auto& dir_entry : fs::recursive_directory_iterator(dirPath_)) {
        std::cout << dir_entry.path() << "\n";
        if (fs::is_directory(dir_entry.path())) {
            continue;
        }
        if (dir_entry.path() == dirPath_ / kInvertIndexFileName) {
            continue;
        }
        ++docAmount;
        name = fs::relative(dir_entry.path(), dirPath_).generic_string();
        int size = name.size();
        from_int_to_char(writeBuffer, size);
        from_int_to_char(writeBuffer + kIntSize, num);
        from_int_to_char(writeBuffer + kIntSize * 2, kDefaultDocSize);
        from_string_to_char(writeBuffer + kIntSize * 3, name);
        indexFile_.write(writeBuffer, kIntSize * 2);
        documentSizeInfoPosition_.push_back(indexFile_.tellp());
        indexFile_.write(writeBuffer + 2 * kIntSize, kIntSize + size);
        namesLen += kIntSize * 3 + size;
        std::cout << "size: " << size << "\n";
        ++num;
    }
    indexFile_.seekp(0, std::ios::beg);
    std::cout << namesLen << "\n";
    from_int_to_char(writeBuffer, docAmount);
    from_int_to_char(writeBuffer + kIntSize, namesLen);
    indexFile_.write(writeBuffer, kIntSize * 2);
    std::cout << "Names written!\n";
}

void DirectoryInverter::WriteHashes() {
    std::cout << "Calculating hash...\n";
    DirectoryHasher hasher{dirPath_};
    hasher.HashDirectory();
    std::cout << "Hash done!\n";
    hash_list_ = std::move(hasher.hash_list_);
    int bytesLeft;
    std::cout << "Making word table...\n";
    int blockSize;
    indexFile_.seekp(0, std::ios::end);
    for (long long h : hash_list_) {
        hash_table_[h].word_ = std::move(hasher.hash_table_[h].word_);
        blockSize = hasher.hash_table_[h].amount_ * (2 * kIntSize);
        from_int_to_char(writeBuffer, blockSize);
        from_int_to_char(writeBuffer + kIntSize, hash_table_[h].word_.size());
        from_string_to_char(writeBuffer + kIntSize + kIntSize, hash_table_[h].word_);
        indexFile_.write(writeBuffer, kIntSize + kIntSize + hash_table_[h].word_.size());
        hash_table_[h].index_ = indexFile_.tellp();
        bytesLeft = hasher.hash_table_[h].amount_ * (2 * kIntSize);
        while (bytesLeft > 0) {
            indexFile_.write(writeBuffer, std::min(bytesLeft, kBufferSize));
            bytesLeft -= kBufferSize;
        }
    }
    from_int_to_char(writeBuffer, 0);
    from_int_to_char(writeBuffer + kIntSize, 0);
    indexFile_.write(writeBuffer, kIntSize * 2);
    std::cout << "Table made!\n";
}

void DirectoryInverter::WriteWords() {
    FileInverter inverter(hash_table_, hash_list_, indexFile_);
    int num = 0;
    int curFileSize;
    int totalSize = 0;
    std::cout << "Writing words...\n";
    for (const auto& dir_entry : fs::recursive_directory_iterator(dirPath_)) {
        std::cout << dir_entry.path() << "\n";
        if (dir_entry.path().filename() == kInvertIndexFileName) {
            continue;
        }
        if (fs::is_directory(dir_entry.path())) {
            continue;
        }
        curFileSize = inverter.InvertFile(dir_entry.path(), num);
        totalSize += curFileSize;
        indexFile_.seekp(documentSizeInfoPosition_[num], std::ios::beg);
        from_int_to_char(writeBuffer, curFileSize);
        indexFile_.write(writeBuffer, kIntSize);
        ++num;
    }
    indexFile_.seekp(kIntSize * 2, std::ios::beg);
    from_int_to_char(writeBuffer, totalSize);
    indexFile_.write(writeBuffer, kIntSize);
    std::cout << "Words written!\n";
}

bool DirectoryInverter::MakeInvertedIndex() {
    WriteFileNames();
    WriteHashes();
    WriteWords();
    return true;
}

}