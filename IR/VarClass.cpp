//
// Created by SNAPDRAGON_11800H on 17/11/2021.
//

#include "VarClass.h"

VarClass::VarClass() {
    this->is_global = false;
    this->is_addr = false;
}

VarClass::VarClass(VarClass *var) {
    this->id = var->id;
    this->is_global = var->is_global;
    this->is_addr = false;
    this->bType = var->bType;
}

VarClass::VarClass(int id, bool is_global, TypeDefine bType) {
    this->id = id;
    this->is_global = is_global;
    this->is_addr = false;
    this->bType = move(bType);
}

int VarClass::getId() const {
    return id;
}

TypeDefine VarClass::getType() {
    return bType;
}

bool VarClass::isGlobal() const {
    return is_global;
}

string VarClass::showDim() {
    string res;
    for (auto x: dims) {
        res += "[" + x->getIR() + "]";
    }
    return res;
}

string VarClass::getIR() {
    string tag = isGlobal() ? "@" : "%";
    return tag + to_string(id) + showDim();
}

void VarClass::addAddr() {
    is_addr = true;
}

bool VarClass::isAddr() const {
    return is_addr;
}

Constant::Constant() : VarClass() {
    this->value = 0;
    this->type = UNKNOWN;
}

Constant::Constant(int x) : VarClass() {
    this->type = INTTK;
    this->value = x;
}

Constant::Constant(string s) : VarClass() {
    this->value = 0;
    this->type = STRCON;
    this->str = move(s);
}

string Constant::getIR() {
    if (type == STRCON) return "\"" + str + "\"";
    return to_string(value);
}

int Constant::getValue() const {
    return value;
}

IrParam::IrParam() {
    this->constant = nullptr;
}

IrParam::IrParam(int varId, Decl *decl) : VarClass(varId, false, decl->getType()) {
    this->constant = nullptr;
}

IrParam::IrParam(VarClass *var) : VarClass(var) {
    this->constant = nullptr;
}

IrParam::IrParam(Constant *constant) {
    this->constant = constant;
}

string IrParam::getIR() {
    if (constant) return constant->getIR();
    return VarClass::getIR();
}

IrArray::IrArray() {
    this->isConst = false;
    this->base = 0;
    this->size = 0;
}

void IrArray::addConst() {
    isConst = true;
}

IrArray::IrArray(int id, bool is_global, const TypeDefine &bType) : VarClass(id, is_global, bType) {
    this->base = id;
    this->isConst = false;
    this->size = 1;
    vector<int> dims;
    dims = bType.getDims();
    for (auto x: dims) this->size *= x;
}

string IrArray::getIR() {
    auto tag = isGlobal() ? "@" : "%";
    return tag + to_string(base);
}

int IrArray::getBase() const {
    return base;
}

int IrArray::getSize() const {
    return size;
}

IrPointer::IrPointer() {
    this->base = 0;
    this->isConst = false;
}

IrPointer::IrPointer(int base, Decl *decl) : IrParam(base, decl) {
    this->base = base;
    this->isConst = false;
}

void IrPointer::addConst() {
    isConst = true;
}

string IrPointer::getIR() {
    return "%" + to_string(base);
}
