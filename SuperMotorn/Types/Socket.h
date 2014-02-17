#pragma once
#include "Matrix.h"
#include <string>
struct Socket {
    std::string mName;
    Matrix      mTransform;
    Socket(const std::string& pName, const Matrix& pTransform) :
    mName(pName), mTransform(pTransform) {
    }
};

