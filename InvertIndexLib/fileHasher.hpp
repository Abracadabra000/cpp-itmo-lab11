#include "utilities.hpp"
#include <fstream>
#include <vector>
#include <filesystem>
#include <list>

namespace fs = std::filesystem;

namespace invindex
{

class FileHashGenerator {
public:

    FileHashGenerator(const fs::path& fileP);

    ~FileHashGenerator();

    void HashFile();

    fs::path filePath;
    std::ifstream targetFile;
    std::list<int> hash_list;
    std::vector<WordData> hash_table;
    char* buffer;
};

}