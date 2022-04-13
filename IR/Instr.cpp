//
// Created by SNAPDRAGON_11800H on 17/11/2021.
//

#include "Instr.h"
#include "VarClass.h"

Inst::Inst() {
    prev = next = nullptr;
}

string Inst::getIR() {
    return "";
}

BinaryInst::BinaryInst(VarClass *var, BinaryExp *exp) : Inst() {
    switch (exp->getOp().sym) {
        case PLUS:
            this->op = Add;
            break;
        case MINU:
            this->op = Sub;
            break;
        case MULT:
            this->op = Mul;
            break;
        case DIV:
            this->op = Div;
            break;
        case MOD:
            this->op = Mod;
            break;
        case LSS:
            this->op = Slt;
            break;
        case LEQ:
            this->op = Sle;
            break;
        case GRE:
            this->op = Sgt;
            break;
        case GEQ:
            this->op = Sge;
            break;
        case EQL:
            this->op = Seq;
            break;
        case NEQ:
            this->op = Sne;
            break;
        case AND:
            this->op = And;
            break;
        case OR:
            this->op = Or;
            break;
        default:   /* bug */       break;
    }
    this->var = var;
    lhs = exp->getLhs()->getVar();
    rhs = exp->getRhs()->getVar();
}

BinaryInst::BinaryInst() : Inst() {
    this->var = nullptr;
    this->lhs = nullptr;
    this->rhs = nullptr;
}


BinaryInst::BinaryInst(VarClass *var, BinaryOp op, Exp *exp) {
    this->op = op;
    this->var = var;
    this->lhs = new Constant(0);
    this->rhs = exp->getVar();
}

BinaryInst::BinaryInst(VarClass *var, VarClass *lhs, Element op, VarClass *rhs) {
    switch (op.sym) {
        case PLUS:
            this->op = Add;
            break;
        case MINU:
            this->op = Sub;
            break;
        case MULT:
            this->op = Mul;
            break;
        case DIV:
            this->op = Div;
            break;
        case MOD:
            this->op = Mod;
            break;
        case LSS:
            this->op = Slt;
            break;
        case LEQ:
            this->op = Sle;
            break;
        case GRE:
            this->op = Sgt;
            break;
        case GEQ:
            this->op = Sge;
            break;
        case EQL:
            this->op = Seq;
            break;
        case NEQ:
            this->op = Sne;
            break;
        case AND:
            this->op = And;
            break;
        case OR:
            this->op = Or;
            break;
        default:   /* bug */       break;
    }
    this->var = var;
    this->lhs = lhs;
    this->rhs = rhs;
}

string BinaryInst::getIR() {
    string ops[] = {"+", "-", "*", "/", "%", "&&", "||",
                    "<", "<=", ">", ">=", "==", "!="};
    return var->getIR() + " = " + lhs->getIR() + " " + ops[op] + " " + rhs->getIR();
}

AssignInst::AssignInst() : Inst() {
    this->lhs = nullptr;
    this->rhs = nullptr;
}

AssignInst::AssignInst(VarClass *lhs, VarClass *rhs) : Inst() {
    this->lhs = lhs;
    this->rhs = rhs;
}

JumpInst::JumpInst() {
    this->label_id = 0;
}

JumpInst::JumpInst(int label_id) {
    this->label_id = label_id;
}

string JumpInst::getIR() {
    return "Jump label_" + to_string(label_id);
}

string AssignInst::getIR() {
    return lhs->getIR() + " = " + rhs->getIR();
}

string BranchInst::getIR() {
    string out[] = {"Br", "Blt", "Ble", "Bgt", "Bge", "Beq", "Bne", "Bnez", "Beqz"};
    return out[op] + " " + var->getIR() + ", label_" + to_string(label_id);
}

BranchInst::BranchInst() : Inst() {

}

BinaryInst::BinaryInst(VarClass *var, BinaryOp op, VarClass *lhs, VarClass *rhs) {
    this->var = var;
    this->op = op;
    this->lhs = lhs;
    this->rhs = rhs;
}

BranchInst::BranchInst(BranchOp op, VarClass *var, int label_id) : Inst() {
    this->op = op;
    this->var = var;
    this->label_id = label_id;
}

ReturnInst::ReturnInst() : Inst() {
    this->var = nullptr;
}

ReturnInst::ReturnInst(VarClass *var) {
    this->var = var;
}

string ReturnInst::getIR() {
    if (var) return "ret " + var->getIR();
    return "ret";
}

CallInst::CallInst(IrFunc *func) {
    this->func = func;
}

CallInst::CallInst(IrFunc *func, vector<VarClass *> &params) {
    this->func = func;
    this->params = params;
}

CallInst::CallInst() : Inst() {
    this->func = nullptr;
}

CallInst::CallInst(Element sym) {
    this->func = nullptr;
    this->sym = move(sym);
}

CallInst::CallInst(Element sym, vector<VarClass *> &params) {
    this->func = nullptr;
    this->sym = move(sym);
    this->params = params;
}

string CallInst::getIR() {
    if (func) {
        string res;
        for (auto x: params) {
            string addr = x->isAddr() ? "addr " : "";
            string push = "push " + addr + x->getIR();
            res += push + "\n";
        }
        string call = "call " + func->getName();
        res += call;
        return res;
    } else {
        if (sym.sym == GETINTTK) {
            return "call getint";
        } else if (sym.sym == PRINTFTK) {
            string res;
            for (auto x: params) {
                string push = "push " + x->getIR();
                res += push + "\n" + "call printf" + "\n";
            }
            return res.substr(0, res.size() - 1);
        }
    }
    return "";
}

NotInst::NotInst(VarClass *var, VarClass *not_var) {
    this->var = var;
    this->not_var = not_var;
}

NotInst::NotInst() {
    this->var = nullptr;
    this->not_var = nullptr;
}

LoadInst::LoadInst(VarClass *dst, VarClass *addr) {
    this->dst = dst;
    this->addr = addr;
}

string NotInst::getIR() {
    auto str = "%" + to_string(var->getId());
    return str + " = !" + str;
}

LoadInst::LoadInst() : Inst() {
    this->addr = nullptr;
    this->dst = nullptr;
}

string LoadInst::getIR() {
    return "load " + dst->getIR() + ", " + addr->getIR();
}

StoreInst::StoreInst() : Inst() {
    this->addr = nullptr;
    this->val = nullptr;
}


string StoreInst::getIR() {
    return "store " + val->getIR() + ", " + addr->getIR();
}

StoreInst::StoreInst(VarClass *val, VarClass *addr) {
    this->addr = addr;
    this->val = val;
}

DeclInst::DeclInst() {
    this->var = nullptr;
    this->init = nullptr;
}

void DeclInst::addInit(VarClass *init) {
    this->init = init;
}
bool DeclInst::hasInit() {
    return init || !inits.empty();
}

void DeclInst::addInits(vector<VarClass *> inits) {
    this->inits = move(inits);
}

DeclInst::DeclInst(VarClass *var) : Inst() {
    this->var = var;
    this->init = nullptr;
}

VarClass *DeclInst::getVar() {
    return var;
}


VarClass *DeclInst::getInit() {
    return init;
}

vector<VarClass *> DeclInst::getInits() {
    return inits;
}

string DeclInst::getIR() {
    if (auto arr = dynamic_cast<IrArray *>(var)) {
        string hasConst = arr->isConst ? "const " : "";
        string tag = arr->isGlobal() ? "@" : "%";
        auto def = "alloca " + hasConst + arr->getType().getString() + " " + tag +
                   to_string(arr->getBase()) + " " + to_string(arr->getSize());
        if (!inits.empty()) {
            for (int i = 0; i < inits.size(); i++)
                def += "\n" + var->getIR() + "[" + to_string(i) + "] = " + inits[i]->getIR();
        }
        return def;

    } else if (auto ptr = dynamic_cast<IrPointer *>(var)) {
        string hasConst = ptr->isConst ? "const " : "";
        vector<int> dims;
        dims = ptr->getType().getDims();
        string sdims;
        for (auto x: dims) sdims += "[" + to_string(x) + "]";
        auto def = "para " + ptr->getType().getString() + " " +
                   ptr->getIR() + "[]" + sdims;
        return def;

    } else {
        if (dynamic_cast<IrParam *>(var) != nullptr) {
            return "para " + var->getType().getString() + " " + var->getIR();
        } else if (init) {
            return var->getType().getString() + " " + var->getIR() + " = " + init->getIR();
        }
        return var->getType().getString() + " " + var->getIR();
    }
}

GetReturnInst::GetReturnInst() : Inst() {
    this->var = nullptr;
}

GetReturnInst::GetReturnInst(VarClass *var) {
    this->var = var;
}

string GetReturnInst::getIR() {
    return var->getIR() + " = RET";
}

LoadAddrInst::LoadAddrInst() {
    this->var = nullptr;
    this->base = nullptr;
}

LoadAddrInst::LoadAddrInst(VarClass *var, VarClass *base) {
    this->var = var;
    this->base = base;
}

string LoadAddrInst::getIR() {
    return "la " + var->getIR() + ", " + base->getIR();
}
