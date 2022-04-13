//
// Created by SNAPDRAGON_11800H on 2021/11/24.
//

#include "MipsGenerate.h"


string &MipsGenerate::getMIPSResult() {
    return Output_MIPS;
}

MipsGenerate &MipsGenerate::getMIPS(IR_Old *b) {
    static MipsGenerate init(b);
    return init;
}


MipsGenerate::MipsGenerate(IR_Old *b) {
    ir = b;

}
