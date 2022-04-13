//
// Created by SNAPDRAGON_11800H on 17/10/2021.
//

#ifndef E5_MIPS_REWORK_ASTCLASS_H
#define E5_MIPS_REWORK_ASTCLASS_H

#include "../include_Base/Base_settings.h"
#include "TypeDefine.h"
#include "../IR/Ctx.h"
#include "../IR/Module.h"

class Module;

class Ctx;

class Program;

class VarClass;

class AstClass {
private:
    Program *program;

public:
    AstClass();

    explicit AstClass(Program *p);

    static Ctx *context;

    Module *generateCode();
};

class ProgramItem {
public:
    ProgramItem();

    virtual void generateCode();
};

class BlockItem {
public:
    BlockItem();

    virtual void generateCode();
};

class Exp;

class Decl : public ProgramItem, public BlockItem {
private:
    TypeDefine bType;
    Element ident;
    vector<Exp *> initVal;

public:
    Decl();

    Decl(TypeDefine bType, Element ident);

    Decl(TypeDefine bType, Element ident, vector<Exp *> initVal);

    bool isConst();

    bool isParam();

    int getDim();

    TypeDefine getType();

    Element getIdent();

    vector<int> getDims();

    vector<int> getInitVal();

    vector<Exp *> getInitValExp();

    bool hasInit();

    bool isArray();

    void generateCode() override;
};

class Stmt : public BlockItem {
public:
    Stmt();

    void generateCode() override;
};

class ReturnStmt;

class LoopStmt;

class Block : public Stmt {
private:
    vector<BlockItem *> block_items;
    Element lBrace, rBrace;

public:
    Block();

    Block(Element lBrace, vector<BlockItem *> items, Element rBrace);

    void generateCode() override;

};

class Func : public ProgramItem {
private:
    Element ident;
    Block *block;
    TypeDefine returnType;
    vector<Decl *> fParams;

public:
    Func();

    Func(TypeDefine returnType, Element ident, vector<Decl *> fParams, Block *block);

    TypeDefine getType();

    Element getIdent();

    void generateCode() override;

};

class Exp {
private:
    VarClass *var;
    bool cond;

public:
    Exp();

    void addCond();

    virtual void addVar(VarClass *variable);

    virtual VarClass *getVar();

    virtual int evalInt();

    virtual TypeDefine evalType();

    virtual void generateCode();

};

class ExpStmt : public Stmt {
private:
    int varId;
    Exp *exp;

public:
    ExpStmt();

    explicit ExpStmt(Exp *e);

    int genVar();

    void generateCode() override;

};

class CondStmt : public Stmt {
private:
    Element token;
    Exp *condExp;
    Stmt *ifStmt;
    Stmt *elseStmt;

public:
    CondStmt();

    CondStmt(Element token, Exp *condExp, Stmt *ifStmt, Stmt *elseStmt = nullptr);

    void generateCode() override;

};

class LoopStmt : public Stmt {
private:
    Element sym;

public:
    LoopStmt();

    explicit LoopStmt(Element sym);

    void generateCode() override;

};

class ReturnStmt : public Stmt {
private:
    Exp *exp;
    Element sym;

public:
    ReturnStmt();

    explicit ReturnStmt(Element sym, Exp *exp);

    void generateCode() override;

};

class UnaryExp : public Exp {
private:
    Element *op, sym;
    Exp *exp;

public:
    UnaryExp();

    explicit UnaryExp(Exp *exp);

    explicit UnaryExp(Element sym);

    explicit UnaryExp(Element op, Element sym);

    explicit UnaryExp(Element op, Exp *exp);

    Exp *getExp();

    Element *getOp();

    Element getSymbol();

    virtual TypeDefine evalType();

    virtual int evalInt();

    virtual void generateCode();

};

class BinaryExp : public Exp {
private:
    Element op;
    Exp *lhs, *rhs;

public:
    BinaryExp();

    BinaryExp(Exp *lhs, Element op, Exp *rhs);

    Element getOp();

    Exp *getLhs();

    Exp *getRhs();

    virtual int evalInt();

    virtual TypeDefine evalType();

    virtual void generateCode();

};

class FormatString : public Exp {
private:
    Element sym;
public:
    FormatString();

    explicit FormatString(Element str);

    Element getSym();

    void generateCode() override;

};

class LVal : public UnaryExp {
private:
    Element ident;
    vector<Exp *> dims;

public:
    LVal();

    explicit LVal(Element sym, vector<Exp *> dims);

    Element getIdent();

    vector<Exp *> getDims();

    int evalInt() override;

    TypeDefine evalType() override;

    void generateCode() override;

};

class AssignExp : public BinaryExp {
private:
    LVal *lhs;
    Exp *rhs;

public:
    AssignExp();

    AssignExp(LVal *l, Exp *r);

    int evalInt() override;

    TypeDefine evalType() override;

    void generateCode() override;

};

class CallExp : public UnaryExp {
private:
    Element ident;
    vector<Exp *> rParams;

public:
    CallExp();

    explicit CallExp(Element ident);

    explicit CallExp(Element ident, vector<Exp *> rParams);

    Element getIdent();

    bool isGetInt() const;

    bool isPrintf() const;

    int evalInt() override;

    TypeDefine evalType() override;

    void generateCode() override;

};

class Program {
private:
    vector<ProgramItem *> program_items;

public:
    Program();

    explicit Program(vector<ProgramItem *> items);

    void generateCode();
};

#endif //E5_MIPS_REWORK_ASTCLASS_H
