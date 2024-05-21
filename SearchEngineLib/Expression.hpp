#include "utilities.hpp"

namespace searcheng
{

class Expression {
public:

    using ResultTy = std::vector<WordPosition>;

    Expression() :
        exprResult_{}
    {}

    Expression(ResultTy&& other) :
        exprResult_{std::move(other)}
    {}

    void clear();

    ResultTy& operator*();

    Expression& operator|=(const Expression& other);

    Expression& operator&=(const Expression& other);

private:

    ResultTy exprResult_;

};

}