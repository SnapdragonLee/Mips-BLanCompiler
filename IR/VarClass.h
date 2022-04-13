//
// Created by SNAPDRAGON_11800H on 17/11/2021.
//

#ifndef E5_MIPS_REWORK_VARCLASS_H
#define E5_MIPS_REWORK_VARCLASS_H

#include "../GrammarAnalyzer/TypeDefine.h"
#include "../GrammarAnalyzer/AstClass.h"

class Decl;

class VarClass {
public:
    VarClass();

    VarClass(VarClass *var);

    VarClass(int id, bool is_global, TypeDefine bType);

    int getId() const;

    bool isGlobal() const;

    bool isAddr() const;

    TypeDefine getType();

    void addDims(vector<VarClass *> dims);

    void addAddr();

    virtual string getIR();

private:
    int id;
    bool is_global, is_addr;
    TypeDefine bType;

    vector<VarClass *> dims;

    string showDim();
};

class Constant : public VarClass {
public:
    Keyword type;
    int value;
    string str;

    Constant();

    explicit Constant(int x);

    explicit Constant(string s);

    int getValue() const;

    string getStr() const;

    string getIR() override;
};

class IrParam : public VarClass {
public:
    Constant *constant;

    IrParam();

    explicit IrParam(int varId, Decl *decl);

    IrParam(VarClass *var);

    IrParam(Constant *constant);

    string getIR() override;
};

class IrArray : public VarClass {
public:
    bool isConst;

    IrArray();

    IrArray(int id, bool is_global, const TypeDefine &bType);

    void addConst();

    int getBase() const;

    int getSize() const;

    string getIR() override;

private:
    int base, size;
};

class IrPointer : public IrParam {
public:
    bool isConst;

    IrPointer();

    IrPointer(int base, Decl *decl);

    void addConst();

    string getIR() override;

private:
    int base;
};


#endif //E5_MIPS_REWORK_VARCLASS_H
