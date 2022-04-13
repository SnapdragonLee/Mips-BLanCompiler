//
// Created by SNAPDRAGON_11800H on 12/17/2021.
//

#ifndef E5_MIPS_REWORK_MULEXP_OBJ_H
#define E5_MIPS_REWORK_MULEXP_OBJ_H

#include "../BNode.h"
#include "Operation_Obj.h"

struct MulExp_Obj : BNode {
    BNode *src1 = nullptr;
    BNode *src2 = nullptr;
    Operation_Obj *op = nullptr;

    explicit MulExp_Obj(PartType p);

    void addsrc(BNode *s1, BNode *op, BNode *s2);
};


#endif //E5_MIPS_REWORK_MULEXP_OBJ_H
