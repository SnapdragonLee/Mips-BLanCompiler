//
// Created by SNAPDRAGON_11800H on 12/17/2021.
//

#ifndef E5_MIPS_REWORK_PRINT_OBJ_H
#define E5_MIPS_REWORK_PRINT_OBJ_H


#include "../BNode.h"

struct Print_Obj : BNode {
    Keyword relation = PRINTFTK;       // Operation
    string *s;
    int dT;
    vector<BNode *> EXT;

    explicit Print_Obj(string *str, int dTime);

    ~Print_Obj() override = default;

    //todo: success to do this, next is explain all component from AST, and push into the symTable;
};


#endif //E5_MIPS_REWORK_PRINT_OBJ_H
