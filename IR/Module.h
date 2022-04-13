//
// Created by SNAPDRAGON_11800H on 17/11/2021.
//

#ifndef E5_MIPS_REWORK_ADDEXP_OBJ_H
#define E5_MIPS_REWORK_ADDEXP_OBJ_H

#include "IrFunc.h"

class IrFunc;

class DeclInst;

class Module {
public:
    vector<DeclInst *> decls;
    vector<IrFunc *> irFuncs;
    set<string> strings;

    Module();

    VarClass *getGlobal(Decl *d);

    void addFunc(IrFunc *func);

    void addDecl(Decl *d, DeclInst *decl);

    void addStr(const string &str);

    IrFunc *getFunc(const string &name);

    string &getResult();

private:
    string output;
    map<string, IrFunc *> funcs;
    map<Decl *, VarClass *> globals;
};


#endif //E5_MIPS_REWORK_ADDEXP_OBJ_H
