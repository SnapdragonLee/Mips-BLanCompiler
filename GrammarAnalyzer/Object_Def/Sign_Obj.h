//
// Created by SNAPDRAGON_11800H on 12/17/2021.
//

#ifndef E5_MIPS_REWORK_SIGN_H
#define E5_MIPS_REWORK_SIGN_H

#include "../BNode.h"

struct Sign_Obj : BNode {
    Keyword relation;       // Operationd

    explicit Sign_Obj(Keyword k);

    ~Sign_Obj() override = default;

    //todo: success to do this, next is explain all component from AST, and push into the symTable;
};


#endif //E5_MIPS_REWORK_SIGN_H
