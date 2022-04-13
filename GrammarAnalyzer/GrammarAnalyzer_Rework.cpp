//
// Created by SNAPDRAGON_11800H on 15/10/2021.
//

#include "GrammarAnalyzer_Rework.h"
#define SYM (sym.sym)

bool isLVal;

GrammarAnalyzer_Rework::GrammarAnalyzer_Rework(Element *symbols, int num) {
    vector<Element> vec(symbols, symbols + num);
    src = move(vec);
    src.emplace_back(END, "", 0, 0);
    analyze();
}

void GrammarAnalyzer_Rework::analyze() {
    ptr = 0;
    sym = nextSymbol();
    program = compUnit();
}

void GrammarAnalyzer_Rework::pushSymbol() {
    /*out.push_back(symbolsName[SYM] + " " + sym.val);*/
    sym = nextSymbol();
}

Element GrammarAnalyzer_Rework::nextSymbol() {
    return src[ptr++];
}

Element GrammarAnalyzer_Rework::viewNextSymbol() {
    return src[ptr];
}

Element GrammarAnalyzer_Rework::viewNextSymbol(int i) {
    return src[ptr + i];
}

int GrammarAnalyzer_Rework::evalInt(Exp *exp) {
    if (auto lval = dynamic_cast<LVal *>(exp)) {
        auto decl = table.findDecl(lval);
        if (decl->getType().getConst() || table.isGlobal()) {
            if (lval->getDims().empty()) {
                return evalInt(decl->getInitValExp()[0]);
            } else {
                auto d = decl->getDims();
                int offset = 0;
                for (int i = 0; i < lval->getDims().size(); i++) {
                    offset = offset * d[i] + evalInt(lval->getDims()[i]);
                }
                return evalInt(decl->getInitValExp()[offset]);
            }
        }
    } else if (auto unaryExp = dynamic_cast<UnaryExp *>(exp)) {
        Element *op;
        op = unaryExp->getOp();
        if (op == nullptr) {
            if (unaryExp->getSymbol().sym == INTCON) return stoi(unaryExp->getSymbol().val);
            else return evalInt(unaryExp->getExp());
        }
        if (op->sym == PLUS) {
            return evalInt(unaryExp->getExp());
        } else if (op->sym == MINU) {
            return -1 * evalInt(unaryExp->getExp());
        } else if (op->sym == NOT) {
            return !evalInt(unaryExp->getExp());
        }
    } else if (auto binaryExp = dynamic_cast<BinaryExp *>(exp)) {
        Element op;
        op = binaryExp->getOp();
        Exp *lhs;
        lhs = binaryExp->getLhs();
        Exp *rhs;
        rhs = binaryExp->getRhs();
        if (op.sym == PLUS) {
            return evalInt(lhs) + evalInt(rhs);
        } else if (op.sym == MINU) {
            return evalInt(lhs) - evalInt(rhs);
        } else if (op.sym == MULT) {
            return evalInt(lhs) * evalInt(rhs);
        } else if (op.sym == DIV) {
            return evalInt(lhs) / evalInt(rhs);
        } else if (op.sym == MOD) {
            return evalInt(lhs) % evalInt(rhs);
        } else if (op.sym == AND) {
            return evalInt(lhs) && evalInt(rhs);
        } else if (op.sym == OR) {
            return evalInt(lhs) || evalInt(rhs);
        } else if (op.sym == LSS) {
            return evalInt(lhs) < evalInt(rhs);
        } else if (op.sym == EQL) {
            return evalInt(lhs) == evalInt(rhs);
        } else if (op.sym == NEQ) {
            return evalInt(lhs) != evalInt(rhs);
        } else if (op.sym == LEQ) {
            return evalInt(lhs) <= evalInt(rhs);
        } else if (op.sym == GRE) {
            return evalInt(lhs) > evalInt(rhs);
        } else if (op.sym == GEQ) {
            return evalInt(lhs) >= evalInt(rhs);
        }
    }
    return 0;
}

vector<Decl *> GrammarAnalyzer_Rework::decl() {
    if (SYM == CONSTTK)
        return constDecl();
    else if (SYM == INTTK)
        return varDecl();
    return {};
}

vector<Decl *> GrammarAnalyzer_Rework::constDecl() {
    vector<Decl *> decls;
    Element type;
    bool isConst = false;

    Element symbol = sym;
    isConst = true;
    pushSymbol();
    type = bType();

    auto def = constDef(isConst, type);
    decls.push_back(def);
    table.pushDecl(def);

    while (SYM == COMMA) {
        pushSymbol();

        auto def = constDef(isConst, type);
        decls.push_back(def);
        table.pushDecl(def);
    }
    pushSymbol();

    return decls;
}

Program *GrammarAnalyzer_Rework::compUnit() {
    table.pushBlock();
    vector<ProgramItem *> items;

    while (SYM == INTTK || SYM == CONSTTK) {
        if (SYM != CONSTTK && (viewNextSymbol().sym != IDENFR || viewNextSymbol(1).sym == LPARENT))
            break;
        vector<Decl *> v = decl();
        items.insert(items.end(), v.begin(), v.end());
    }

    while (SYM == INTTK || SYM == VOIDTK) {
        if (viewNextSymbol().sym == MAINTK)
            break;
        Func *f = funcDef();
        items.push_back(f);
        table.pushFunc(f);
    }


    items.push_back(mainFuncDef());

    table.popBlock();
    return new Program(items);
}

Element GrammarAnalyzer_Rework::bType() {
    if (SYM == INTTK) {
        Element btype = sym;
        pushSymbol();
        return btype;
    }
    return {};
}

Decl *GrammarAnalyzer_Rework::constDef(bool isConst, const Element &type) {
    Element idt;
    vector<Exp *> init;
    vector<int> dims;

    idt = ident();
    while (SYM == LBRACK) {
        pushSymbol();
        Element symbol = sym;
        dims.push_back(evalInt(constExp()));
        pushSymbol();
    }

    pushSymbol();
    init = constInitVal();

    return new Decl(TypeDefine(type, dims, isConst), idt, init);
}

vector<Decl *> GrammarAnalyzer_Rework::varDecl() {
    vector<Decl *> decls;
    Element type;

    Element symbol;
    type = bType();

    decls.push_back(varDef(type));

    while (SYM == COMMA) {
        pushSymbol();

        decls.push_back(varDef(type));
    }
    pushSymbol();

    return decls;
}

vector<Exp *> GrammarAnalyzer_Rework::constInitVal() {
    vector<Exp *> init;

    if (SYM == LBRACE) {
        pushSymbol();
        if (SYM != RBRACE) {
            vector<Exp *> v;
            v = constInitVal();
            init.insert(init.end(), v.begin(), v.end());
            while (SYM == COMMA) {
                pushSymbol();
                v = constInitVal();
                init.insert(init.end(), v.begin(), v.end());
            }
        }

        pushSymbol();
    } else {
        init.push_back(constExp());
    }

    return init;
}

Decl *GrammarAnalyzer_Rework::varDef(const Element &type) {
    Element idt;
    vector<Exp *> init;
    vector<int> dims;

    idt = ident();
    while (SYM == LBRACK) {
        pushSymbol();
        Element symbol = sym;
        dims.push_back(evalInt(constExp()));
        pushSymbol();
    }

    if (SYM == ASSIGN) {
        pushSymbol();
        init = initVal();
    }

    return new Decl(TypeDefine(type, dims), idt, init);
}

vector<Exp *> GrammarAnalyzer_Rework::initVal() {
    vector<Exp *> init;

    if (SYM == LBRACE) {
        pushSymbol();
        if (SYM != RBRACE) {
            vector<Exp *> v;
            v = initVal();
            init.insert(init.end(), v.begin(), v.end());
            while (SYM == COMMA) {
                pushSymbol();
                v = initVal();
                init.insert(init.end(), v.begin(), v.end());
            }
        }
        pushSymbol();
    } else {
        init.push_back(exp());
    }

    return init;
}

Func *GrammarAnalyzer_Rework::funcDef() {
    Element type, idt;
    vector<int> dims;
    vector<Decl *> fParams;
    Block *blk = nullptr;

    type = funcType();
    idt = ident();


    Element symbol = sym;
    pushSymbol();
    if (SYM != RPARENT && SYM == INTTK) fParams = funcFParams();
    pushSymbol();

    table.pushBlock();
    blk = block();
    table.popBlock();

    return new Func(TypeDefine(type, dims), idt, fParams, blk);
}

Func *GrammarAnalyzer_Rework::mainFuncDef() {
    Element type, idt;
    Block *blk = nullptr;


    type = sym;
    pushSymbol();


    idt = sym;
    pushSymbol();


    Element symbol = sym;
    pushSymbol();

    pushSymbol();
    table.pushBlock();
    blk = block();
    table.popBlock();

    return new Func(TypeDefine(type), idt, {}, blk);
}

Element GrammarAnalyzer_Rework::funcType() {

    Element ftype = sym;
    pushSymbol();
    return ftype;
}

vector<Decl *> GrammarAnalyzer_Rework::funcFParams() {
    vector<Decl *> fParams;
    Decl *fParam;

    fParam = funcFParam();
    fParams.push_back(fParam);
    while (SYM == COMMA) {
        pushSymbol();
        fParam = funcFParam();
        fParams.push_back(fParam);
    }

    return fParams;
}

Decl *GrammarAnalyzer_Rework::funcFParam() {
    Element type, idt;
    vector<int> dims;
    bool isConst = false, isPointer = false, isParam = true;

    type = bType();
    idt = ident();
    if (SYM == LBRACK) {
        Element symbol = sym;
        pushSymbol();
        pushSymbol();
        isPointer = true;

        while (SYM == LBRACK) {
            pushSymbol();
            symbol = sym;
            dims.push_back(evalInt(constExp()));
            pushSymbol();
        }
    }

    return new Decl(TypeDefine(type, dims, isConst, isParam, isPointer), idt);
}

bool GrammarAnalyzer_Rework::isExp() const {
    return SYM == SEMICN || SYM == IDENFR || SYM == PLUS ||
           SYM == MINU || SYM == NOT || SYM == LPARENT ||
           SYM == INTCON;
}

bool GrammarAnalyzer_Rework::isBlockItem() {
    return SYM == INTTK || SYM == CONSTTK || isExp() ||
           SYM == IFTK || SYM == WHILETK || SYM == CONTINUETK ||
           SYM == BREAKTK || SYM == PRINTFTK || SYM == RETURNTK ||
           SYM == LBRACE;
}

Block *GrammarAnalyzer_Rework::block() {
    Block *blk = nullptr;
    vector<BlockItem *> items;
    Element lBrace, rBrace;

    lBrace = sym;
    pushSymbol();
    while (isBlockItem()) {
        blockItem(items);
    }

    rBrace = sym;
    pushSymbol();

    return new Block(lBrace, items, rBrace);
}

void GrammarAnalyzer_Rework::blockItem(vector<BlockItem *> &items) {
    if (SYM == INTTK || SYM == CONSTTK) {
        vector<Decl *> v = decl();
        for (auto x: v) items.push_back(x);
    } else {
        items.push_back(stmt());
    }
}

void popout(vector<string> &out) {
    out.pop_back();
    out.pop_back();
    out.pop_back();
    out.pop_back();
    out.pop_back();
}

Stmt *GrammarAnalyzer_Rework::stmt() {
    Element symbol = sym;
    if (SYM == SEMICN) {
        pushSymbol();

        return new ExpStmt(new Exp());
    } else if (isExp()) { // [Exp];
        Exp *e;

        isLVal = false;
        e = exp();
        if (isLVal && SYM == ASSIGN) {
            LVal *lVal = (LVal *) e;
            Exp *assignExp;

            pushSymbol();
            if (SYM == GETINTTK) {
                Element idt;

                idt = sym;
                pushSymbol();

                symbol = sym;
                pushSymbol();
                pushSymbol();

                assignExp = new CallExp(idt);
            } else {
                assignExp = exp();
            }
            e = new AssignExp(lVal, assignExp);
        }
        pushSymbol();

        return new ExpStmt(e);
    } else if (SYM == LBRACE) {
        table.pushBlock();
        Block *blk = block();
        table.popBlock();

        return blk;
    } else if (SYM == IFTK) {
        Element tkn = sym;
        Exp *condExp = nullptr;
        Stmt *ifStmt = nullptr, *elseStmt = nullptr;

        pushSymbol();

        pushSymbol();

        condExp = cond();
        pushSymbol();

        ifStmt = stmt();
        if (SYM == ELSETK) {
            pushSymbol();
            elseStmt = stmt();
        }

        return new CondStmt(tkn, condExp, ifStmt, elseStmt);
    } else if (SYM == WHILETK) {
        Element tkn = sym;
        Exp *condExp = nullptr;
        Stmt *whileStmt = nullptr;

        pushSymbol();

        pushSymbol();

        condExp = cond();
        pushSymbol();

        whileStmt = stmt();

        return new CondStmt(tkn, condExp, whileStmt);
    } else if (SYM == BREAKTK || SYM == CONTINUETK) {
        Element tkn = sym;

        pushSymbol();
        pushSymbol();

        return new LoopStmt(tkn);
    } else if (SYM == RETURNTK) {
        Element tkn = sym;
        Exp *returnExp = nullptr;

        pushSymbol();
        if (SYM != SEMICN && isExp()) returnExp = exp();
        pushSymbol();

        return new ReturnStmt(tkn, returnExp);
    } else if (SYM == PRINTFTK) {
        Element idt;
        vector<Exp *> rParams;

        idt = sym;
        pushSymbol();

        symbol = sym;
        pushSymbol();

        rParams.push_back(new FormatString(formatString()));
        while (SYM == COMMA) {
            pushSymbol();
            rParams.push_back(exp());
        }

        pushSymbol();

        pushSymbol();

        return new ExpStmt(new CallExp(idt, rParams));
    }

    return nullptr;
}

Exp *GrammarAnalyzer_Rework::exp() {
    Exp *node = addExp();

    return node;
}

Exp *GrammarAnalyzer_Rework::cond() {
    Exp *e = lOrExp();
    e->addCond();

    return e;
}

UnaryExp *GrammarAnalyzer_Rework::lVal() {
    Element idt = ident();
    vector<Exp *> dims;

    while (SYM == LBRACK) {
        pushSymbol();
        Element symbol = sym;
        dims.push_back(exp());
        pushSymbol();
    }
    isLVal = true;

    return new LVal(idt, dims);
}

UnaryExp *GrammarAnalyzer_Rework::primaryExp() {
    if (SYM == LPARENT) {
        Exp *e = nullptr;

        Element symbol = sym;
        pushSymbol();

        e = exp();
        pushSymbol();

        return new UnaryExp(e);
    } else if (SYM == IDENFR) {
        return lVal();
    } else if (SYM == INTCON) {
        return number();
    }

    return nullptr;
}

UnaryExp *GrammarAnalyzer_Rework::number() {
    Element num;

    num = sym;
    pushSymbol();

    return new UnaryExp(num);
}

UnaryExp *GrammarAnalyzer_Rework::unaryExp() {
    if (SYM == LPARENT || SYM == INTCON) {
        return primaryExp();
    } else if (SYM == IDENFR || SYM == GETINTTK) {
        Element idt = sym;
        vector<Exp *> rParams;

        pushSymbol();
        if (SYM == LPARENT) {
            Element symbol = sym;
            pushSymbol();
            if (SYM != RPARENT && isExp())
                rParams = funcRParams();

            pushSymbol();

            return new CallExp(idt, rParams);
        } else {
            ptr -= 2;
            sym = nextSymbol();
            return primaryExp();
        }
    } else if (SYM == PLUS || SYM == MINU || SYM == NOT) {
        Element op = unaryOp();
        UnaryExp *e = unaryExp();

        return new UnaryExp(op, e);
    }

    return nullptr;
}

Element GrammarAnalyzer_Rework::unaryOp() {
    Element op = sym;
    if (SYM == PLUS || SYM == MINU || SYM == NOT) {
        pushSymbol();
    }

    return op;
}

vector<Exp *> GrammarAnalyzer_Rework::funcRParams() {
    vector<Exp *> rParams;

    rParams.push_back(exp());
    while (SYM == COMMA) {
        pushSymbol();
        rParams.push_back(exp());
    }

    return rParams;
}

Exp *GrammarAnalyzer_Rework::mulExp() {
    Exp *lhs = nullptr, *rhs = nullptr;
    Element op;

    lhs = unaryExp();
    while (SYM == MULT || SYM == DIV || SYM == MOD) {
        op = sym;
        pushSymbol();
        rhs = unaryExp();
        lhs = new BinaryExp(lhs, op, rhs);
    }

    return lhs;
}

Exp *GrammarAnalyzer_Rework::addExp() {
    Exp *lhs = nullptr, *rhs = nullptr;
    Element op;

    lhs = mulExp();
    while (SYM == PLUS || SYM == MINU) {
        op = sym;
        pushSymbol();
        rhs = mulExp();
        lhs = new BinaryExp(lhs, op, rhs);
    }

    return lhs;
}

Exp *GrammarAnalyzer_Rework::relExp() {
    Exp *lhs = nullptr, *rhs = nullptr;
    Element op;

    lhs = addExp();
    while (SYM == LSS || SYM == LEQ || SYM == GRE || SYM == GEQ) {
        op = sym;
        pushSymbol();
        rhs = addExp();
        lhs = new BinaryExp(lhs, op, rhs);
    }

    return lhs;
}

Exp *GrammarAnalyzer_Rework::eqExp() {
    Exp *lhs = nullptr, *rhs = nullptr;
    Element op;

    lhs = relExp();
    while (SYM == EQL || SYM == NEQ) {
        op = sym;
        pushSymbol();
        rhs = relExp();
        lhs = new BinaryExp(lhs, op, rhs);
    }

    return lhs;
}

Exp *GrammarAnalyzer_Rework::lAndExp() {
    Exp *lhs = nullptr, *rhs = nullptr;
    Element op;

    lhs = eqExp();
    while (SYM == AND) {
        op = sym;
        pushSymbol();
        rhs = eqExp();
        lhs = new BinaryExp(lhs, op, rhs);
    }

    return lhs;
}

Exp *GrammarAnalyzer_Rework::lOrExp() {
    Exp *lhs = nullptr, *rhs = nullptr;
    Element op;

    lhs = lAndExp();
    while (SYM == OR) {
        op = sym;
        pushSymbol();
        rhs = lAndExp();
        lhs = new BinaryExp(lhs, op, rhs);
    }

    return lhs;
}

Exp *GrammarAnalyzer_Rework::constExp() {
    Exp *node = addExp();

    return node;
}

Element GrammarAnalyzer_Rework::ident() {
    if (SYM == IDENFR) {
        Element s = sym;
        pushSymbol();
        return s;
    }
    return {};
}

Element GrammarAnalyzer_Rework::formatString() {
    if (SYM == STRCON) {
        Element name = sym;
        pushSymbol();
        return name;
    }else{
        return {};
    }
}

Program *GrammarAnalyzer_Rework::getProgram() {
    return this->program;
}
