#include "fileHasher.hpp"
#include <fstream>
#include <vector>
#include <list>

namespace fs = std::filesystem;

namespace invindex
{

class DirectoryHasher {
public:

    DirectoryHasher(const fs::path& dirP, const std::vector<std::string>& ignore):
        dirPath_{dirP},
        ignoreList_{ignore},
        hash_list_{},
        hash_table_{kExpandedHashBase}
    {}

    void HashFile(const fs::path& targetP);

    void HashDirectory();

    void PrintHash();

    struct WordData {
        std::string word_;
        int amount_ = 0;
    };

    std::list<long long> hash_list_;
    std::vector<WordData> hash_table_;
    std::vector<std::string> ignoreList_;
    fs::path dirPath_;
};
}