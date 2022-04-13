//
// Created by SNAPDRAGON_11800H on 12/17/2021.
//

#ifndef E5_MIPS_REWORK_CONSTNUM_OBJ_H
#define E5_MIPS_REWORK_CONSTNUM_OBJ_H

#include "../BNode.h"

struct ConstNum_Obj : BNode {
    string *n;                  // num

    explicit ConstNum_Obj(string *t) : BNode(Number) {
        n = t;
    }

    string *getTarget() override {
        return n;
    };

    ~ConstNum_Obj() override = default;
};


#endif //E5_MIPS_REWORK_CONSTNUM_OBJ_H
