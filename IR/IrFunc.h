//
// Created by SNAPDRAGON_11800H on 17/11/2021.
//

#ifndef E5_MIPS_REWORK_IRFUNC_H
#define E5_MIPS_REWORK_IRFUNC_H

#include "LinkedList_IR.h"
#include "BlockEntry.h"
#include "VarClass.h"
#include "Instr.h"

class BlockEntry;
class DeclInst;
class VarClass;
class IrParam;
class Func;
class Decl;
class IrFunc {
public:
    IrFunc();
    explicit IrFunc(Func* func);

    void addBlock(BlockEntry* blk);
    void addDecl(Decl* d, DeclInst* decl);
    int genVar();
    int genArray(int size);

    int getVarId() const;
    TypeDefine getType();
    string getName();
    VarClass* getVar(Decl* decl);
    BlockEntry* getEntryBlock() const;

    string getIR();
private:
    int varId;

    LinkedList_IR<BlockEntry> blocks;
    string funcName;
    vector<IrParam*> params;
    map<Decl*, VarClass*> table;
    TypeDefine returnType;
};


#endif //E5_MIPS_REWORK_IRFUNC_H
