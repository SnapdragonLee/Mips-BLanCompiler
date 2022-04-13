//
// Created by SNAPDRAGON_11800H on 12/17/2021.
//

#ifndef E5_MIPS_REWORK_VARIDENT_OBJ_H
#define E5_MIPS_REWORK_VARIDENT_OBJ_H

#include "../BNode.h"

struct VarIdent_Obj : BNode {
    string *name = nullptr; // Const Ident model
    BNode *column = nullptr, *row = nullptr, *init = nullptr;
    int dim;
    bool canEdit = true, is_define = false;

    explicit VarIdent_Obj(string *t, PartType p, bool define);

    string *getTarget() override;;

    ~VarIdent_Obj() override = default;
};


#endif //E5_MIPS_REWORK_VARIDENT_OBJ_H
