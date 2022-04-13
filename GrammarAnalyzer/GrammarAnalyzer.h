//
// Created by SNAPDRAGON_11800H on 2021/10/3.
//

#ifndef E2_GRAMMAR_ANALYSIS_GRAMMARANALYZER_H
#define E2_GRAMMAR_ANALYSIS_GRAMMARANALYZER_H

#include "../include_Base/Base_settings.h"
#include "../LexicalAnalyzer/LexicalAnalyzer.h"
#include "../ErrorProcess/ErrorList.h"
#include "Object_Def/AddExp_Obj.h"
#include "Object_Def/Block_Obj.h"
#include "Object_Def/CompUnit_Obj.h"
#include "Object_Def/Cond_Obj.h"
#include "Object_Def/ConstDecl_Obj.h"
#include "Object_Def/ConstDef_Obj.h"
#include "Object_Def/ConstExp_Obj.h"
#include "Object_Def/ConstInitVal_Obj.h"
#include "Object_Def/Decl_Obj.h"
#include "Object_Def/EqExp_Obj.h"
#include "Object_Def/Exp_Obj.h"
#include "Object_Def/FormatString_Obj.h"
#include "Object_Def/FuncDef_Obj.h"
#include "Object_Def/FuncFParam_Obj.h"
#include "Object_Def/FuncFParams_Obj.h"
#include "Object_Def/FuncParams_Obj.h"
#include "Object_Def/FuncRParams_Obj.h"
#include "Object_Def/InitVal_Obj.h"
#include "Object_Def/LAndExp_Obj.h"
#include "Object_Def/LOrExp_Obj.h"
#include "Object_Def/LVal_Obj.h"
#include "Object_Def/MainFuncDef_Obj.h"
#include "Object_Def/MulExp_Obj.h"
#include "Object_Def/PrimaryExp_Obj.h"
#include "Object_Def/RelExp_Obj.h"
#include "Object_Def/Stmt_Obj.h"
#include "Object_Def/Sign_Obj.h"
#include "Object_Def/UnaryExp_Obj.h"
#include "Object_Def/UnaryOp_Obj.h"
#include "Object_Def/VarDecl_Obj.h"
#include "Object_Def/VarDef_Obj.h"

#include "Object_Def/ConstIdent_Obj.h"
#include "Object_Def/FuncIdent_Obj.h"
#include "Object_Def/VarIdent_Obj.h"
#include "Object_Def/ConstNum_Obj.h"
#include "Object_Def/Operation_Obj.h"
#include "Object_Def/Print_Obj.h"


#ifdef Grammar
#define print_Tp(T) do { \
                       if (!cantPrint) { \
                           out += '<'; \
                           switch (T) { \
                               Case (CompUnit); \
                               Case (ConstDecl); \
                               Case (ConstDef); \
                               Case (ConstInitVal); \
                               Case (VarDecl); \
                               Case (VarDef); \
                               Case (InitVal); \
                               Case (FuncDef); \
                               Case (MainFuncDef); \
                               Case (FuncType); \
                               Case (FuncFParams); \
                               Case (FuncFParam); \
                               Case (Block); \
                               Case (Stmt); \
                               Case (Exp); \
                               Case (Cond); \
                               Case (LVal); \
                               Case (PrimaryExp); \
                               Case (Number); \
                               Case (UnaryExp); \
                               Case (UnaryOp); \
                               Case (FuncRParams); \
                               Case (MulExp); \
                               Case (AddExp); \
                               Case (RelExp); \
                               Case (EqExp); \
                               Case (LAndExp); \
                               Case (LOrExp); \
                               Case (ConstExp); \
                               Case (FormatString); \
                               /*Case (BlockItem);*/ \
                               /*Case (Decl);*/ \
                               /*Case (UNKNOWN);*/ \
                               /*Case (Ident);*/ \
                               default: \
                                   break; \
                           } \
                           out += ">\n"; \
                       }     \
                   } while (0)
#else
#define print_Tp(T) do{}while(0)
#endif

class Grammar_Analyzer {

    /* private variety defined here */
    int index;
    Element *elemList;
    Element *currElem;
    Element *nxtElem;
    ErrorList *err;

    /* private function defined here */
    explicit Grammar_Analyzer(Lexical_Analyzer *t, ErrorList *error);

    void initial();

    void getElem();

    void release_last(vector<BNode *> *e);

    void paramCheck(vector<BNode *> *real, vector<BNode *> *definition);

    void LVal_Ana(vector<BNode *> *e);

    void Exp_Ana(vector<BNode *> *e);

    void FuncRParams_Ana(vector<BNode *> *e);

    void PrimaryExp_Ana(vector<BNode *> *e);

    void UnaryExp_Ana(vector<BNode *> *e);

    void MulExp_Ana(vector<BNode *> *e);

    void AddExp_Ana(vector<BNode *> *e);

    void ConstInitVal_Ana(vector<BNode *> *e);

    void ConstExp_Ana(vector<BNode *> *e);

    void ConstDef_Ana(vector<BNode *> *e);

    void ConstDecl_Ana(vector<BNode *> *e);

    void InitVal_Ana(vector<BNode *> *e);

    void VarDecl_Ana(vector<BNode *> *e);

    void VarDef_Ana(vector<BNode *> *e);

    void Decl_Ana(vector<BNode *> *e);

    void FuncFParam_Ana(vector<BNode *> *e);

    void FuncFParams_Ana(vector<BNode *> *e);

    void BlockItem_Ana(vector<BNode *> *e);

    void Stmt_Ana(vector<BNode *> *e);

    void Cond_Ana(vector<BNode *> *e);

    void LOrExp_Ana(vector<BNode *> *e);

    void LAndExp_Ana(vector<BNode *> *e);

    void EqExp_Ana(vector<BNode *> *e);

    void RelExp_Ana(vector<BNode *> *e);

    void Block_Ana(vector<BNode *> *e);

    void FuncDef_Ana(vector<BNode *> *e);

    void MainFuncDef_Ana(vector<BNode *> *e);

    void GAnalyze();

public:
    struct efficiency {
        int shell;                         // 层数
        BNode *data;                       // data

        efficiency(int s, BNode *p) {
            shell = s;
            data = p;
        }
    };

    typedef struct efficiency Efficiency;

    bool needReturn, cantPrint;
    int shellNum, whileNum, previous;
    string out;

    BNode *compUnit{};

    unordered_map<string, vector<Efficiency *>> symTable{300};


    static Grammar_Analyzer &getGrammarResult(Lexical_Analyzer *obj, ErrorList *error);

    string &result();
};


#endif //E2_GRAMMAR_ANALYSIS_GRAMMARANALYZER_H
