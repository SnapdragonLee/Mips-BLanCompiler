//
// Created by SNAPDRAGON_11800H on 12/17/2021.
//

#ifndef E5_MIPS_REWORK_CONSTIDENT_OBJ_H
#define E5_MIPS_REWORK_CONSTIDENT_OBJ_H

#include "../BNode.h"

struct ConstIdent_Obj : BNode {
    string *name = nullptr; // Const Ident model
    BNode *column = nullptr, *row = nullptr, *init = nullptr;
    int dim = 0;
    bool canEdit = false;

    explicit ConstIdent_Obj(string *t, PartType p);

    string *getTarget() override;

    ~ConstIdent_Obj() override = default;
};

#endif //E5_MIPS_REWORK_CONSTIDENT_OBJ_H
