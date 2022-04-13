//
// Created by SNAPDRAGON_11800H on 17/11/2021.
//

#include "Ctx.h"
#include "IrFunc.h"

IrFunc::IrFunc() {
    varId = 0;
}

IrFunc::IrFunc(Func *func) {
    varId = 0;
    funcName   = func->getIdent().val;
    returnType = func->getType();
}

void IrFunc::addBlock(BlockEntry *blk) {
    blocks.push_back(blk);
}

void IrFunc::addDecl(Decl* d, DeclInst* decl) {
    table[d] = decl->getVar();
}

string IrFunc::getName() {
    return funcName;
}


int IrFunc::genVar() {
    return varId++;
}

BlockEntry *IrFunc::getEntryBlock() const {
    return blocks.h;
}

int IrFunc::getVarId() const {
    return varId;
}

TypeDefine IrFunc::getType() {
    return returnType;
}

VarClass *IrFunc::getVar(Decl *decl) {
    return table[decl];
}

int IrFunc::genArray(int size) {
    int id = varId;
    varId += size;
    return id;
}

string IrFunc::getIR() {
    string ty = returnType.getString();
    string f  = ty + " " + funcName + "()";
    string p;
    for (auto x : params) p += x->getIR() + "\n";
    string b;
    for (BlockEntry* blk = blocks.h; blk; blk = blk->next) {
        b += blk->getIR();
    }
    return f + "\n" + p + b;
}

