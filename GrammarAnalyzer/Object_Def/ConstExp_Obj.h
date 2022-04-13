//
// Created by SNAPDRAGON_11800H on 12/17/2021.
//

#ifndef E5_MIPS_REWORK_CONSTEXP_H
#define E5_MIPS_REWORK_CONSTEXP_H

#include "../BNode.h"

struct ConstExp_Obj : BNode {

    bool is_calc = true;

    explicit ConstExp_Obj(PartType e);

};

#endif //E5_MIPS_REWORK_CONSTEXP_H
