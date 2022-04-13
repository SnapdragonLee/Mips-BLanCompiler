//
// Created by SNAPDRAGON_11800H on 2021/11/24.
//

#ifndef E4_INTERMEDIATE_REPRESENTATION_MIPSGENERATE_H
#define E4_INTERMEDIATE_REPRESENTATION_MIPSGENERATE_H

#include "../include_Base/Base_settings.h"
#include "../IR/IR_Old.h"

class MipsGenerate {
    IR_Old *ir = nullptr;

    MipsGenerate(IR_Old *b);


public:
    static MipsGenerate &getMIPS(IR_Old *b);

    string Output_MIPS;

    string &getMIPSResult();
};


#endif //E4_INTERMEDIATE_REPRESENTATION_MIPSGENERATE_H
