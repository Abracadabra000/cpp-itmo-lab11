#include "ExpressionProcessor.hpp"

namespace searcheng
{

class SearchProcessor {
public:

    SearchProcessor(const fs::path& indexP) :
        indexPath_{indexP},
        buffer{new char[kBufferSize]}
    {}

    ~SearchProcessor() {
        delete buffer;
    }

    void Search(const std::string& stringExpr);

private:

    fs::path indexPath_;
    char* buffer;
};

}