//
// Created by SNAPDRAGON_11800H on 12/17/2021.
//

#ifndef E5_MIPS_REWORK_UNARYEXP_OBJ_H
#define E5_MIPS_REWORK_UNARYEXP_OBJ_H

#include "../BNode.h"

struct UnaryExp_Obj : BNode {
    bool negative;

    explicit UnaryExp_Obj(PartType p);

    //BNode *getTarget();
};


#endif //E5_MIPS_REWORK_UNARYEXP_OBJ_H
