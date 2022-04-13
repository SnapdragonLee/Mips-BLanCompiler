//
// Created by SNAPDRAGON_11800H on 17/11/2021.
//

#ifndef E5_MIPS_REWORK_CTX_H
#define E5_MIPS_REWORK_CTX_H

#include "BlockEntry.h"
#include "../GrammarAnalyzer/TableList.h"
#include "../GrammarAnalyzer/AstClass.h"

class TableList;

class IrFunc;

class Module;

class ExpStmt;

class Constant;

class BlockEntry;

class Ctx {
public:
    Ctx();

    TableList *table;
    IrFunc *func;
    Module *module;
    BlockEntry *blk;
    BlockEntry *if_blk;
    BlockEntry *if_body_blk;
    BlockEntry *else_blk;
    BlockEntry *if_end_blk;
    BlockEntry *while_blk;
    BlockEntry *while_body_blk;
    BlockEntry *while_end_blk;
    ExpStmt *expStmt;
    bool isParam;
    bool isCond;

    void addConst(Decl *decl);

    void addLabel(int id, BlockEntry *blk);

    int genGlobal();

    int genArray();

    int genLabel();

    IrFunc *getFunc(const string &name);

    void pushWhile(BlockEntry *while_blk, BlockEntry *body_blk, BlockEntry *while_end_blk);

    void popWhile();

    map<Decl *, Constant *> const_map;
    map<int, BlockEntry *> labels;
private:
    stack<BlockEntry *> while_blks;
    stack<BlockEntry *> while_body_blks;
    stack<BlockEntry *> while_end_blks;
    int global_id, label_id;

    void getWhile();
};


#endif //E5_MIPS_REWORK_CTX_H
