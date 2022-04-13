//
// Created by SNAPDRAGON_11800H on 2021/10/4.
//

#include "GrammarAnalyzer.h"

#define currET        (currElem -> sym)        //current element type (Keyword)
#define currEN        (currElem -> val)        //current element name

#define nxtET        (nxtElem -> sym)        //current element name
#define nxtEN        (nxtElem -> val)        //current element name

#define NXT_IDENT      (nxtET == IDENFR      )      // Ident
#define NXT_CONSTNUM   (nxtET == INTCON      )      // Number
#define NXT_STR        (nxtET == STRCON      )      // FormatString
#define NXT_MAIN       (nxtET == MAINTK      )      // main
#define NXT_CONST      (nxtET == CONSTTK     )      // const
#define NXT_INT        (nxtET == INTTK       )      // int
#define NXT_BREAK      (nxtET == BREAKTK     )      // break
#define NXT_CONTINUE   (nxtET == CONTINUETK  )      // continue
#define NXT_IF         (nxtET == IFTK        )      // if
#define NXT_ELSE       (nxtET == ELSETK      )      // else
#define NXT_WHILE      (nxtET == WHILETK     )      // while
#define NXT_GETINT     (nxtET == GETINTTK    )      // getint
#define NXT_PRINTF     (nxtET == PRINTFTK    )      // printf
#define NXT_RETURN     (nxtET == RETURNTK    )      // return
#define NXT_VOID       (nxtET == VOIDTK      )      // void
#define NXT_NOT        (nxtET == NOT         )      // !
#define NXT_AND        (nxtET == AND         )      // &&
#define NXT_OR         (nxtET == OR          )      // ||
#define NXT_PLUS       (nxtET == PLUS        )      // +
#define NXT_MINU       (nxtET == MINU        )      // -
#define NXT_MULT       (nxtET == MULT        )      // *
#define NXT_DIV        (nxtET == DIV         )      // /
#define NXT_MOD        (nxtET == MOD         )      // %
#define NXT_LSS        (nxtET == LSS         )      // <
#define NXT_LEQ        (nxtET == LEQ         )      // <=
#define NXT_GRE        (nxtET == GRE         )      // >
#define NXT_GEQ        (nxtET == GEQ         )      // >=
#define NXT_EQL        (nxtET == EQL         )      // ==
#define NXT_NEQ        (nxtET == NEQ         )      // !=
#define NXT_ASSIGN     (nxtET == ASSIGN      )      // =
#define NXT_SEMICN     (nxtET == SEMICN      )      // ;
#define NXT_COMMA      (nxtET == COMMA       )      // ,
#define NXT_LPARENT    (nxtET == LPARENT     )      // (
#define NXT_RPARENT    (nxtET == RPARENT     )      // )
#define NXT_LBRACK     (nxtET == LBRACK      )      // [
#define NXT_RBRACK     (nxtET == RBRACK      )      // ]
#define NXT_LBRACE     (nxtET == LBRACE      )      // {
#define NXT_RBRACE     (nxtET == RBRACE      )      // }

#define preload2 (elemList[index+1])
#define preload3 (elemList[index+2])

#define NodeExtend(e, Part, Constuctor) do { \
                                BNode *node = new BNode(Part); \
                                (Grammar_Analyzer::Constuctor)(&node->chd); \
                                print_Tp(node->tp); \
                                e->push_back(node); \
                            } while(0)

ConstNum_Obj constOne = ConstNum_Obj(new string("1"));

Grammar_Analyzer &Grammar_Analyzer::getGrammarResult(Lexical_Analyzer *obj, ErrorList *error) {
    static Grammar_Analyzer real(obj, error);
    return real;
}

Grammar_Analyzer::Grammar_Analyzer(Lexical_Analyzer *t, ErrorList *error) {
    elemList = t->elemList;
    err = error;

    initial();
    GAnalyze();
    print_Tp(compUnit->tp);
}

void Grammar_Analyzer::initial() {
    index = shellNum = whileNum = previous = 0;
    needReturn = cantPrint = false;
    currElem = &elemList[index];
    nxtElem = &elemList[index];
    compUnit = new BNode(CompUnit);
}

void Grammar_Analyzer::getElem() {
    currElem = &elemList[index];

#ifdef Compose_Grammar
    if (!cantPrint) {
        printsym(currElem->sym);
        out.append(" " + currElem->val + "\n");
    }
#endif
    index++;
    nxtElem = &elemList[index];
}

void Grammar_Analyzer::release_last(vector<BNode *> *e) {
    auto it = (*e).end();
    *e->erase(it - 1);
}

void Grammar_Analyzer::paramCheck(vector<BNode *> *real, vector<BNode *> *definition) {
    if ((definition == nullptr && !real->empty()) || (real->size() != definition->size())) {
        err->add2ErrList(currElem->row, 'd');
        return;
    }
    for (int i = 0; i < real->size(); i++) {
        BNode *node = (*real)[i]->chd[0]->chd[0]->chd[0];
        do {
            node = node->chd[0];
            BNode *cI = dynamic_cast<FuncIdent_Obj *>(node);
            if (cI != nullptr) {
                if ((((FuncIdent_Obj *) cI)->returnType != INTTK ||
                     ((VarIdent_Obj *) (*definition)[i]->chd[0])->dim != 0)) {
                    err->add2ErrList(currElem->row, 'e');
                    return;
                }
                continue;
            }

            cI = dynamic_cast<ConstNum_Obj *>(node);
            if (cI != nullptr) {
                if (((VarIdent_Obj *) (*definition)[i]->chd[0])->dim != 0) {
                    err->add2ErrList(currElem->row, 'e');
                    return;
                }
                continue;
            }

            cI = dynamic_cast<VarIdent_Obj *>(node);
            if (cI != nullptr) {
                if (((VarIdent_Obj *) (*definition)[i]->chd[0])->dim != ((ConstIdent_Obj *) cI)->dim) {
                    err->add2ErrList(currElem->row, 'e');
                    return;
                }
                continue;
            }

            cI = dynamic_cast<ConstIdent_Obj *>(node);
            if (cI != nullptr) {
                if (((ConstIdent_Obj *) (*definition)[i]->chd[0])->dim != ((ConstIdent_Obj *) cI)->dim) {
                    err->add2ErrList(currElem->row, 'e');
                    return;
                }
                continue;
            }
        } while (!node->chd.empty());
    }
}

void Grammar_Analyzer::LVal_Ana(vector<BNode *> *e) {

    auto *cI = new VarIdent_Obj(&nxtEN, Ident, false);
    e->push_back(cI);

    int dim = -1;
    auto *efficiency = new Efficiency(shellNum, cI);
    auto iter = symTable.find(nxtEN);
    if (!cantPrint && (iter == symTable.end() || iter->second.empty())) {
#ifdef Error_Detect
        err->add2ErrList(currElem->row, 'c');
        delete efficiency;
#endif
    } else if (iter != symTable.end() && !iter->second.empty()) {
        BNode *node = dynamic_cast<VarIdent_Obj *>(iter->second.back()->data);
        if (node != nullptr) {
            dim = ((VarIdent_Obj *) node)->dim;
        }
        node = dynamic_cast<ConstIdent_Obj *>(iter->second.back()->data);
        if (node != nullptr) {
            dim = ((ConstIdent_Obj *) node)->dim;
        }
    }

    getElem();
    if (NXT_LBRACK) {
        getElem();

        NodeExtend(e, Exp, Exp_Ana);

        if (NXT_RBRACK) {
            getElem();
        } else {
#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'k');
#endif
        }

        if (NXT_LBRACK) {
            getElem();

            NodeExtend(e, Exp, Exp_Ana);

            if (NXT_RBRACK) {
                getElem();
            } else {
#ifdef Error_Detect
                err->add2ErrList(currElem->row, 'k');
#endif
            }
            cI->dim = dim - 2;
        } else {
            cI->dim = dim - 1;
        }
    } else {
        cI->dim = dim;
    }
}

void Grammar_Analyzer::Exp_Ana(vector<BNode *> *e) {
    NodeExtend(e, AddExp, AddExp_Ana);
}

void Grammar_Analyzer::PrimaryExp_Ana(vector<BNode *> *e) {

    if (NXT_LPARENT) {
        getElem();
        NodeExtend(e, Exp, Exp_Ana);
        if (!NXT_RPARENT) {/*throw;*/} //void error
        getElem();
    } else if (NXT_IDENT) {
        NodeExtend(e, LVal, LVal_Ana);
    } else if (NXT_CONSTNUM) {
        getElem();
        auto *node = new ConstNum_Obj(&currEN);
        print_Tp(node->tp);
        e->push_back(node);
    }
}

void Grammar_Analyzer::FuncRParams_Ana(vector<BNode *> *e) {
    FRPA:
    NodeExtend(e, Exp, Exp_Ana);
    if (NXT_COMMA) {
        getElem();
        goto FRPA;
    }
}

void Grammar_Analyzer::UnaryExp_Ana(vector<BNode *> *e) {

    if (NXT_LPARENT || (NXT_IDENT && preload2.sym != LPARENT) || NXT_CONSTNUM) {
        NodeExtend(e, PrimaryExp, PrimaryExp_Ana);
    } else if (NXT_IDENT && preload2.sym == LPARENT) {
        auto *cI = new FuncIdent_Obj(&nxtEN, Ident);
        auto *efficiency = new Efficiency(shellNum, cI);
        auto iter = symTable.find(*cI->name);
        if (iter == symTable.end() || iter->second.empty()) {
#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'c');
            delete efficiency;
#endif
        } else {
            cI->returnType = ((FuncIdent_Obj *) iter->second.back()->data)->returnType;
            cI->funcParam = ((FuncIdent_Obj *) iter->second.back()->data)->funcParam;
        }
        e->push_back(cI);

        getElem();
        if (preload2.sym != RPARENT) {
            getElem();
            NodeExtend(e, FuncRParams, FuncRParams_Ana);
            cI->funcParam = (*e).back()->chd;
        } else {
            getElem();
        }
#ifdef Error_Detect
        paramCheck(&e->back(), &cI);
#endif

        if (NXT_RPARENT) {
            getElem();
        } else {
#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'j');
#endif
        }


    } else if (NXT_PLUS || NXT_MINU || NXT_NOT) {
        getElem();
        auto *node = new Operation_Obj(currET, UnaryOp);
        print_Tp(node->tp);
        e->push_back(node);

        NodeExtend(e, UnaryExp, UnaryExp_Ana);

    } else {/*throw;*/} //void error
}


void Grammar_Analyzer::MulExp_Ana(vector<BNode *> *e) {
    auto *node = new BNode(UnaryExp);
    UnaryExp_Ana(&node->chd);
    print_Tp(node->tp);

    while (NXT_MULT || NXT_DIV || NXT_MOD) {
        auto *pNode = new Operation_Obj(nxtET, MulExp);
        print_Tp(pNode->tp);
        pNode->chd.push_back(node);
        getElem();
        UnaryExp_Ana(&pNode->chd);
        print_Tp(UnaryExp);
        node = pNode;
    }
    e->push_back(node);
}

void Grammar_Analyzer::AddExp_Ana(vector<BNode *> *e) {
    auto *node = new BNode(MulExp);
    MulExp_Ana(&node->chd);
    print_Tp(node->tp);

    while (NXT_PLUS || NXT_MINU) {
        auto *pNode = new Operation_Obj(nxtET, AddExp);
        print_Tp(pNode->tp);
        pNode->chd.push_back(node);
        getElem();
        MulExp_Ana(&pNode->chd);
        print_Tp(MulExp);
        node = pNode;
    }
    e->push_back(node);
}

void Grammar_Analyzer::ConstExp_Ana(vector<BNode *> *e) {
    NodeExtend(e, AddExp, AddExp_Ana);
}

void Grammar_Analyzer::ConstInitVal_Ana(vector<BNode *> *e) {
    if (!NXT_LBRACE) {
        NodeExtend(e, ConstExp, ConstExp_Ana);
    } else {
        getElem();
        if (NXT_RBRACE) {
            getElem();
            return;
        }

        CIVA:
        NodeExtend(e, ConstInitVal, ConstInitVal_Ana);
        if (NXT_COMMA) {
            getElem();
            goto CIVA;
        } else if (NXT_RBRACE) {
            getElem();
        }
    }
}

void Grammar_Analyzer::ConstDef_Ana(vector<BNode *> *e) {
    getElem();
    auto *cI = new ConstIdent_Obj(&currEN, Ident);
    e->push_back(cI);

    if (NXT_LBRACK) {
        getElem();
        NodeExtend(e, ConstExp, ConstExp_Ana);

        if (NXT_RBRACK) {
            getElem();
        } else {
#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'k');
#endif
        }

        if (NXT_LBRACK) {
            getElem();
            //int getConst = e->back()->getConst();
            //cI->row = new ConstNum_Obj(new string(to_string(getConst)));
            cI->row = e->back();
            NodeExtend(e, ConstExp, ConstExp_Ana);

            if (NXT_RBRACK) {
                getElem();
            } else {
#ifdef Error_Detect
                err->add2ErrList(currElem->row, 'k');
#endif
            }

            cI->dim = 2;

            //getConst = e->back()->getConst();
            //cI->column = new ConstNum_Obj(new string(to_string(getConst)));
            cI->column = e->back();


            if (!NXT_ASSIGN) {/*throw;*/} //void error

        } else {
            cI->dim = 1;
            //int getConst = e->back()->getConst();
            //cI->column = new ConstNum_Obj(new string(to_string(getConst)));

            cI->column = e->back();
            cI->row = &constOne;
        }
    } else {
        cI->dim = 0;
        cI->column = cI->row = &constOne;
    }

    getElem();
    NodeExtend(e, ConstInitVal, ConstInitVal_Ana);

    cI->init = e->back();
    //delete e->back();
    release_last(e);

    //////////////////////////////////////////////////////
    auto *efficiency = new Efficiency(shellNum, cI);
    auto iter = symTable.find(*cI->name);
    if (iter != symTable.end()) {
        if (!iter->second.empty() && shellNum == iter->second.back()->shell) {

#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'b');
            delete efficiency;
#endif
        } else {
            iter->second.push_back(efficiency);
        }
    } else {
        vector<Efficiency *> vec;
        vec.push_back(efficiency);
        symTable.insert(pair<string, vector<Efficiency *>>(*cI->name, vec));
    }
    //////////////////////////////////////////////////////
}

void Grammar_Analyzer::ConstDecl_Ana(vector<BNode *> *e) {
    while (NXT_IDENT) {
        NodeExtend(e, ConstDef, ConstDef_Ana);
        if (NXT_SEMICN) {
            getElem();
            break;
        } else if (!NXT_COMMA) {
#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'i');
            return;
#endif
        }// Must be COMMA
        getElem();
    }
}


void Grammar_Analyzer::InitVal_Ana(vector<BNode *> *e) {
    if (!NXT_LBRACE) {
        NodeExtend(e, Exp, Exp_Ana);
    } else {
        getElem();
        if (NXT_RBRACE) {
            getElem();
            return;
        }

        IVA:
        NodeExtend(e, InitVal, InitVal_Ana);
        if (NXT_COMMA) {
            getElem();
            goto IVA;
        } else if (NXT_RBRACE) {
            getElem();
        }
    }
}


void Grammar_Analyzer::VarDef_Ana(vector<BNode *> *e) {
    getElem();
    auto *cI = new VarIdent_Obj(&currEN, Ident, true);
    e->push_back(cI);

    if (NXT_LBRACK) {
        getElem();

        NodeExtend(e, ConstExp, ConstExp_Ana);

        if (NXT_RBRACK) {
            getElem();
        } else {
#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'k');
#endif
        }

        if (NXT_LBRACK) {
            cI->row = e->back();
            getElem();

            NodeExtend(e, ConstExp, ConstExp_Ana);
            cI->dim = 2;
            cI->column = e->back();

            if (NXT_RBRACK) {
                getElem();
            } else {
#ifdef Error_Detect
                err->add2ErrList(currElem->row, 'k');
#endif
            }

        } else {
            cI->dim = 1;
            cI->row = &constOne;
            cI->column = e->back();
        }
    } else {
        cI->dim = 0;
        cI->row = cI->column = &constOne;
    }

    if (NXT_ASSIGN) {
        getElem();
        NodeExtend(e, InitVal, InitVal_Ana);
        cI->init = e->back();
        //delete e->back();

        release_last(e);
    }

    auto *efficiency = new Efficiency(shellNum, cI);
    auto iter = symTable.find(*cI->name);
    if (iter != symTable.end()) {
        if (!iter->second.empty() && shellNum == iter->second.back()->shell) {
#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'b');
            delete efficiency;
#endif
        } else {
            iter->second.push_back(efficiency);
        }
    } else {
        vector<Efficiency *> vec;
        vec.push_back(efficiency);
        symTable.insert(pair<string, vector<Efficiency *>>(*cI->name, vec));
    }
}

void Grammar_Analyzer::VarDecl_Ana(vector<BNode *> *e) {
    while (NXT_IDENT) {
        NodeExtend(e, VarDef, VarDef_Ana);

        if (NXT_SEMICN) {
            getElem();
            break;
        } else if (!NXT_COMMA) {
#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'i');
            return;
#endif
        }  //Must be COMMA
        getElem();
    }
}

void Grammar_Analyzer::Decl_Ana(vector<BNode *> *e) {
    if (NXT_CONST) { //Tested
        getElem();
        getElem();
        NodeExtend(e, ConstDecl, ConstDecl_Ana);
    } else {
        getElem();
        NodeExtend(e, VarDecl, VarDecl_Ana);
    }
}

void Grammar_Analyzer::FuncFParam_Ana(vector<BNode *> *e) {

    if (NXT_INT) {
        getElem();
        getElem();
        auto *cI = new VarIdent_Obj(&currEN, Ident, true);
        e->push_back(cI);

        if (NXT_LBRACK) {
            getElem();

            if (NXT_RBRACK) {
                getElem();
            } else {
#ifdef Error_Detect
                err->add2ErrList(currElem->row, 'k');
#endif
            }

        } else {
            cI->dim = 0;
            cI->column = cI->row = &constOne;

            auto *efficiency = new Efficiency(shellNum, cI);
            auto iter = symTable.find(*cI->name);
            if (iter != symTable.end()) {
                if (!iter->second.empty() && shellNum == iter->second.back()->shell) {
#ifdef Error_Detect
                    err->add2ErrList(currElem->row, 'b');
                    delete efficiency;
#endif
                } else {
                    iter->second.push_back(efficiency);
                }
            } else {
                vector<Efficiency *> vec;
                vec.push_back(efficiency);
                symTable.insert(pair<string, vector<Efficiency *>>(*cI->name, vec));
            }
            return;
        }

        if (NXT_LBRACK) {
            getElem();

            NodeExtend(e, ConstExp, ConstExp_Ana);
            cI->column = e->back();

            if (NXT_RBRACK) {
                getElem();
            } else {
#ifdef Error_Detect
                err->add2ErrList(currElem->row, 'k');
#endif
            }

            cI->dim = 2;

        } else {
            cI->dim = 1;
        }

        auto *efficiency = new Efficiency(shellNum, cI);
        auto iter = symTable.find(*cI->name);
        if (iter != symTable.end()) {
            if (!iter->second.empty() && shellNum == iter->second.back()->shell) {
#ifdef Error_Detect
                err->add2ErrList(currElem->row, 'b');
                delete efficiency;
#endif
            } else {
                iter->second.push_back(efficiency);
            }
        } else {
            vector<Efficiency *> vec;
            vec.push_back(efficiency);
            symTable.insert(pair<string, vector<Efficiency *>>(*cI->name, vec));
        }
    }
    /*else throw; // void error*/ // Could have Error_Detect language spot.
}

void Grammar_Analyzer::FuncFParams_Ana(vector<BNode *> *e) {
    shellNum++;

    FFPA:
    NodeExtend(e, FuncFParam, FuncFParam_Ana);

    if (NXT_COMMA) {
        getElem();
        goto FFPA;
    }
    shellNum--;
}

void Grammar_Analyzer::RelExp_Ana(vector<BNode *> *e) {
    auto *node = new BNode(AddExp);
    AddExp_Ana(&node->chd);
    print_Tp(node->tp);

    while (NXT_LSS || NXT_LEQ || NXT_GRE ||
           NXT_GEQ) {
        auto *pNode = new Operation_Obj(nxtET, RelExp);
        print_Tp(pNode->tp);
        pNode->chd.push_back(node);
        getElem();
        AddExp_Ana(&pNode->chd);
        print_Tp(AddExp);
        node = pNode;
    }
    e->push_back(node);
}

void Grammar_Analyzer::EqExp_Ana(vector<BNode *> *e) {
    auto *node = new BNode(RelExp);
    RelExp_Ana(&node->chd);
    print_Tp(node->tp);

    while (NXT_EQL || NXT_NEQ) {
        auto *pNode = new Operation_Obj(nxtET, EqExp);
        print_Tp(pNode->tp);
        pNode->chd.push_back(node);
        getElem();
        RelExp_Ana(&pNode->chd);
        print_Tp(RelExp);
        node = pNode;
    }
    e->push_back(node);
}

void Grammar_Analyzer::LAndExp_Ana(vector<BNode *> *e) {
    auto *node = new BNode(EqExp);
    EqExp_Ana(&node->chd);
    print_Tp(node->tp);

    while (NXT_AND) {
        auto *pNode = new Operation_Obj(nxtET, LAndExp);
        print_Tp(pNode->tp);
        pNode->chd.push_back(node);
        getElem();
        EqExp_Ana(&pNode->chd);
        print_Tp(EqExp);
        node = pNode;
    }
    e->push_back(node);
}

void Grammar_Analyzer::LOrExp_Ana(vector<BNode *> *e) {
    auto *node = new BNode(LAndExp);
    LAndExp_Ana(&node->chd);
    print_Tp(node->tp);

    while (NXT_OR) {
        auto *pNode = new Operation_Obj(nxtET, LOrExp);
        print_Tp(pNode->tp);
        pNode->chd.push_back(node);
        getElem();
        LAndExp_Ana(&pNode->chd);
        print_Tp(LAndExp);
        node = pNode;
    }
    e->push_back(node);
}

void Grammar_Analyzer::Cond_Ana(vector<BNode *> *e) {
    NodeExtend(e, LOrExp, LOrExp_Ana);
}

void Grammar_Analyzer::Stmt_Ana(vector<BNode *> *e) {
    if (NXT_IF) {
        BNode *pNode = new Sign_Obj(nxtET);
        e->push_back(pNode);

        getElem();
        if (!NXT_LPARENT) {/*throw;*/} //void error
        getElem();

        NodeExtend(e, Cond, Cond_Ana);

        if (!NXT_RPARENT) {

#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'j');
#endif
        } else {
            getElem();
        }
#

        NodeExtend(e, Stmt, Stmt_Ana);
        if (NXT_ELSE) {
            getElem();
            NodeExtend(e, Stmt, Stmt_Ana);
        }
    } else if (NXT_WHILE) {
        auto *pNode = new Sign_Obj(nxtET);
        e->push_back(pNode);

        getElem();
        if (!NXT_LPARENT) {/*throw;*/} //void error

        getElem();
        NodeExtend(e, Cond, Cond_Ana);
        if (!NXT_RPARENT) {

#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'j');
#endif
        } else {
            getElem();
        }

        whileNum++;
        NodeExtend(e, Stmt, Stmt_Ana);
        whileNum--;

    } else if (NXT_BREAK || NXT_CONTINUE) {
        auto *node = new Sign_Obj(nxtET);
        e->push_back(node);
        getElem();
        if (NXT_SEMICN) {
            getElem();
        } else {
#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'i');
#endif
        }

        if (whileNum == 0) {
#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'm');
#endif
        }
    } else if (NXT_RETURN) {
        auto *pNode = new Sign_Obj(nxtET);
        e->push_back(pNode);
        getElem();
        if (!NXT_SEMICN) {
            NodeExtend(e, Exp, Exp_Ana);
        }

        if (NXT_SEMICN) {
            getElem();
        } else {
#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'i');
#endif
        }

    } else if (NXT_PRINTF) {
        int expTime = 0, dTime = 0;

        getElem();
        getElem();
        if (!NXT_STR) {/*throw;*/} //void error

        for (int i = 0; i < currElem->val.length() - 1; i++) {
            if (currElem->val[i] == '%' && currElem->val[i + 1] == 'd') {
                dTime++;
            }
        }

        auto *pNode = new Print_Obj(&currEN, dTime);
        e->push_back(pNode);

        getElem();
        while (NXT_COMMA) {
            getElem();
            expTime++;
            NodeExtend(e, Exp, Exp_Ana);
            pNode->EXT.push_back(e->back());
        }

        if (expTime != dTime) {
#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'l');
#endif
        }

        if (NXT_RPARENT) {
            getElem();
            if (!NXT_SEMICN) {
#ifdef Error_Detect
                err->add2ErrList(currElem->row, 'i');
#endif
            } else {
                getElem();
            }
        } else {
#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'j');
#endif
            if (NXT_SEMICN) {
                return;
            }
        }

    } else if (NXT_LBRACE) {
        NodeExtend(e, Block, Block_Ana);

    } else if (NXT_SEMICN) {
        getElem();
        return;

    } else if (NXT_IDENT) {
        previous = index;

        auto iter = symTable.find(currElem->val);
        if (iter != symTable.end() && !iter->second.empty()) {
            BNode *node = iter->second.back()->data;
            BNode *cI = dynamic_cast<FuncIdent_Obj *>(node);
            if (cI != nullptr) {
                goto Exp;
            }
        }

        cantPrint = true;
        NodeExtend(e, LVal, LVal_Ana);
        cantPrint = false;

        delete (*e)[0];
        auto it = (*e).begin();
        *e->erase(it);


        if (!NXT_ASSIGN) {
            index = previous;
            currElem = &elemList[index - 1];
            nxtElem = &elemList[index];
            goto Exp;
        }

        index = previous;
        currElem = &elemList[index - 1];
        nxtElem = &elemList[index];
        NodeExtend(e, LVal, LVal_Ana);

#ifdef Error_Detect
        iter = symTable.find(*((VarIdent_Obj *) ((*e)[0]->chd)[0])->name);

        if (iter != symTable.end()) {
            BNode *node = iter->second.back()->data;
            ConstIdent_Obj *c = dynamic_cast<ConstIdent_Obj *>(node);
            if (c != nullptr) {
                err->add2ErrList(currElem->row, 'h');
            }
        }
#endif

        getElem();
        if (NXT_GETINT) {
            auto *s = new Sign_Obj(nxtET);
            e->push_back(s);
            getElem();
            if (preload2.sym != RPARENT) {
#ifdef Error_Detect
                err->add2ErrList(currElem->row, 'j');
#endif
            } else {
                getElem();
                getElem();
            }
        } else {
            NodeExtend(e, Exp, Exp_Ana);
        }

        if (NXT_SEMICN) {
            getElem();
        } else {
#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'i');
#endif
        }
    } else {
        Exp:
        NodeExtend(e, Exp, Exp_Ana);

        if (NXT_SEMICN) {
            getElem();
        } else {
#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'i');
#endif
        }
    }
}

void Grammar_Analyzer::BlockItem_Ana(vector<BNode *> *e) {

    if (NXT_CONST || NXT_INT) {
        auto *node = new BNode(Decl);
        Decl_Ana(&node->chd);
        e->push_back(node);
    } else {
        NodeExtend(e, Stmt, Stmt_Ana);
    }
}

void Grammar_Analyzer::Block_Ana(vector<BNode *> *e) {
    shellNum++;

    if (!NXT_LBRACE) {
        {/*throw;*/} //void error
    } else if (preload2.sym == RBRACE) {
        getElem();
        shellNum--;
        getElem();
        return;
    }
    getElem();

    BLA:
    auto *node = new BNode(BlockItem);
    BlockItem_Ana(&node->chd);
    //print_Tp(node->tp);
    e->push_back(node);
    if (!NXT_RBRACE) {
        goto BLA;
    } else if (NXT_RBRACE) {
        getElem();
    }

    for (auto &iter: symTable) {
        for (auto inner = iter.second.rbegin();
             inner != iter.second.rend(); inner++) {
            if ((*inner)->shell == shellNum) {
                delete *(inner + 1).base();
                iter.second.erase((inner + 1).base());
            }
        }
    }
    shellNum--;
}

void Grammar_Analyzer::FuncDef_Ana(vector<BNode *> *e) {
    auto *cI = new FuncIdent_Obj(currET, &nxtEN, Ident);
    if (currET == INTTK) {
        needReturn = true;
    }
    print_Tp(FuncType);
    e->push_back(cI);

    auto *efficiency = new Efficiency(shellNum, cI);
    auto iter = symTable.find(*cI->name);
    if (iter != symTable.end()) {
        if (!iter->second.empty() && shellNum == iter->second.back()->shell) {
#ifdef Error_Detect
            err->add2ErrList(currElem->row, 'b');
            delete efficiency;
#endif
        } else {
            iter->second.push_back(efficiency);
        }
    } else {
        vector<Efficiency *> vec;
        vec.push_back(efficiency);
        symTable.insert(pair<string, vector<Efficiency *>>(*cI->name, vec));
    }

    getElem();

    if (!NXT_RPARENT) {
        getElem();
        NodeExtend(e, FuncFParams, FuncFParams_Ana);
        cI->funcParam = *&e->back()->chd;
    }

    if (!NXT_RPARENT) {
#ifdef Error_Detect
        err->add2ErrList(currElem->row, 'j');
#endif
    } else {
        getElem();
    }

    NodeExtend(e, Block, Block_Ana);
    cI->Block = *&e->back()->chd;

    shellNum++;
    for (auto &iter: symTable) {
        for (auto inner = iter.second.rbegin();
             inner != iter.second.rend(); inner++) {
            if ((*inner)->shell == shellNum) {
                delete *(inner + 1).base();
                iter.second.erase((inner + 1).base());
            }
        }
    }
    shellNum--;

#ifdef Error_Detect
    if (e->back()->chd.size() == 0) {
        if (needReturn) {
            err->add2ErrList(currElem->row, 'g');
        }
    } else {
        BNode *node = e->back()->chd.back()->chd[0];
        Sign_Obj *s = dynamic_cast<Sign_Obj *>(node->chd[0]);
        if (needReturn) {
            if (s == nullptr || s->relation != RETURNTK || node->chd.size() < 2) {
                err->add2ErrList(currElem->row, 'g');
            }
        } else if (s != nullptr && s->relation == RETURNTK && node->chd.size() > 1) {
            err->add2ErrList(currElem->row - 1, 'f');

        }
    }
#endif
    needReturn = false;
}

void Grammar_Analyzer::MainFuncDef_Ana(vector<BNode *> *e) {
    if (!NXT_MAIN) {/*throw;*/} //void error
    getElem();
    getElem();
    getElem();

    needReturn = true;
    NodeExtend(e, Block, Block_Ana);

#ifdef Error_Detect
    if (e->back()->chd.size() == 0) {
        if (needReturn) {
            err->add2ErrList(currElem->row, 'g');
        }
    } else {
        BNode *node = e->back()->chd.back()->chd[0];
        Sign_Obj *s = dynamic_cast<Sign_Obj *>(node->chd[0]);
        if (needReturn) {
            if (s == nullptr || s->relation != RETURNTK || node->chd.size() < 2) {
                err->add2ErrList(currElem->row, 'g');
            }
        } else if (s != nullptr && s->relation == RETURNTK && node->chd.size() > 1) {
            err->add2ErrList(currElem->row - 1, 'f');
        }
    }
#endif
}

void Grammar_Analyzer::GAnalyze() {
    while (NXT_CONST || (NXT_INT && preload2.sym == IDENFR && preload3.sym != LPARENT)) {
        auto *node = new BNode(Decl);
        Decl_Ana(&node->chd);
        compUnit->chd.push_back(node);
    }

    while ((NXT_VOID || NXT_INT) && preload2.sym == IDENFR && preload3.sym == LPARENT) {
        getElem();
        auto *node = new BNode(FuncDef);
        FuncDef_Ana(&node->chd);
        print_Tp(node->tp);
        compUnit->chd.push_back(node);
    }
    if (NXT_INT && preload2.sym == MAINTK && preload3.sym == LPARENT) {
        getElem();
        auto *node = new BNode(MainFuncDef);
        MainFuncDef_Ana(&node->chd);
        print_Tp(node->tp);
        compUnit->chd.push_back(node);
    }
}

string &Grammar_Analyzer::result() {
    return out;
}
