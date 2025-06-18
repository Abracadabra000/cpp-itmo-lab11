#include "directoryHasher.hpp"
#include <iostream>

namespace fs = std::filesystem;

namespace invindex
{

void DirectoryHasher::HashFile(const fs::path& targetP) {
    FileHashGenerator hasher{targetP};
    hasher.HashFile();
    long long expH;
    int amount;
    int jump;
    for (int h : hasher.hash_list) {
        std::string& w = hasher.hash_table[h].word_;
        expH = ExpandedHash(w);
        amount = hasher.hash_table[h].amount_;
        jump = kInitialJump;
        while (hash_table_[expH].amount_ != 0 &&
               hash_table_[expH].word_ != hasher.hash_table[h].word_) 
        {
            expH += jump * jump;
            expH %= kExpandedHashBase;
            ++jump;
            jump %= kExpandedHashBase;
        }
        if (hash_table_[expH].amount_ == 0) {
            hash_list_.push_back(expH);
            hash_table_[expH].word_ = w;
        }
        hash_table_[expH].amount_ += hasher.hash_table[h].amount_;
    }
}

void DirectoryHasher::HashDirectory() {
    bool isIgnoredFile;
    for (const auto& dir_entry : fs::recursive_directory_iterator(dirPath_)) {
        isIgnoredFile = false;
        for (const auto& name : ignoreList_) {
            for (const auto& dir : dir_entry.path()) {
                if (dir == name) {
                    isIgnoredFile = true;
                    break;
                }
            }
            if (isIgnoredFile) {
                break;
            }
        }
        if (isIgnoredFile) {
            continue;
        }
        if (fs::is_directory(dir_entry.path())) {
            continue;
        }
        if (dir_entry.path().filename() == kInvertIndexFileName) {
            continue;
        }
        std::cout << "Hashing " << dir_entry.path().generic_string() << "\n";
        HashFile(dir_entry.path());
    }
}

void DirectoryHasher::PrintHash() {
    for (auto i : hash_list_) {
        std::cout << hash_table_[i].word_ << " " << hash_table_[i].amount_ << "\n";
    }
}

}