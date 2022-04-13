//
// Created by SNAPDRAGON_11800H on 29/10/2021.
//

#include "TypeDefine.h"

TypeDefine::TypeDefine() = default;

string TypeDefine::getString() const {
    string dim_string = isPointer ? "[]" : "";
    for (auto x: dims) dim_string += "[" + to_string(x) + "]";
    return type.val;
}

TypeDefine::TypeDefine(Element type, vector<int> dims, bool isConst, bool isParam, bool isPointer) {
    this->type = move(type);
    this->dims.insert(this->dims.end(), dims.begin(), dims.end());
    this->isConst = isConst;
    this->isParam = isParam;
    this->isPointer = isPointer;
}

TypeDefine::TypeDefine(Element type) {
    this->isParam = false;
    this->isPointer = false;
    this->isConst = false;
    this->type = move(type);
}

void TypeDefine::addDim(int dim) {
    dims.push_back(dim);
}

Element TypeDefine::getType() const {
    return type;
}

int TypeDefine::getDim() const {
    return (int) dims.size() + isPointer;
}

vector<int> TypeDefine::getDims() const {
    return dims;
}

bool TypeDefine::getConst() const {
    return isConst;
}

void TypeDefine::addPointer() {
    this->isPointer = true;
}

void TypeDefine::addParam() {
    this->isParam = true;
}

bool TypeDefine::operator==(const TypeDefine &t) {
    return type == t.getType() && checkDim(t);
}

bool TypeDefine::operator!=(const TypeDefine &t) {
    return type != t.getType() || !checkDim(t);
}

bool TypeDefine::getParam() const {
    return isParam;
}

bool TypeDefine::getPointer() const {
    return isPointer;
}

bool TypeDefine::checkDim(const TypeDefine &t) const {
    if (getDim() != t.getDim()) return false;
    if (getPointer() && t.getPointer() || !getPointer() && !t.getPointer()) {
        vector<int> v,w;
        v = getDims();
        w = t.getDims();
        for (int i = 0; i < v.size(); i++)
            if (v[i] != w[i])
                return false;
    } else if (getPointer()) {
        vector<int> v, w;
        v = getDims();
        w = t.getDims();
        for (int i = 0; i < v.size(); i++)
            if (v[i] != w[i])
                return false;
    } else if (t.getPointer()) {
        vector<int> v,w;
        v = getDims();
        w = t.getDims();
        for (int i = 0; i < w.size(); i++)
            if (v[i] != w[i])
                return false;
    }
    return true;
}

