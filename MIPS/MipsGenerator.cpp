//
// Created by SNAPDRAGON_11800H on 19/11/2021.
//

#include "MipsGenerator.h"

string branchOp[] = {
        "b",
        "blt",
        "ble",
        "bgt",
        "bge",
        "beq",
        "bne",
        "bnez",
        "beqz"
};

MipsGenerator::MipsGenerator() {
    this->str_id = 1;
    this->module = nullptr;
}

MipsGenerator::MipsGenerator(Module *module) {
    this->str_id = 1;
    this->module = module;
    prodDataSegment();
    prodTextSegment();
}

string MipsGenerator::prodWordTag(int id, VarClass *init) {
    if (init) return "global_" + to_string(id) + ": .word " + init->getIR();
    return "global_" + to_string(id) + ": .word 0";
}

string MipsGenerator::prodAsciizTag(const string &s) {
    strMap[s] = str_id;
    return "str_" + to_string(str_id++) + ": .asciiz \"" + s + "\"";
}

string MipsGenerator::prodLabelTag(int id) {
    return "label_" + to_string(id) + ":";
}

string MipsGenerator::prodArrayTag(IrArray *arr) {
    return "global_" + to_string(arr->getId()) + ": .space " + to_string(arr->getSize() << 2);
}

void MipsGenerator::prodDataSegment() {
    mips.emplace_back(".data");

    // prodGlobals
    auto decls = module->decls;
    for (auto x: decls) {
        auto var = x->getVar();
        if (auto arr = dynamic_cast<IrArray *>(var)) {
            auto res = prodArrayTag(arr);
            mips.emplace_back(res);
            if (x->hasInit()) {
                mips.emplace_back(".text");
                int n = arr->getSize();
                loadAddr(arr, 2);
                for (int i = 0; i < n; i++) {
                    auto init = x->getInits()[i];
                    if (auto constant = dynamic_cast<Constant *>(init)) {
                        loadConst(constant, 0);
                    } else {
                        loadVar(init, 0);
                    }
                    mips.emplace_back("sw $t0, ($t2)");
                    mips.emplace_back("addiu $t2, $t2, 4");
                }
                mips.emplace_back(".data");
            }
        } else {
            auto res = prodWordTag(x->getVar()->getId(), x->getInit());
            mips.emplace_back(res);
        }
    }

    // prodStrings
    auto strings = module->strings;
    for (auto &x: strings) {
        string res = prodAsciizTag(x);
        mips.emplace_back(res);
    }
}

string getLabel(int id) {
    return "label_" + to_string(id);
}

string getFuncVars(int num) {
    return to_string(num << 2);
}

void MipsGenerator::prodTextSegment() {
    mips.emplace_back("\n.text");

    auto funcs = module->irFuncs;
    auto label = getLabel(funcs[0]->getEntryBlock()->getId());
    auto args = getFuncVars(funcs[0]->getVarId());
    mips.emplace_back("addiu $sp, $sp, -" + args);
    mips.emplace_back("jal " + label);
    mips.emplace_back("li $v0, 10");
    mips.emplace_back("syscall");
    for (auto x: funcs) {
        prodFuncDef(x);
    }
}

void MipsGenerator::prodInst(Inst *inst) {
    if (auto binaryInst = dynamic_cast<BinaryInst *>(inst)) {
        prodBinaryInst(binaryInst);
    } else if (auto assignInst = dynamic_cast<AssignInst *>(inst)) {
        prodAssignInst(assignInst);
    } else if (auto branchInst = dynamic_cast<BranchInst *>(inst)) {
        prodBranchInst(branchInst);
    } else if (auto jumpInst = dynamic_cast<JumpInst *>(inst)) {
        prodJumpInst(jumpInst);
    } else if (auto returnInst = dynamic_cast<ReturnInst *>(inst)) {
        prodReturnInst(returnInst);
    } else if (auto callInst = dynamic_cast<CallInst *>(inst)) {
        prodCallInst(callInst);
    } else if (auto loadAddrInst = dynamic_cast<LoadAddrInst *>(inst)) {
        prodLoadAddrInst(loadAddrInst);
    } else if (auto loadInst = dynamic_cast<LoadInst *>(inst)) {
        prodLoadInst(loadInst);
    } else if (auto storeInst = dynamic_cast<StoreInst *>(inst)) {
        prodStoreInst(storeInst);
    } else if (auto declInst = dynamic_cast<DeclInst *>(inst)) {
        prodDeclInst(declInst);
    } else if (auto getReturnInst = dynamic_cast<GetReturnInst *>(inst)) {
        prodGetReturnInst(getReturnInst);
    } else if (auto notInst = dynamic_cast<NotInst *>(inst)) {
        prodNotInst(notInst);
    }
}

void MipsGenerator::prodFuncDef(IrFunc *func) {
    mips.emplace_back("# " + func->getName());

    auto blk = func->getEntryBlock();
    while (blk) {
        prodBlock(blk);
        blk = blk->next;
    }
}

void MipsGenerator::prodBlock(BlockEntry *block) {
    mips.emplace_back(prodLabelTag(block->getId()));
    auto inst = block->getEntryInst();
    while (inst) {
        prodInst(inst);
        inst = inst->next;
    }
}

void MipsGenerator::loadVar(VarClass *var, int reg) {
    string t = "$t" + to_string(reg);
    if (var->isGlobal()) {
        string id = to_string(var->getId());
        mips.emplace_back("lw " + t + ", global_" + id);
    } else {
        string id = to_string(var->getId() << 2);
        mips.emplace_back("lw " + t + ", " + id + "($sp)");
    }
}

void MipsGenerator::loadConst(Constant *var, int reg) {
    string t = "$t" + to_string(reg);
    string val = to_string(var->getValue());
    mips.emplace_back("li " + t + ", " + val);
}

void MipsGenerator::assign(VarClass *var, int reg) {
    string t = "$t" + to_string(reg);
    if (var->isGlobal()) {
        string id = to_string(var->getId());
        mips.emplace_back("sw " + t + ", global_" + id);
    } else {
        string id = to_string(var->getId() << 2);
        mips.emplace_back("sw " + t + ", " + id + "($sp)");
    }
}

void MipsGenerator::loadAddr(VarClass *base, int reg) {
    string r = "$t" + to_string(reg);
    if (base->isGlobal()) {
        mips.emplace_back("la " + r + ", global_" + to_string(base->getId()));
    } else {
        mips.emplace_back("la " + r + ", " + to_string(base->getId() << 2) + "($sp)");
    }
}

void MipsGenerator::loadWord(int addr, int reg) {
    string r = "$t" + to_string(reg);
    string a = "$t" + to_string(addr);
    mips.emplace_back("lw " + r + ", (" + a + ")");
}

void MipsGenerator::storeWord(int addr, int reg) {
    string r = "$t" + to_string(reg);
    string a = "$t" + to_string(addr);
    mips.emplace_back("sw " + r + ", (" + a + ")");
}

void MipsGenerator::prodCallInst(CallInst *inst) {
    if (inst->sym.sym == PRINTFTK) {
        for (auto x: inst->params) {
            auto arg = dynamic_cast<IrParam *>(x);
            if (arg->constant) {
                auto constant = arg->constant;
                if (constant->type == STRCON) {
                    auto id = to_string(strMap[arg->constant->str]);
                    mips.emplace_back("la $a0, str_" + id);
                    mips.emplace_back("addiu $v0, $0, 4");
                    mips.emplace_back("syscall");
                } else {
                    auto val = to_string(constant->getValue());
                    mips.emplace_back("li $a0, " + val);
                    mips.emplace_back("addiu $v0, $0, 1");
                    mips.emplace_back("syscall");
                }
            } else {
                if (arg->isGlobal()) {
                    auto id = to_string(arg->getId());
                    mips.emplace_back("lw $a0, global_" + id);
                } else {
                    auto id = to_string(arg->getId() << 2);
                    mips.emplace_back("lw $a0, " + id + "($sp)");
                }
                mips.emplace_back("addiu $v0, $0, 1");
                mips.emplace_back("syscall");
            }
        }
    } else if (inst->sym.sym == GETINTTK) {
        mips.emplace_back("addiu $v0, $0, 5");
        mips.emplace_back("syscall");
    } else {
        int arg = inst->func->getVarId();
        auto args = to_string(arg << 2);
        auto label = to_string(inst->func->getEntryBlock()->getId());
        int id = 0;
        for (auto x: inst->params) {
            auto str_id = to_string(4 * (id - arg - 1));
            if (auto var = dynamic_cast<Constant *>(x)) {
                loadConst(var, 0);
            } else {
                loadVar(x, 0);
            }
            mips.emplace_back("sw $t0, " + str_id + "($sp)");
            id++;
        }
        mips.emplace_back("addiu $sp, $sp, -4");
        mips.emplace_back("sw $ra, ($sp)");
        if (arg) mips.emplace_back("addiu $sp, $sp, -" + args);
        mips.emplace_back("jal label_" + label);
        if (arg) mips.emplace_back("addiu $sp, $sp, " + args);
        mips.emplace_back("lw $ra, ($sp)");
        mips.emplace_back("addiu $sp, $sp, 4");
    }
}


void MipsGenerator::prodBinaryInst(BinaryInst *inst) {
    auto id = to_string(inst->var->getId() << 2);
    auto lhs = inst->lhs;
    auto rhs = inst->rhs;

    auto left = dynamic_cast<Constant *>(lhs);
    auto right = dynamic_cast<Constant *>(rhs);

    if (left && right) {
        int x = left->getValue();
        int y = right->getValue();
        int t;
        switch (inst->op) {
            case Add:
                t = x + y;
                break;
            case Sub:
                t = x - y;
                break;
            case Mul:
                t = x * y;
                break;
            case Div:
                t = x / y;
                break;
            case Mod:
                t = x % y;
                break;
            case And:
                t = x && y;
                break;
            case Or:
                t = x || y;
                break;
            case Slt:
                t = x < y;
                break;
            case Sle:
                t = x <= y;
                break;
            case Sgt:
                t = x > y;
                break;
            case Sge:
                t = x >= y;
                break;
            case Seq:
                t = x == y;
                break;
            case Sne:
                t = x != y;
                break;
            default:
                t = 0;
                break;
        }
        mips.emplace_back("addiu $t0, $0, " + to_string(t));
        // save it to stack
        assign(inst->var, 0);
    } else if (left) {
        loadConst(left, 0);
        loadVar(rhs, 1);
        switch (inst->op) {
            case Add:
                mips.emplace_back("addu $t0, $t0, $t1");
                break;
            case Sub:
                mips.emplace_back("subu $t0, $t0, $t1");
                break;
            case Mul:
                mips.emplace_back("mul $t0, $t0, $t1");
                break;
            case Div:
                mips.emplace_back("div $t0, $t0, $t1");
                break;
            case Mod:
                mips.emplace_back("div $t0, $t1");
                mips.emplace_back("mfhi $t0");
                break;
            case And:
                mips.emplace_back("and $t0, $t0, $t1");
                break;
            case Or:
                mips.emplace_back("or  $t0, $t0, $t1");
                break;
            case Slt:
                mips.emplace_back("slt $t0, $t0, $t1");
                break;
            case Sle:
                mips.emplace_back("sle $t0, $t0, $t1");
                break;
            case Sgt:
                mips.emplace_back("sgt $t0, $t0, $t1");
                break;
            case Sge:
                mips.emplace_back("sge $t0, $t0, $t1");
                break;
            case Seq:
                mips.emplace_back("seq $t0, $t0, $t1");
                break;
            case Sne:
                mips.emplace_back("sne $t0, $t0, $t1");
                break;
            default:
                break;
        }
        // save it to stack
        assign(inst->var, 0);
    } else if (right) {
        loadVar(lhs, 0);
        auto x = to_string(right->getValue());
        switch (inst->op) {
            case Add:
                mips.emplace_back("addiu $t0, $t0, " + x);
                break;
            case Sub:
                mips.emplace_back("addiu $t0, $t0, -" + x);
                break;
            case Mul:
                mips.emplace_back("mul $t0, $t0, " + x);
                break;
            case Div:
                mips.emplace_back("div $t0, $t0, " + x);
                break;
            case Mod:
                mips.emplace_back("div $t0, $t0, " + x);
                mips.emplace_back("mfhi $t0");
                break;
            case And:
                mips.emplace_back("and $t0, $t0, " + x);
                break;
            case Or:
                mips.emplace_back("or  $t0, $t0, " + x);
                break;
            case Slt:
                mips.emplace_back("slti $t0, $t0, " + x);
                break;
            case Sle:
                mips.emplace_back("sle $t0, $t0, " + x);
                break;
            case Sgt:
                mips.emplace_back("sgt $t0, $t0, " + x);
                break;
            case Sge:
                mips.emplace_back("sge $t0, $t0, " + x);
                break;
            case Seq:
                mips.emplace_back("seq $t0, $t0, " + x);
                break;
            case Sne:
                mips.emplace_back("sne $t0, $t0, " + x);
                break;
            default:
                break;
        }
        assign(inst->var, 0);
        // save to stack

    } else {
        loadVar(lhs, 0);
        loadVar(rhs, 1);
        switch (inst->op) {
            case Add:
                mips.emplace_back("addu $t0, $t0, $t1");
                break;
            case Sub:
                mips.emplace_back("subu $t0, $t0, $t1");
                break;
            case Mul:
                mips.emplace_back("mul $t0, $t0, $t1");
                break;
            case Div:
                mips.emplace_back("div $t0, $t0, $t1");
                break;
            case Mod:
                mips.emplace_back("div $t0, $t1");
                mips.emplace_back("mfhi $t0");
                break;
            case Or:
                mips.emplace_back("or  $t0, $t0, $t1");
                break;
            case And:
                mips.emplace_back("and $t0, $t0, $t1");
                break;
            case Slt:
                mips.emplace_back("slt $t0, $t0, $t1");
                break;
            case Sne:
                mips.emplace_back("sne $t0, $t0, $t1");
                break;
            case Sle:
                mips.emplace_back("sle $t0, $t0, $t1");
                break;
            case Sgt:
                mips.emplace_back("sgt $t0, $t0, $t1");
                break;
            case Sge:
                mips.emplace_back("sge $t0, $t0, $t1");
                break;
            case Seq:
                mips.emplace_back("seq $t0, $t0, $t1");
                break;
            default:
                break;
        }
        // save it to stack
        assign(inst->var, 0);
    }
}

void MipsGenerator::prodAssignInst(AssignInst *inst) {
    auto lhs = inst->lhs;
    auto rhs = inst->rhs;

    if (auto constant = dynamic_cast<Constant *>(rhs)) {
        loadConst(constant, 0);
        assign(lhs, 0);
    } else {
        loadVar(rhs, 0);
        assign(lhs, 0);
    }
}

void MipsGenerator::prodReturnInst(ReturnInst *inst) {
    if (inst->var) {
        VarClass *var = inst->var;
        if (auto constant = dynamic_cast<Constant *>(var)) {
            auto x = to_string(constant->getValue());
            mips.emplace_back("addiu $v0, $0, " + x);
        } else {
            auto id = to_string(var->getId() << 2);
            mips.emplace_back("lw $v0, " + id + "($sp)");
        }
    }
    mips.emplace_back("jr $ra");
}

void MipsGenerator::prodGetReturnInst(GetReturnInst *inst) {
    VarClass *var;
    var = inst->var;
    string id;
    id = to_string(var->getId() << 2);
    mips.emplace_back("addu $t0, $0, $v0");
    assign(inst->var, 0);
}

void MipsGenerator::prodBranchInst(BranchInst *inst) {
    if (auto constant = dynamic_cast<Constant *>(inst->var)) {
        loadConst(constant, 0);
    } else {
        loadVar(inst->var, 0);
    }
    string id = to_string(inst->label_id);
    mips.emplace_back(branchOp[inst->op] + " $t0, label_" + id);
}

void MipsGenerator::prodJumpInst(JumpInst *inst) {
    string id = to_string(inst->label_id);
    mips.emplace_back("j label_" + id);
}

void MipsGenerator::prodNotInst(NotInst *inst) {
    if (auto constant = dynamic_cast<Constant *>(inst->var)) {
        loadConst(constant, 0);
    } else {
        loadVar(inst->var, 0);
    }
    mips.emplace_back("seq $t0, $t0, 0");
    assign(inst->not_var, 0);
}

void MipsGenerator::prodLoadAddrInst(LoadAddrInst *inst) {
    loadAddr(inst->base, 2);
    assign(inst->var, 2);
}

void MipsGenerator::prodLoadInst(LoadInst *inst) {
    loadVar(inst->addr, 2);
    loadWord(2, 3);
    auto dst = inst->dst;
    if (dst->isGlobal()) {
        string id = to_string(dst->getId());
        mips.emplace_back("sw $t3, global_" + id);
    } else {
        string id = to_string(dst->getId() << 2);
        mips.emplace_back("sw $t3, " + id + "($sp)");
    }
}

void MipsGenerator::prodStoreInst(StoreInst *inst) { // todo: Don't touch this
    loadVar(inst->val, 2);
    loadVar(inst->addr, 3);
    storeWord(3, 2);
}

void MipsGenerator::prodDeclInst(DeclInst *inst) { //todo: need generate more methods
    /*VarClass *init;
    init = inst->getInit();
    if (auto param = dynamic_cast<IrParam *>(inst->getVar())) {
        // do nothing
    } else if (auto arr = dynamic_cast<IrArray *>(inst->getVar())) {
        int n = arr->getSize();
        loadAddr(arr, 2);
        for (int i = 0; i < n; i++) {
            if (inst->hasInit()) {
                auto x = inst->getInits()[i];
                if (auto constant = dynamic_cast<Constant *>(x)) {
                    if (constant->value != 0) {
                        loadConst(constant, 0);
                        mips.emplace_back("sw $t0, ($t2)");
                    }
                } else {
                    loadVar(x, 0);
                    mips.emplace_back("sw $t0, ($t2)");
                }
            }*//* else {
                mips.emplace_back("li $t0, 0");
                mips.emplace_back("sw $t0, ($t2)");
            }*//*
            mips.emplace_back("addiu $t2, $t2, 4");
        }

    } else if (init == nullptr) {
        VarClass *var;
        var = inst->getVar();
        string id;
        id = to_string(var->getId() << 2);
        mips.emplace_back("sw $0, " + id + "($sp)");
    } else if (auto constant = dynamic_cast<Constant *>(init)) {
        loadConst(constant, 0);
        assign(inst->getVar(), 0);
    } else {
        loadVar(init, 0);
        assign(inst->getVar(), 0);
    }*/
    auto init = inst->getInit();
    if (auto param = dynamic_cast<IrParam*>(inst->getVar())) {
        // do nothing
    } else if (auto arr = dynamic_cast<IrArray*>(inst->getVar())) {
        int n = arr->getSize();
        loadAddr(arr, 2);
        for (int i = 0; i < n; i++) {
            if (inst->hasInit()) {
                auto x = inst->getInits()[i];
                if (auto constant = dynamic_cast<Constant*>(x)) {
                    loadConst(constant, 0);
                } else {
                    loadVar(x, 0);
                }
                mips.emplace_back("sw $t0, ($t2)");
            } else {
                mips.emplace_back("li $t0, 0");
                mips.emplace_back("sw $t0, ($t2)");
            }
            mips.emplace_back("addiu $t2, $t2, 4");
        }
    } else if (init == nullptr) {
        auto var = inst->getVar();
        auto id = to_string(var->getId() << 2);
        mips.emplace_back("sw $0, " + id + "($sp)");
    } else if (auto constant = dynamic_cast<Constant*>(init)) {
        loadConst(constant, 0);
        assign(inst->getVar(), 0);
    } else {
        loadVar(init, 0);
        assign(inst->getVar(), 0);
    }

}


string &MipsGenerator::getMIPSResult() {
#ifdef MIPSGenerate
    for (auto &x: mips) {
        output += x + "\n";
    }
#endif
    return output;
}
