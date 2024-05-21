#include "Expression.hpp"

namespace searcheng
{

void Expression::clear() {
    exprResult_.clear();
}

Expression::ResultTy& Expression::operator*() {
    return exprResult_;
}

Expression& Expression::operator|=(const Expression& other) {
    for (int i = 0; i < other.exprResult_.size(); ++i) {
        exprResult_.push_back(other.exprResult_[i]);
    }
    std::sort(exprResult_.begin(), exprResult_.end());
    return *this;
}

Expression& Expression::operator&=(const Expression& other) {
    int i = 0;
    int j = 0;
    ResultTy result;
    while (i < exprResult_.size() && j < other.exprResult_.size()) {
        if (exprResult_[i] == other.exprResult_[j]) {
            result.push_back(exprResult_[i]);
            ++i;
            ++j;
            continue;
        }
        if (exprResult_[i] < other.exprResult_[j]) {
            ++i;
        } else {
            ++j;
        }
    }
    exprResult_ = result;
    return *this;
}

}