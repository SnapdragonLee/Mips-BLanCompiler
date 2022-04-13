//
// Created by SNAPDRAGON_11800H on 15/10/2021.
//

#ifndef E5_MIPS_REWORK_GRAMMARANALYZER_H
#define E5_MIPS_REWORK_GRAMMARANALYZER_H

#include "AstClass.h"

class GrammarAnalyzer_Rework {

private:
    vector<Element> src;
    Element sym;

    Program *program;
    TableList table;
    int ptr;

    void pushSymbol();

    Element nextSymbol();

    Element viewNextSymbol();

    Element viewNextSymbol(int i);

    Element bType();

    Decl *constDef(bool isConst, const Element &type);

    vector<Exp *> constInitVal();

    vector<Decl *> varDecl();

    Decl *varDef(const Element &type);

    int evalInt(Exp *exp);

    Program *compUnit();

    vector<Decl *> decl();

    vector<Decl *> constDecl();

    vector<Exp *> initVal();

    Func *funcDef();

    Func *mainFuncDef();

    Element funcType();

    Exp *exp();

    Exp *cond();

    UnaryExp *lVal();

    UnaryExp *primaryExp();

    vector<Decl *> funcFParams();

    Decl *funcFParam();

    Block *block();

    void blockItem(vector<BlockItem *> &items);

    Stmt *stmt();


    UnaryExp *number();

    UnaryExp *unaryExp();

    Element unaryOp();

    vector<Exp *> funcRParams();

    Exp *mulExp();

    Exp *addExp();

    Exp *relExp();

    Exp *eqExp();

    Exp *lAndExp();

    Exp *lOrExp();

    Exp *constExp();

    bool isExp() const;

    bool isBlockItem();

    Element ident();

    Element formatString();


public:
    GrammarAnalyzer_Rework(Element *symbols, int num);

    void analyze();

    Program *getProgram();
};


#endif //E5_MIPS_REWORK_GRAMMARANALYZER_H
