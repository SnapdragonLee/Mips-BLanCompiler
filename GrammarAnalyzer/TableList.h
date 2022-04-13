//
// Created by SNAPDRAGON_11800H on 28/10/2021.
//

#ifndef E5_MIPS_REWORK_TABLELIST_H
#define E5_MIPS_REWORK_TABLELIST_H

#include "AstClass.h"

class Decl;

class LVal;

class Func;

class CallExp;

class TableList {
public:
    TableList();

    Decl *findDecl(LVal *lval);

    void pushDecl(Decl *decl);

    Func *findFunc(CallExp *func);

    void pushFunc(Func *func);

    void pushBlock();

    void popBlock();

    bool isGlobal();

private:

    vector<map<string, Decl *>> declTable;
    vector<map<string, Func *>> funcTable;
};


#endif //E5_MIPS_REWORK_TABLELIST_H
