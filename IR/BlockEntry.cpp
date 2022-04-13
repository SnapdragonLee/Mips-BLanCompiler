//
// Created by SNAPDRAGON_11800H on 17/11/2021.
//

#include "BlockEntry.h"

BlockEntry::BlockEntry() {
    this->prev = nullptr;
    this->next = nullptr;
    this->label_id = 0;
}

BlockEntry::BlockEntry(int id) {
    this->prev = nullptr;
    this->next = nullptr;
    this->label_id = id;
}

void BlockEntry::addInst(Inst *inst) {
    insts.push_back(inst);
}

int BlockEntry::getId() const {
    return label_id;
}

Inst *BlockEntry::getLastInst() const {
    return insts.t;
}

Inst *BlockEntry::getEntryInst() const {
    return insts.h;
}


string BlockEntry::getIR() const {
    string res;
    res += "label_" + to_string(label_id) + ":\n";
    for (Inst *inst = insts.h; inst; inst = inst->next) {
        res += inst->getIR() + "\n";
    }
    return res;
}