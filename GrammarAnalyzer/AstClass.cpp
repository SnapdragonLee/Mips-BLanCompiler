//
// Created by SNAPDRAGON_11800H on 17/10/2021.
//

#include "AstClass.h"
#include "TableList.h"

#define IF 1
#define IF_BODY 2
#define ELSE 3
#define IF_END 5

TableList table;
Ctx *AstClass::context;

AstClass::AstClass() {
    program = nullptr;
}

AstClass::AstClass(Program *p) {
    program = p;
    context = new Ctx();
    context->module = new Module();
    context->table = &table;
}

Module *AstClass::generateCode() {
    program->generateCode();
    return context->module;
}

BlockItem::BlockItem() = default;

void BlockItem::generateCode() {

}

ProgramItem::ProgramItem() = default;

void ProgramItem::generateCode() {

}

Decl::Decl() = default;

Decl::Decl(TypeDefine bType, Element ident) {
    this->bType = move(bType);
    this->ident = move(ident);
}

Decl::Decl(TypeDefine bType, Element ident, vector<Exp *> initVal) {
    this->bType = move(bType);
    this->ident = move(ident);
    this->initVal.insert(this->initVal.end(), initVal.begin(), initVal.end());
}

bool Decl::isConst() {
    return bType.getConst();
}

bool Decl::isParam() {
    return bType.getParam();
}

TypeDefine Decl::getType() {
    return bType;
}

int Decl::getDim() {
    return bType.getDim();
}

vector<int> Decl::getDims() {
    return bType.getDims();
}

Element Decl::getIdent() {
    return ident;
}

vector<int> Decl::getInitVal() {
    vector<int> v;
    for (auto x: initVal) {
        v.push_back(x->evalInt());
    }
    return v;
}

vector<Exp *> Decl::getInitValExp() {
    return initVal;
}

bool Decl::isArray() {
    return bType.getDim() > 0;
}

bool Decl::hasInit() {
    return !initVal.empty();
}

void Decl::generateCode() {
    table.pushDecl(this);

    if (isArray()) {
        if (isParam()) {
            int base = AstClass::context->func->genVar();
            auto ptr = new IrPointer(base, this);
            if (isConst()) ptr->addConst();
            auto decl = new DeclInst(ptr);
            AstClass::context->func->addDecl(this, decl);
            AstClass::context->blk->addInst(decl);
        } else {
            bool isGlobal = AstClass::context->table->isGlobal();
            int size = 1;
            for (auto x: bType.getDims()) size *= x;
            int base = isGlobal ? AstClass::context->genArray() : AstClass::context->func->genArray(size);
            auto arr = new IrArray(base, isGlobal, bType);

            if (isConst()) arr->addConst();
            auto decl = new DeclInst(arr);
            if (hasInit()) {
                vector<VarClass *> inits;
                for (auto x: initVal) {
                    if (isConst() || isGlobal) {
                        inits.push_back(new Constant(x->evalInt()));
                    } else {
                        x->generateCode();
                        if (x->getVar()->isAddr()) {
                            auto val = new VarClass(AstClass::context->func->genVar(), false, x->evalType());
                            AstClass::context->blk->addInst(new LoadInst(val, x->getVar()));
                            inits.push_back(val);
                        } else {
                            inits.push_back(x->getVar());
                        }
                    }
                }
                decl->addInits(inits);
            }
            if (isGlobal) {
                AstClass::context->module->addDecl(this, decl);
            } else {
                AstClass::context->func->addDecl(this, decl);
                AstClass::context->blk->addInst(decl);
            }
        }
    } else {
        if (isConst()) {
            AstClass::context->addConst(this);
            return;
        }

        VarClass *var;
        bool isGlobal = AstClass::context->table->isGlobal();
        int id = isGlobal ? AstClass::context->genGlobal() : AstClass::context->func->genVar();
        if (AstClass::context->isParam) var = new IrParam(id, this);
        else var = new VarClass(id, isGlobal, bType);
        auto decl = new DeclInst(var);

        if (hasInit()) {
            if (isConst() || isGlobal) {
                decl->addInit(new Constant(initVal[0]->evalInt()));
            } else {
                initVal[0]->generateCode();
                if (initVal[0]->getVar()->isAddr()) {
                    auto val = new VarClass(AstClass::context->func->genVar(), false, initVal[0]->evalType());
                    AstClass::context->blk->addInst(new LoadInst(val, initVal[0]->getVar()));
                    decl->addInit(val);
                } else {
                    decl->addInit(initVal[0]->getVar());
                }
            }
        }

        if (isGlobal) {
            AstClass::context->module->addDecl(this, decl);
        } else {
            AstClass::context->func->addDecl(this, decl);
            AstClass::context->blk->addInst(decl);
        }
    }
}

Func::Func() {
    block = nullptr;
}

Func::Func(TypeDefine returnType, Element ident, vector<Decl *> fParams, Block *block) {
    this->returnType = move(returnType);
    this->ident = move(ident);
    this->fParams.insert(this->fParams.end(), fParams.begin(), fParams.end());
    this->block = block;
}

Element Func::getIdent() {
    return ident;
}

TypeDefine Func::getType() {
    return returnType;
}

void Func::generateCode() {
    table.pushFunc(this);
    table.pushBlock();

    auto irFunc = new IrFunc(this);
    int label_id = AstClass::context->genLabel();
    auto entry_blk = new BlockEntry(label_id);
    AstClass::context->module->addFunc(irFunc);
    irFunc->addBlock(entry_blk);
    AstClass::context->addLabel(label_id, entry_blk);

    AstClass::context->func = irFunc;
    // generate function parameters
    AstClass::context->isParam = true;
    AstClass::context->blk = irFunc->getEntryBlock();
    for (auto x: fParams)
        x->generateCode();
    AstClass::context->isParam = false;

    block->generateCode();
    auto instr = AstClass::context->blk->getLastInst();
    auto retInstr = dynamic_cast<ReturnInst *>(instr);
    // if block don't have return instruction, add return instruction on IR_Old
    if (!retInstr) AstClass::context->blk->addInst(new ReturnInst());

    AstClass::context->blk = nullptr;
    AstClass::context->func = nullptr;
    table.popBlock();
}

BinaryExp::BinaryExp() {
    lhs = nullptr;
    rhs = nullptr;
}

BinaryExp::BinaryExp(Exp *lhs, Element op, Exp *rhs) {
    this->lhs = lhs;
    this->op = move(op);
    this->rhs = rhs;
}

Element BinaryExp::getOp() {
    return op;
}

Exp *BinaryExp::getLhs() {
    return lhs;
}

Exp *BinaryExp::getRhs() {
    return rhs;
}

TypeDefine BinaryExp::evalType() {
    if (lhs->evalType() == rhs->evalType()) {
        return lhs->evalType();
    } else {
        throw;
    }
    //return Exp::evalType();
}

int BinaryExp::evalInt() {
    if (op.sym == PLUS) {
        return lhs->evalInt() + rhs->evalInt();
    } else if (op.sym == MINU) {
        return lhs->evalInt() - rhs->evalInt();
    } else if (op.sym == MULT) {
        return lhs->evalInt() * rhs->evalInt();
    } else if (op.sym == DIV) {
        return lhs->evalInt() / rhs->evalInt();
    } else if (op.sym == MOD) {
        return lhs->evalInt() % rhs->evalInt();
    } else if (op.sym == AND) {
        return lhs->evalInt() && rhs->evalInt();
    } else if (op.sym == OR) {
        return lhs->evalInt() || rhs->evalInt();
    } else if (op.sym == LSS) {
        return lhs->evalInt() < rhs->evalInt();
    } else if (op.sym == LEQ) {
        return lhs->evalInt() <= rhs->evalInt();
    } else if (op.sym == GRE) {
        return lhs->evalInt() > rhs->evalInt();
    } else if (op.sym == GEQ) {
        return lhs->evalInt() >= rhs->evalInt();
    } else if (op.sym == EQL) {
        return lhs->evalInt() == rhs->evalInt();
    } else if (op.sym == NEQ) {
        return lhs->evalInt() != rhs->evalInt();
    }
    return 0;
}

void BinaryExp::generateCode() {
    auto sym = this->op.sym;
    if (sym == AND) {
        int end_label = AstClass::context->genLabel();
        auto end_blk = new BlockEntry(end_label);
        int right_label = AstClass::context->genLabel();
        auto right_blk = new BlockEntry(right_label);
        auto var = new VarClass(AstClass::context->func->genVar(), false, evalType());
        lhs->generateCode();
        if (lhs->getVar()->isAddr()) {
            auto val = new VarClass(AstClass::context->func->genVar(), false, evalType());
            AstClass::context->blk->addInst(new LoadInst(val, lhs->getVar()));
            AstClass::context->blk->addInst(new BranchInst(Bnez, val, right_label));
        } else {
            AstClass::context->blk->addInst(new BranchInst(Bnez, lhs->getVar(), right_label));
        }
        auto blk = new BlockEntry(AstClass::context->genLabel());
        AstClass::context->func->addBlock(blk);
        AstClass::context->blk = blk;
        AstClass::context->blk->addInst(new AssignInst(var, new Constant(0)));
        AstClass::context->blk->addInst(new JumpInst(end_label));
        AstClass::context->func->addBlock(right_blk);
        AstClass::context->blk = right_blk;
        rhs->generateCode();
        if (rhs->getVar()->isAddr()) {
            auto val = new VarClass(AstClass::context->func->genVar(), false, evalType());
            AstClass::context->blk->addInst(new LoadInst(val, rhs->getVar()));
            AstClass::context->blk->addInst(new AssignInst(var, val));
        } else {
            AstClass::context->blk->addInst(new AssignInst(var, rhs->getVar()));
        }
        AstClass::context->func->addBlock(end_blk);
        AstClass::context->blk = end_blk;
        this->addVar(var);
    } else if (sym == OR) {
        int end_label = AstClass::context->genLabel();
        auto end_blk = new BlockEntry(end_label);
        int right_label = AstClass::context->genLabel();
        auto right_blk = new BlockEntry(right_label);
        auto var = new VarClass(AstClass::context->func->genVar(), false, evalType());
        lhs->generateCode();
        if (lhs->getVar()->isAddr()) {
            auto val = new VarClass(AstClass::context->func->genVar(), false, evalType());
            AstClass::context->blk->addInst(new LoadInst(val, lhs->getVar()));
            AstClass::context->blk->addInst(new BranchInst(Beqz, val, right_label));
        } else {
            AstClass::context->blk->addInst(new BranchInst(Beqz, lhs->getVar(), right_label));
        }
        auto blk = new BlockEntry(AstClass::context->genLabel());
        AstClass::context->func->addBlock(blk);
        AstClass::context->blk = blk;
        AstClass::context->blk->addInst(new AssignInst(var, new Constant(1)));
        AstClass::context->blk->addInst(new JumpInst(end_label));
        AstClass::context->func->addBlock(right_blk);
        AstClass::context->blk = right_blk;
        rhs->generateCode();
        if (rhs->getVar()->isAddr()) {
            auto val = new VarClass(AstClass::context->func->genVar(), false, evalType());
            AstClass::context->blk->addInst(new LoadInst(val, rhs->getVar()));
            AstClass::context->blk->addInst(new AssignInst(var, val));
        } else {
            AstClass::context->blk->addInst(new AssignInst(var, rhs->getVar()));
        }
        AstClass::context->func->addBlock(end_blk);
        AstClass::context->blk = end_blk;
        this->addVar(var);
    } else {
        lhs->generateCode();
        rhs->generateCode();
        auto var = new VarClass(AstClass::context->expStmt->genVar(), false, evalType());
        if (lhs->getVar()->isAddr() && rhs->getVar()->isAddr()) {
            auto lVariable = new VarClass(AstClass::context->expStmt->genVar(), false, lhs->evalType());
            auto rVariable = new VarClass(AstClass::context->expStmt->genVar(), false, rhs->evalType());
            AstClass::context->blk->addInst(new LoadInst(lVariable, lhs->getVar()));
            AstClass::context->blk->addInst(new LoadInst(rVariable, rhs->getVar()));
            AstClass::context->blk->addInst(new BinaryInst(var, lVariable, op, rVariable));
            this->addVar(var);
        } else if (lhs->getVar()->isAddr()) {
            auto lVariable = new VarClass(AstClass::context->expStmt->genVar(), false, lhs->evalType());
            AstClass::context->blk->addInst(new LoadInst(lVariable, lhs->getVar()));
            AstClass::context->blk->addInst(new BinaryInst(var, lVariable, op, rhs->getVar()));
            this->addVar(var);
        } else if (rhs->getVar()->isAddr()) {
            auto rVariable = new VarClass(AstClass::context->expStmt->genVar(), false, rhs->evalType());
            AstClass::context->blk->addInst(new LoadInst(rVariable, rhs->getVar()));
            AstClass::context->blk->addInst(new BinaryInst(var, lhs->getVar(), op, rVariable));
            this->addVar(var);
        } else {
            auto inst = new BinaryInst(var, this);
            AstClass::context->blk->addInst(inst);
            this->addVar(var);
        }
    }
}

Program::Program() = default;

Program::Program(vector<ProgramItem *> items) {
    this->program_items.insert(this->program_items.end(), items.begin(), items.end());
}

void Program::generateCode() {
    table.pushBlock();
    for (auto node: program_items) {
        node->generateCode();
    }
    table.popBlock();
}

Block::Block() = default;

Block::Block(Element lBrace, vector<BlockItem *> items, Element rBrace) {
    this->lBrace = move(lBrace);
    block_items.insert(block_items.end(), items.begin(), items.end());
    this->rBrace = move(rBrace);
}


void Block::generateCode() {
    for (auto x: block_items) {
        if (auto blk = dynamic_cast<Block *>(x)) {
            table.pushBlock();
            blk->generateCode();
            table.popBlock();
        } else {
            x->generateCode();
        }
    }
}

CallExp::CallExp() = default;

CallExp::CallExp(Element ident) {
    this->ident = move(ident);
}

CallExp::CallExp(Element ident, vector<Exp *> rParams) {
    this->ident = move(ident);
    this->rParams.insert(this->rParams.end(), rParams.begin(), rParams.end());
}

bool CallExp::isGetInt() const {
    return ident.sym == GETINTTK;
}

bool CallExp::isPrintf() const {
    return ident.sym == PRINTFTK;
}

Element CallExp::getIdent() {
    return ident;
}

TypeDefine CallExp::evalType() {
    if (isGetInt()) return TypeDefine(Element(INTTK, "int", ident.row, ident.col));
    Func *func = table.findFunc(this);
    return func->getType();
}

int CallExp::evalInt() {
    cout << "only const can eval" << endl;
    return 0;
}

vector<VarClass *> split(vector<Exp *> rParams) {
    auto fstring = dynamic_cast<FormatString *>(rParams[0]);
    auto str = fstring->getSym().val;
    // remove ""
    str = str.substr(1, str.size() - 2);
    int start = 0, end = (int) str.find("%d");
    vector<VarClass *> v;
    int i = 1;
    if (end == -1) {  // if fstring don't have %d
        AstClass::context->module->addStr(str);
        v.push_back(new IrParam(new Constant(str)));
    } else {
        while (end != -1) {
            auto s = str.substr(start, end - start);
            if (!s.empty()) {
                AstClass::context->module->addStr(s);
                v.push_back(new IrParam(new Constant(s)));
            }
            rParams[i]->generateCode();
            auto var = rParams[i++]->getVar();
            if (var->isAddr()) {
                auto val = new VarClass(AstClass::context->func->genVar(), false, var->getType());
                AstClass::context->blk->addInst(new LoadInst(val, var));
                v.push_back(new IrParam(val));
            } else if (auto constant = dynamic_cast<Constant *>(var)) {
                v.push_back(new IrParam(constant));
            } else {
                v.push_back(new IrParam(var));
            }

            start = end + 2;
            end = (int) str.find("%d", start);
            if (end == -1 && start < str.size()) {
                AstClass::context->module->addStr(str.substr(start));
                v.push_back(new IrParam(new Constant(str.substr(start))));
            }
        }
    }

    return v;
}

void CallExp::generateCode() {
    if (isPrintf()) { //  printf()
        auto p = split(rParams);
        auto inst = new CallInst(ident, p);
        AstClass::context->blk->addInst(inst);
    } else if (isGetInt()) { //  getint()
        AstClass::context->blk->addInst(new CallInst(ident));
        int id = AstClass::context->func->genVar();
        auto var = new VarClass(id, false, this->evalType());
        AstClass::context->blk->addInst(new GetReturnInst(var));
        this->addVar(var);
    } else { //  other function
        vector<VarClass *> params;
        auto func = AstClass::context->getFunc(ident.val);
        if (rParams.empty()) { //  if function don't have parameters
            AstClass::context->blk->addInst(new CallInst(func));
        } else {
            for (auto x: rParams) {
                x->generateCode();
                auto var = x->getVar();
                if (var->isAddr()) {
                    auto val = new VarClass(AstClass::context->func->genVar(), false, var->getType());
                    AstClass::context->blk->addInst(new LoadInst(val, var));
                    params.push_back(val);
                } else {
                    params.push_back(var);
                }
            }
            AstClass::context->blk->addInst(new CallInst(func, params));
        }
        //  if this function has return value
        if (func->getType().getType().sym != VOIDTK) {
            int id = AstClass::context->func->genVar();
            auto var = new VarClass(id, false, this->evalType());
            AstClass::context->blk->addInst(new GetReturnInst(var));
            this->addVar(var);
        }
    }
}

ExpStmt::ExpStmt() {
    exp = nullptr;
    this->varId = 0;
}

ExpStmt::ExpStmt(Exp *e) {
    this->exp = e;
    this->varId = 0;
}

int ExpStmt::genVar() {
    if (AstClass::context->table->isGlobal()) return varId++;
    else return AstClass::context->func->genVar();
}

void ExpStmt::generateCode() {
    AstClass::context->expStmt = this;
    exp->generateCode();
}

UnaryExp::UnaryExp() {
    this->op = nullptr;
    this->exp = nullptr;
}

UnaryExp::UnaryExp(Exp *exp) {
    this->op = nullptr;
    this->exp = exp;
}

UnaryExp::UnaryExp(Element sym) {
    this->op = nullptr;
    this->exp = nullptr;
    this->sym = move(sym);
}

UnaryExp::UnaryExp(Element op, Element sym) {
    this->op = new Element;
    *(this->op) = move(op);
    this->sym = move(sym);
    this->exp = nullptr;
}

UnaryExp::UnaryExp(Element op, Exp *exp) {
    this->op = new Element;
    *(this->op) = move(op);
    this->exp = exp;
}

Exp *UnaryExp::getExp() {
    return exp;
}

Element *UnaryExp::getOp() {
    return op;
}

Element UnaryExp::getSymbol() {
    return sym;
}

TypeDefine UnaryExp::evalType() {
    if (sym.sym == INTCON)
        return TypeDefine(Element(INTTK, "int", sym.row, sym.col));
    return exp->evalType();
}

int UnaryExp::evalInt() {
    if (op == nullptr) {
        if (sym.sym == INTCON) return stoi(sym.val);
        else return exp->evalInt();
    }
    if (op->sym == PLUS) {
        return exp->evalInt();
    } else if (op->sym == MINU) {
        return -1 * exp->evalInt();
    } else if (op->sym == NOT) {
        return !exp->evalInt();
    }
    return 0;
}

void UnaryExp::generateCode() {
    if (op == nullptr) {
        if (exp) {
            exp->generateCode();
            if (exp->getVar()->isAddr()) {
                auto var = new VarClass(AstClass::context->expStmt->genVar(), false, evalType());
                AstClass::context->blk->addInst(new LoadInst(var, exp->getVar()));
                this->addVar(var);
            } else {
                this->addVar(exp->getVar());
            }
        } else if (sym.sym == INTCON) { //  this Exp is an integer
            this->addVar(new Constant(stoi(sym.val)));
        } else {} // todo: more options
    } else {
        if (op->sym == PLUS) {
            exp->generateCode();
            if (exp->getVar()->isAddr()) {
                auto val = new VarClass(AstClass::context->expStmt->genVar(), false, evalType());
                AstClass::context->blk->addInst(new LoadInst(val, exp->getVar()));
                this->addVar(val);
            } else {
                this->addVar(exp->getVar());
            }
        } else if (op->sym == MINU) {
            exp->generateCode();
            if (exp->getVar()->isAddr()) {
                auto val = new VarClass(AstClass::context->func->genVar(), false, exp->evalType());
                AstClass::context->blk->addInst(new LoadInst(val, exp->getVar()));
                auto var = new VarClass(AstClass::context->expStmt->genVar(), false, exp->evalType());
                AstClass::context->blk->addInst(new BinaryInst(var, Sub, new Constant(0), val));
                this->addVar(var);
            } else {
                auto var = new VarClass(AstClass::context->expStmt->genVar(), false, exp->evalType());
                AstClass::context->blk->addInst(new BinaryInst(var, Sub, exp));
                this->addVar(var);
            }
        } else if (op->sym == NOT) {
            exp->generateCode();
            if (exp->getVar()->isAddr()) {
                auto val = new VarClass(AstClass::context->func->genVar(), false, exp->evalType());
                AstClass::context->blk->addInst(new LoadInst(val, exp->getVar()));
                auto var = new VarClass(AstClass::context->func->genVar(), false, exp->evalType());
                AstClass::context->blk->addInst(new NotInst(val, var));
                this->addVar(var);
            } else {
                auto var = new VarClass(AstClass::context->func->genVar(), false, exp->evalType());
                AstClass::context->blk->addInst(new NotInst(exp->getVar(), var));
                this->addVar(var);
            }
        }
    }
}

Stmt::Stmt() = default;

void Stmt::generateCode() {

}

LVal::LVal() = default;

LVal::LVal(Element sym, vector<Exp *> dims = {}) {
    this->ident = move(sym);
    this->dims.insert(this->dims.end(), dims.begin(), dims.end());
}

Element LVal::getIdent() {
    return ident;
}

vector<Exp *> LVal::getDims() {
    return dims;
}

TypeDefine LVal::evalType() {
    Decl *decl = table.findDecl(this);
    if (decl == nullptr) return {};

    TypeDefine declType = decl->getType();
    Element tkn = declType.getType();
    vector<int> declDim = decl->getDims();

    TypeDefine type = TypeDefine(tkn);

    int n = (int) declType.getDim();
    int lValDim = n - (int) dims.size();
    for (int i = 0; i < lValDim; i++) {
        if (declType.getPointer()) {
            if (i == n - 1 && declType.getPointer()) type.addPointer();
            else type.addDim(declDim[n - i - 2]);
        } else {
            type.addDim(declDim[n - i - 1]);
        }
    }
    if (declType.getParam()) type.addParam();

    return type;
}

int LVal::evalInt() {
    Decl *decl = table.findDecl(this);
    if (decl->getType().getConst()) {
        if (dims.empty()) return decl->getInitVal()[0];
        else {
            auto d = decl->getDims();
            int offset = 0;
            for (int i = 0; i < dims.size(); i++) {
                offset = offset * d[i] + dims[i]->evalInt();
            }
            return decl->getInitVal()[offset];
        }
    } else if (AstClass::context->table->isGlobal()) {
        if (dims.empty()) return decl->getInitVal()[0];
        else {
            auto d = decl->getDims();
            int offset = 0;
            for (int i = 0; i < dims.size(); i++) {
                offset = offset * d[i] + dims[i]->evalInt();
            }
            return decl->getInitVal()[offset];
        }
    }
    return 0;
}

void LVal::generateCode() {
    Decl *decl = table.findDecl(this);
    if (decl->isArray()) {
        auto base = AstClass::context->func->getVar(decl);
        // is global variable
        if (base == nullptr) base = AstClass::context->module->getGlobal(decl);
        auto var = new VarClass(AstClass::context->func->genVar(), false, base->getType());
        if (auto ptr = dynamic_cast<IrPointer *>(base)) AstClass::context->blk->addInst(new AssignInst(var, base));
        else AstClass::context->blk->addInst(new LoadAddrInst(var, base));
        if (dims.empty()) {
            this->addVar(var);
        } else {
            VarClass *prev = nullptr;
            if (dims.size() < decl->getDim()) {
                int n = decl->getDim();
                auto offset = new VarClass(AstClass::context->func->genVar(), false, dims[0]->evalType());
                for (int i = 0; i < n - 1; i++) {
                    Constant *size;
                    if (auto ptr = dynamic_cast<IrPointer *>(base)) {
                        size = new Constant(decl->getDims()[i]);
                    } else {
                        size = new Constant(decl->getDims()[i + 1]);
                    }
                    if (i < dims.size()) {
                        dims[i]->generateCode();
                        if (i == 0) {
                            if (dims[i]->getVar()->isAddr()) {
                                auto val = new VarClass(AstClass::context->func->genVar(), false, dims[i]->evalType());
                                AstClass::context->blk->addInst(new LoadInst(val, dims[i]->getVar()));
                                AstClass::context->blk->addInst(new BinaryInst(offset, Mul, val, size));
                            } else {
                                AstClass::context->blk->addInst(new BinaryInst(offset, Mul, dims[i]->getVar(), size));
                            }
                        } else {
                            if (dims[i]->getVar()->isAddr()) {
                                auto val = new VarClass(AstClass::context->func->genVar(), false, dims[i]->evalType());
                                AstClass::context->blk->addInst(new LoadInst(val, dims[i]->getVar()));
                                AstClass::context->blk->addInst(new BinaryInst(offset, Mul, val, size));
                            } else {
                                AstClass::context->blk->addInst(new BinaryInst(offset, Mul, dims[i]->getVar(), size));
                            }
                            AstClass::context->blk->addInst(new BinaryInst(offset, Mul, offset, size));
                        }
                    } else {
                        AstClass::context->blk->addInst(new BinaryInst(offset, Mul, offset, size));
                    }
                }
                AstClass::context->blk->addInst(new BinaryInst(offset, Mul, offset, new Constant(4)));
                AstClass::context->blk->addInst(new BinaryInst(offset, Add, var, offset));
                this->addVar(offset);
            } else {
                for (int i = 0; i < dims.size(); i++) {
                    int id = AstClass::context->func->genVar();
                    auto offset = new VarClass(id, false, dims[i]->evalType());
                    if (i == dims.size() - 1) {
                        dims[i]->generateCode();
                        if (prev == nullptr) {
                            if (dims[i]->getVar()->isAddr()) {
                                auto val = new VarClass(AstClass::context->func->genVar(), false, dims[i]->evalType());
                                AstClass::context->blk->addInst(new LoadInst(val, dims[i]->getVar()));
                                prev = val;
                            } else {
                                prev = dims[i]->getVar();
                            }
                        }
                        AstClass::context->blk->addInst(new BinaryInst(offset, Mul, prev, new Constant(4)));
                        AstClass::context->blk->addInst(new BinaryInst(offset, Add, var, offset));
                    } else {
                        if (auto ptr = dynamic_cast<IrPointer *>(base)) {
                            auto size = new Constant(decl->getDims()[i]);
                            if (prev == nullptr) {
                                dims[i]->generateCode();
                                if (dims[i]->getVar()->isAddr()) {
                                    auto val = new VarClass(AstClass::context->func->genVar(), false, dims[i]->evalType());
                                    AstClass::context->blk->addInst(new LoadInst(val, dims[i]->getVar()));
                                    AstClass::context->blk->addInst(new BinaryInst(offset, Mul, val, size));
                                } else {
                                    AstClass::context->blk->addInst(new BinaryInst(offset, Mul, dims[i]->getVar(), size));
                                }
                            } else {
                                AstClass::context->blk->addInst(new BinaryInst(offset, Mul, prev, size));
                            }
                            dims[i + 1]->generateCode();
                            if (dims[i + 1]->getVar()->isAddr()) {
                                auto val = new VarClass(AstClass::context->func->genVar(), false, dims[i + 1]->evalType());
                                AstClass::context->blk->addInst(new LoadInst(val, dims[i + 1]->getVar()));
                                AstClass::context->blk->addInst(new BinaryInst(offset, Add, offset, val));
                            } else {
                                AstClass::context->blk->addInst(new BinaryInst(offset, Add, offset, dims[i + 1]->getVar()));
                            }
                        } else {
                            auto size = new Constant(decl->getDims()[i + 1]);
                            dims[i + 1]->generateCode();
                            if (prev == nullptr) {
                                dims[i]->generateCode();
                                if (dims[i]->getVar()->isAddr()) {
                                    auto val = new VarClass(AstClass::context->func->genVar(), false, evalType());
                                    AstClass::context->blk->addInst(new LoadInst(val, dims[i]->getVar()));
                                    AstClass::context->blk->addInst(new BinaryInst(offset, Mul, val, size));
                                } else {
                                    AstClass::context->blk->addInst(new BinaryInst(offset, Mul, dims[i]->getVar(), size));
                                }
                            } else {
                                AstClass::context->blk->addInst(new BinaryInst(offset, Mul, prev, size));
                            }
                            if (dims[i + 1]->getVar()->isAddr()) {
                                auto val = new VarClass(AstClass::context->func->genVar(), false, evalType());
                                AstClass::context->blk->addInst(new LoadInst(val, dims[i + 1]->getVar()));
                                AstClass::context->blk->addInst(new BinaryInst(offset, Add, offset, val));
                            } else {
                                AstClass::context->blk->addInst(new BinaryInst(offset, Add, offset, dims[i + 1]->getVar()));
                            }
                        }
                    }
                    prev = offset;
                }
                prev->addAddr();
                this->addVar(prev);
            }
        }
    } else if (decl->isConst()) {
        this->addVar(AstClass::context->const_map[decl]);
    } else if (AstClass::context->func && AstClass::context->func->getVar(decl)) {
        auto var = AstClass::context->func->getVar(decl);
        this->addVar(var);
    } else {
        auto var = AstClass::context->module->getGlobal(decl);
        this->addVar(var);
    }
}

Exp::Exp() {
    var = nullptr;
    cond = false;
}

int Exp::evalInt() {
    return 0;
}

TypeDefine Exp::evalType() {
    return {};
}

void Exp::generateCode() {

}

void Exp::addVar(VarClass *variable) {
    this->var = variable;
}

VarClass *Exp::getVar() {
    return var;
}

void Exp::addCond() {
    cond = true;
}

AssignExp::AssignExp() {
    lhs = nullptr;
    rhs = nullptr;
}

AssignExp::AssignExp(LVal *l, Exp *r) {
    lhs = l;
    rhs = r;
}

TypeDefine AssignExp::evalType() {
    // check lhs and rhs type
    return lhs->evalType();
}

int AssignExp::evalInt() {
    cout << "only const can eval" << endl;
    return rhs->evalInt();
}

void AssignExp::generateCode() {
    lhs->generateCode();
    rhs->generateCode();

    if (lhs->getVar()->isAddr() && rhs->getVar()->isAddr()) {
        auto rval = new VarClass(AstClass::context->func->genVar(), false, rhs->evalType());
        AstClass::context->blk->addInst(new LoadInst(rval, rhs->getVar()));
        auto lval = new VarClass(AstClass::context->func->genVar(), false, rhs->evalType());
        AstClass::context->blk->addInst(new AssignInst(lval, rval));
        AstClass::context->blk->addInst(new StoreInst(lval, lhs->getVar()));
    } else {
        if (lhs->getVar()->isAddr()) {
            auto val = new VarClass(AstClass::context->func->genVar(), false, lhs->evalType());
            AstClass::context->blk->addInst(new LoadInst(val, lhs->getVar()));
            AstClass::context->blk->addInst(new AssignInst(val, rhs->getVar()));
            AstClass::context->blk->addInst(new StoreInst(val, lhs->getVar()));
        } else if (rhs->getVar()->isAddr()) {
            auto val = new VarClass(AstClass::context->func->genVar(), false, rhs->evalType());
            AstClass::context->blk->addInst(new LoadInst(val, rhs->getVar()));
            AstClass::context->blk->addInst(new AssignInst(lhs->getVar(), val));
        } else {
            this->addVar(lhs->getVar());
            AstClass::context->blk->addInst(new AssignInst(lhs->getVar(), rhs->getVar()));
        }
    }
}

CondStmt::CondStmt() {
    condExp = nullptr;
    ifStmt = elseStmt = nullptr;
}

CondStmt::CondStmt(Element token, Exp *condExp, Stmt *ifStmt, Stmt *elseStmt) {
    this->token = move(token);
    this->condExp = condExp;
    this->ifStmt = ifStmt;
    this->elseStmt = elseStmt;
}

BlockEntry *setupIfBlock(int id, int type) {
    auto blk = new BlockEntry(id);
    switch (type) {
        case IF:
            AstClass::context->if_blk = blk;
            break;
        case IF_BODY:
            AstClass::context->if_body_blk = blk;
            break;
        case ELSE:
            AstClass::context->else_blk = blk;
            break;
        case IF_END:
            AstClass::context->if_end_blk = blk;
            break;
        default:
            break;
    }
    AstClass::context->addLabel(id, blk);
    return blk;
}

BlockEntry *setupWhileBlock(int id) {
    auto blk = new BlockEntry(id);
    AstClass::context->addLabel(id, blk);
    return blk;
}

void CondStmt::generateCode() {
    if (token.sym == WHILETK) {
        int while_label_id = AstClass::context->genLabel();
        auto while_blk = setupWhileBlock(while_label_id);
        AstClass::context->func->addBlock(while_blk);

        int body_label_id = AstClass::context->genLabel();
        auto body_blk = setupWhileBlock(body_label_id);

        int while_end_label_id = AstClass::context->genLabel();
        auto while_end_blk = setupWhileBlock(while_end_label_id);

        AstClass::context->pushWhile(while_blk, body_blk, while_end_blk);
        // while condition block
        AstClass::context->blk = while_blk;
        // TODO: implement && and ||
        condExp->generateCode();
        if (condExp->getVar()->isAddr()) {
            auto val = new VarClass(AstClass::context->func->genVar(), false, condExp->evalType());
            AstClass::context->blk->addInst(new LoadInst(val, condExp->getVar()));
            AstClass::context->blk->addInst(new BranchInst(Beqz, val, while_end_label_id));
        } else {
            AstClass::context->blk->addInst(new BranchInst(Beqz, condExp->getVar(), while_end_label_id));
        }

        // while block
        AstClass::context->blk = body_blk;
        AstClass::context->func->addBlock(body_blk);
        table.pushBlock();
        ifStmt->generateCode();
        table.popBlock();
        AstClass::context->blk->addInst(new JumpInst(while_label_id));

        // while end
        AstClass::context->popWhile();
        AstClass::context->func->addBlock(while_end_blk);
        AstClass::context->blk = while_end_blk;
    } else if (token.sym == IFTK) {
        int if_label_id = AstClass::context->genLabel();
        auto if_blk = setupIfBlock(if_label_id, IF);
        AstClass::context->func->addBlock(if_blk);

        int body_label_id = AstClass::context->genLabel();
        auto body_blk = setupIfBlock(body_label_id, IF_BODY);

        int else_label_id = 0, end_label_id = 0;
        BlockEntry *else_blk = nullptr, *if_end_blk = nullptr;
        if (elseStmt) {
            else_label_id = AstClass::context->genLabel();
            else_blk = setupIfBlock(else_label_id, ELSE);
        }
        end_label_id = AstClass::context->genLabel();
        if_end_blk = setupIfBlock(end_label_id, IF_END);

        AstClass::context->blk = if_blk;
        condExp->generateCode();
        int next_label_id = else_blk ? else_label_id : end_label_id;
        if (condExp->getVar()->isAddr()) {
            auto val = new VarClass(AstClass::context->func->genVar(), false, condExp->evalType());
            AstClass::context->blk->addInst(new LoadInst(val, condExp->getVar()));
            AstClass::context->blk->addInst(new BranchInst(Beqz, val, next_label_id));
        } else {
            AstClass::context->blk->addInst(new BranchInst(Beqz, condExp->getVar(), next_label_id));
        }
        AstClass::context->func->addBlock(body_blk);
        AstClass::context->blk = body_blk;
        table.pushBlock();
        ifStmt->generateCode();
        table.popBlock();
        AstClass::context->blk->addInst(new JumpInst(end_label_id));
        if (else_blk) {
            AstClass::context->func->addBlock(else_blk);
            AstClass::context->blk = else_blk;
            table.pushBlock();
            elseStmt->generateCode();
            table.popBlock();
            AstClass::context->blk->addInst(new JumpInst(end_label_id));
        }
        AstClass::context->if_blk = nullptr;
        AstClass::context->if_body_blk = nullptr;
        AstClass::context->if_end_blk = nullptr;
        AstClass::context->func->addBlock(if_end_blk);
        AstClass::context->blk = if_end_blk;
    } else {
        // bug
    }
}

LoopStmt::LoopStmt() = default;

LoopStmt::LoopStmt(Element sym) {
    this->sym = move(sym);
}

void LoopStmt::generateCode() {
    if (sym.sym == BREAKTK) {
        AstClass::context->blk->addInst(new JumpInst(AstClass::context->while_end_blk->getId()));
    } else if (sym.sym == CONTINUETK) {
        AstClass::context->blk->addInst(new JumpInst(AstClass::context->while_blk->getId()));
    } else {
        // bug
    }
}

ReturnStmt::ReturnStmt() {
    exp = nullptr;
}

ReturnStmt::ReturnStmt(Element sym, Exp *exp = nullptr) {
    this->sym = move(sym);
    this->exp = exp;
}

void ReturnStmt::generateCode() {
    if (exp) {
        exp->generateCode();
        if (exp->getVar()->isAddr()) {
            auto var = new VarClass(AstClass::context->func->genVar(), false, exp->evalType());
            AstClass::context->blk->addInst(new LoadInst(var, exp->getVar()));
            AstClass::context->blk->addInst(new ReturnInst(var));
        } else {
            AstClass::context->blk->addInst(new ReturnInst(exp->getVar()));
        }
    } else {
        AstClass::context->blk->addInst(new ReturnInst());
    }
}

FormatString::FormatString() = default;

FormatString::FormatString(Element str) {
    sym = move(str);
}

Element FormatString::getSym() {
    return sym;
}

void FormatString::generateCode() {

}
