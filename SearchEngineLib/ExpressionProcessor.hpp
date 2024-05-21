#include "Expression.hpp"
#include "WordReader.hpp"

namespace searcheng
{

class ExprProcessor {
public:

    ExprProcessor(const fs::path& indexP);

    Expression CalculateExpression(const std::string& stringExpression);

    std::vector<double>& GetScores();

private:


    fs::path indexPath_;
    std::vector<double> documentScores_;
    double avgDocLength_;
    int docNumber_;
    std::vector<int> documentSizes_;
    
    void FindString(const std::string& stringExpression,
                    int& position, WordReader& reader,
                    Expression& dumpExpr);
    
    void FindBracket(const std::string& stringExpression,
                     int& position, WordReader& reader,
                     Expression& dumpExpr);

    char NextOper(const std::string& stringExpression, int& position);

};

}