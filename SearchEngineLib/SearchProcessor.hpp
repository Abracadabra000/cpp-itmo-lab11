#include "ExpressionProcessor.hpp"

namespace searcheng
{

class SearchProcessor {
public:

    SearchProcessor(const fs::path& indexP) :
        indexPath_{indexP},
        buffer{new char[kBufferSize]}
    {
        if (!fs::exists(indexP)) {
            throw no_index_file_found{};
        }
    }

    ~SearchProcessor() {
        delete buffer;
    }

    std::vector<LineMatch> Search(const std::string& stringExpr);

private:

    fs::path indexPath_;
    char* buffer;
};

}