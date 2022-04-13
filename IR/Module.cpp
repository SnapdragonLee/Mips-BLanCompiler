//
// Created by SNAPDRAGON_11800H on 17/11/2021.
//

#include "Module.h"

Module::Module() = default;

void Module::addFunc(IrFunc *func) {
    funcs[func->getName()] = func;
    irFuncs.insert(irFuncs.begin(), 1, func);
}

IrFunc *Module::getFunc(const string &name) {
    return funcs[name];
}

void Module::addDecl(Decl *d, DeclInst *decl) {
    globals[d] = decl->getVar();
    decls.push_back(decl);
}

void Module::addStr(const string &str) {
    if (strings.count(str) == 0) {
        strings.insert(str);
    }
}

VarClass *Module::getGlobal(Decl *d) {
    return globals[d];
}

string &Module::getResult() {
#ifdef IR
    for (auto x: decls) {
        output += x->getIR() + "\n";
    }
    for (auto x: irFuncs) {
        output += x->getIR() + "\n";
    }
#endif
    return output;
}

