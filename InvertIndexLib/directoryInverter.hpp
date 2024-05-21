#include "fileInverter.hpp"
#include <filesystem>

namespace fs = std::filesystem;

namespace invindex 
{

class DirectoryInverter {
public:

    DirectoryInverter(const fs::path& dirP, bool ov, const std::vector<std::string>& ignore):
        overrideIndex_{ov},
        ignoreList_{ignore},
        dirPath_{dirP},
        hash_list_{},
        hash_table_{kExpandedHashBase},
        writeBuffer{new char[kBufferSize]}
    {
        if (!fs::exists(dirP)) {
            throw cannot_find_directory_error{};
        }
        if (fs::exists(dirP / kInvertIndexFileName) && overrideIndex_ == false) {
            throw invert_index_file_overwrite_error{};
        }
        indexFile_.open(dirP / kInvertIndexFileName, std::ios::binary);
    }

    ~DirectoryInverter() {
        delete writeBuffer;
    }

    bool MakeInvertedIndex();

private:

    bool overrideIndex_;
    fs::path dirPath_;
    std::ofstream indexFile_;
    std::list<long long> hash_list_;
    std::vector<WordIndex> hash_table_;
    std::vector<int> documentSizeInfoPosition_;
    std::vector<std::string> ignoreList_;
    char* writeBuffer;

    void WriteFileNames();

    void WriteHashes();

    void WriteWords();
};

}