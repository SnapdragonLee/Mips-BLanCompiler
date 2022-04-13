//
// Created by SNAPDRAGON_11800H on 12/17/2021.
//

#ifndef E5_MIPS_REWORK_OPERATION_OBJ_H
#define E5_MIPS_REWORK_OPERATION_OBJ_H

#include "../BNode.h"

struct Operation_Obj : BNode {
    Keyword relation;       // Operation

    explicit Operation_Obj(Keyword k, PartType e);

    string *getTarget() override;;

    ~Operation_Obj() override = default;
};


#endif //E5_MIPS_REWORK_OPERATION_OBJ_H
