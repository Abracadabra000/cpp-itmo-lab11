#include "fileInverter.hpp"
#include <filesystem>

namespace fs = std::filesystem;

namespace invindex 
{

class DirectoryInverter {
public:

    DirectoryInverter(const fs::path& dirP):
        dirPath_{dirP},
        hash_list_{},
        hash_table_{kExpandedHashBase},
        indexFile_(dirP / kInvertIndexFileName),
        writeBuffer{new char[kBufferSize]}
    {}

    ~DirectoryInverter() {
        delete writeBuffer;
    }

    bool MakeInvertedIndex();

private:
    fs::path dirPath_;
    std::ofstream indexFile_;
    std::list<long long> hash_list_;
    std::vector<WordIndex> hash_table_;
    std::vector<int> documentSizeInfoPosition_;
    char* writeBuffer;

    void WriteFileNames();

    void WriteHashes();

    void WriteWords();
};

}