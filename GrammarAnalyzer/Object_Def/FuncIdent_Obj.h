//
// Created by SNAPDRAGON_11800H on 12/17/2021.
//

#ifndef E5_MIPS_REWORK_FUNCIDENT_OBJ_H
#define E5_MIPS_REWORK_FUNCIDENT_OBJ_H

#include "../BNode.h"

struct FuncIdent_Obj : BNode {
    string *name = nullptr; // Func Param model
    Keyword returnType;
    vector<BNode *> Block;
    vector<BNode *> funcParam;

    FuncIdent_Obj(Keyword k, string *t, PartType p);

    FuncIdent_Obj(string *t, PartType p);

    string *getTarget() override;

    ~FuncIdent_Obj() override = default;
};

#endif //E5_MIPS_REWORK_FUNCIDENT_OBJ_H
