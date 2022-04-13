//
// Created by SNAPDRAGON_11800H on 17/11/2021.
//

#include "Ctx.h"

Ctx::Ctx() {
    this->blk = nullptr;
    this->if_blk = nullptr;
    this->while_blk = nullptr;
    this->module = nullptr;
    this->expStmt = nullptr;
    this->if_body_blk = nullptr;
    this->while_body_blk = nullptr;
    this->while_end_blk = nullptr;
    this->func = nullptr;
    this->table = nullptr;
    this->if_end_blk = nullptr;
    this->isParam = false;
    this->isCond = false;

    this->global_id = 1;
    this->label_id = 1;
}

int Ctx::genGlobal() {
    return global_id++;
}

int Ctx::genLabel() {
    return label_id++;
}

void Ctx::addConst(Decl *decl) {
    const_map[decl] = new Constant(decl->getInitVal()[0]);
}

void Ctx::addLabel(int id, BlockEntry *blk) {
    labels[id] = blk;
}

void Ctx::getWhile() {
    this->while_blk = while_blks.empty() ? nullptr : while_blks.top();
    this->while_body_blk = while_body_blks.empty() ? nullptr : while_body_blks.top();
    this->while_end_blk = while_end_blks.empty() ? nullptr : while_end_blks.top();
}

void Ctx::popWhile() {
    while_blks.pop();
    while_body_blks.pop();
    while_end_blks.pop();
    getWhile();
}

IrFunc *Ctx::getFunc(const string &name) {
    return module->getFunc(name);
}


void Ctx::pushWhile(BlockEntry *while_blk, BlockEntry *body_blk, BlockEntry *while_end_blk) {
    while_blks.push(while_blk);
    while_body_blks.push(body_blk);
    while_end_blks.push(while_end_blk);
    getWhile();
}

int Ctx::genArray() {
    return global_id++;
}

