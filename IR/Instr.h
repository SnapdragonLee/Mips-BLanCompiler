//
// Created by SNAPDRAGON_11800H on 17/11/2021.
//

#ifndef E5_MIPS_REWORK_INSTR_H
#define E5_MIPS_REWORK_INSTR_H

#include "../include_Base/Base_settings.h"
#include "../GrammarAnalyzer/TypeDefine.h"
#include "../GrammarAnalyzer/AstClass.h"
#include "IrFunc.h"

enum BinaryOp {
    Add, Sub, Mul, Div, Mod, And, Or,
    Slt, Sle, Sgt, Sge, Seq, Sne
};

class Exp;

class IrFunc;

class BinaryExp;

class Inst {
public:
    Inst *prev, *next;

    Inst();

    virtual string getIR();
};

class BinaryInst : public Inst {
public:
    BinaryOp op;
    VarClass *var, *lhs, *rhs;

    BinaryInst();

    explicit BinaryInst(VarClass *var, BinaryExp *exp);

    BinaryInst(VarClass *var, BinaryOp op, Exp *exp);

    BinaryInst(VarClass *var, BinaryOp op, VarClass *lhs, VarClass *rhs);

    BinaryInst(VarClass *var, VarClass *lhs, Element op, VarClass *rhs);

    string getIR() override;
};

class AssignInst : public Inst {
public:
    VarClass *lhs, *rhs;

    AssignInst();

    AssignInst(VarClass *lhs, VarClass *rhs);

    string getIR() override;
};

enum BranchOp {
    Br, Blt, Ble, Bgt, Bge, Beq, Bne, Bnez, Beqz
};

class BranchInst : public Inst {
public:
    BranchOp op;
    VarClass *var;
    int label_id;

    BranchInst();

    BranchInst(BranchOp op, VarClass *var, int label_id);

    string getIR() override;
};

class JumpInst : public Inst {
public:
    int label_id;

    JumpInst();

    JumpInst(int label_id);

    string getIR() override;
};

class ReturnInst : public Inst {
public:
    VarClass *var;

    ReturnInst();

    explicit ReturnInst(VarClass *var);

    string getIR() override;
};

class CallInst : public Inst {
public:
    Element sym;
    IrFunc *func;
    vector<VarClass *> params;

    CallInst();

    CallInst(Element sym);

    CallInst(Element sym, vector<VarClass *> &params);

    CallInst(IrFunc *func);

    CallInst(IrFunc *func, vector<VarClass *> &params);

    string getIR() override;
};

class NotInst : public Inst {
public:
    VarClass *var, *not_var;

    NotInst();

    NotInst(VarClass *var, VarClass *not_var);

    string getIR() override;
};

class IrArray;

class LoadAddrInst : public Inst {
public:
    VarClass *var, *base;

    LoadAddrInst();

    LoadAddrInst(VarClass *var, VarClass *base);

    string getIR() override;
};

class LoadInst : public Inst {
public:
    VarClass *dst, *addr;

    LoadInst();

    LoadInst(VarClass *dst, VarClass *addr);

    string getIR() override;
};

class StoreInst : public Inst {
public:
    VarClass *addr, *val;

    StoreInst();

    StoreInst(VarClass *val, VarClass *addr);

    string getIR() override;
};

class DeclInst : public Inst {
private:
    VarClass *var, *init;
    vector<VarClass *> inits;

public:
    DeclInst();

    DeclInst(VarClass *var);

    VarClass *getVar();

    VarClass *getInit();

    vector<VarClass *> getInits();

    void addInit(VarClass *init);

    void addInits(vector<VarClass *> inits);

    bool hasInit();

    string getIR() override;

};

class GetReturnInst : public Inst {
public:
    VarClass *var;

    GetReturnInst();

    GetReturnInst(VarClass *var);

    string getIR() override;
};

#endif //E5_MIPS_REWORK_INSTR_H
