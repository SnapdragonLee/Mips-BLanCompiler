//
// Created by SNAPDRAGON_11800H on 28/10/2021.
//

#include "TableList.h"

using namespace std;

TableList::TableList() = default;

void TableList::pushDecl(Decl *decl) {
    declTable.back().insert(make_pair(decl->getIdent().val, decl));
}

void TableList::pushBlock() {
    declTable.emplace_back();
    funcTable.emplace_back();
}

Decl *TableList::findDecl(LVal *lval) {
    for (auto v = declTable.rbegin(); v != declTable.rend(); v++)
        if (v->count(lval->getIdent().val))
            return (*v)[lval->getIdent().val];
    return nullptr;
}

void TableList::popBlock() {
    declTable.pop_back();
    funcTable.pop_back();
}
void TableList::pushFunc(Func *func) {
    funcTable.back().insert(make_pair(func->getIdent().val, func));
}

bool TableList::isGlobal() {
    return declTable.size() == 1;
}

Func *TableList::findFunc(CallExp *func) {
    for (auto v = funcTable.rbegin(); v != funcTable.rend(); v++)
        if (v->count(func->getIdent().val))
            return (*v)[func->getIdent().val];
    return nullptr;
}

