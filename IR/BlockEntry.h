//
// Created by SNAPDRAGON_11800H on 17/11/2021.
//

#ifndef E5_MIPS_REWORK_BLOCKENTRY_H
#define E5_MIPS_REWORK_BLOCKENTRY_H

#include "Instr.h"
#include "LinkedList_IR.h"

class Inst;

class BlockEntry {
public:
    BlockEntry *prev, *next;

    BlockEntry();

    explicit BlockEntry(int id);

    void addInst(Inst *inst);

    Inst *getEntryInst() const;

    Inst *getLastInst() const;

    int getId() const;

    string getIR() const;

private:
    int label_id;

    LinkedList_IR<Inst> insts;
};


#endif //E5_MIPS_REWORK_BLOCKENTRY_H
