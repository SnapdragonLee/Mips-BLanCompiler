//
// Created by SNAPDRAGON_11800H on 2021/11/22.
//

#include "IR_Old.h"

IR_Old::IR_Old(BNode *b) {
    compUnit = b;
    quaternion.IC = *&Quaternion::IC;
    IRGenarate();
}

IR_Old &IR_Old::getIR(BNode *b) {
    static IR_Old init(b);
    return init;
}

void IR_Old::IRGenarate() {
    int i = 0;
    quaternion.IC.emplace_back();
    if (compUnit->chd[i]->tp == Decl) {
        i++;
        //Decl_B(compUnit->chd[i++]);
    }
    is_global = false;
    quaternion.IC.emplace_back();
    Quaternion::index++;
    while (compUnit->chd[i]->tp == FuncDef) {
        //Func_B(compUnit->chd[i++]);
        i++;
        quaternion.IC.emplace_back();
        Quaternion::index++;
    }
    MainFunc_B(compUnit->chd[i]);
}

string &IR_Old::getIRResult() {
    for (int i = 0; i < quaternion.IC[0].size(); i++) {
        quaternion.IC[0][i]->out(&outIR);
    }
    for (int i = quaternion.IC.size() - 1; i > 0; i--) {
        for (int j = 0; j < quaternion.IC[i].size(); j++) {
            quaternion.IC[i][j]->out(&outIR);
        }
    }
    return outIR;
}

void IR_Old::Decl_B(BNode *b) {
    for (int i = 0; i < b->chd.size(); i++) {
        if (b->chd[i]->tp == ConstDecl) {
            ConstDecl_B(b->chd[i]);
        } else {
            VarDecl_B(b->chd[i]);
        }
        //b->chd[i]->getTarget();
    }
}

void IR_Old::Func_B(BNode *b) {

    //printf("shell_N = %d\n", shell_N);
    b->chd[0]->getTarget();
    Block_B(b->chd.back());
    tag_num = 0;
    shell_NSub();
}

void IR_Old::Block_B(BNode *b) {
    for (int i = 0; i < b->chd.size(); i++) {
        BlockItem_B(b->chd[i]->chd[0]);
    }
}

void IR_Old::BlockItem_B(BNode *b) {
    if (b->tp == Decl) {
        Decl_B(b);
    } else if (b->tp == Stmt) {
        Stmt_B(b);
    }
}

void IR_Old::Stmt_B(BNode *b) {
    for (int i = 0; i < b->chd.size(); i++) {
        if (b->chd[i]->tp == LVal) {
            if (b->chd[i + 1]->tp == Exp) {
                string *n1 = b->chd[i + 1]->getTarget();
                vector<sym> *vec = &Quaternion::tag_repr.find(*((VarIdent_Obj *) b->chd[i]->chd[0])->name)->second;
                string *l;
                for (int i = vec->size() - 1; i > -1; i--) {
                    if ((*vec)[i].shell_q == 0) {
                        l = new string("%" + to_string((*vec)[i].tag));
                        break;
                    } else if ((*vec)[i].shell_q <= shell_N) {
                        l = new string("@" + to_string((*vec)[i].tag));
                        break;
                    }
                }
                VarInUse *ex = new VarInUse(l, n1);
                Quaternion::IC[Quaternion::index].push_back(ex);
            } else if ((dynamic_cast<Sign_Obj *>(b->chd[i + 1])) != nullptr) {
                if (dynamic_cast<Sign_Obj *>(b->chd[i + 1])->relation == GETINTTK) {
                    FuncNameCall *ep = new FuncNameCall(new string("getint"));
                    Quaternion::IC[Quaternion::index].push_back(ep);
                    string *n1 = new string("RET");
                    vector<sym> *vec = &Quaternion::tag_repr.find(*((VarIdent_Obj *) b->chd[i]->chd[0])->name)->second;
                    string *l;
                    for (int i = vec->size() - 1; i > -1; i--) {
                        if ((*vec)[i].shell_q == 0) {
                            l = new string("%" + to_string((*vec)[i].tag));
                            break;
                        } else if ((*vec)[i].shell_q <= shell_N) {
                            l = new string("@" + to_string((*vec)[i].tag));
                            break;
                        }
                    }
                    VarInUse *ex = new VarInUse(l, n1);
                    Quaternion::IC[Quaternion::index].push_back(ex);
                }
            }
            i++;
        } else if (b->chd[i]->tp == Exp) {
            string *n1 = b->chd[i]->getTarget();
            string *l = create_tag();
            VarInUse *ex = new VarInUse(l, n1);
            Quaternion::IC[Quaternion::index].push_back(ex);
            //VarDecl_B(b->chd[i]);
        } else if (((Sign_Obj *) (b->chd[i]))->relation == RETURNTK) {
            string *n1 = b->chd[i + 1]->getTarget();
            FuncReturn *ex = new FuncReturn(n1);
            Quaternion::IC[Quaternion::index].push_back(ex);
            i++;
        } else if ((dynamic_cast<Print_Obj *>( b->chd[i]))->relation == PRINTFTK) {
            string *p = dynamic_cast<Print_Obj *>( b->chd[i])->s;
            string *t = new string();
            for (int i = 1, j = 0; i < p->length() - 1; i++) {
                if ((*p)[i] == '%' && (*p)[i + 1] == 'd') {
                    FuncParamCall *ex;
                    FuncNameCall *ep;

                    if ((*t).size() != 0) {
                        *t = "\"" + *t + "\"";
                        Quaternion::str_G.push_back(*t);
                        ex = new FuncParamCall(t);
                        Quaternion::IC[Quaternion::index].push_back(ex);
                        ep = new FuncNameCall(new string("printf"));
                        Quaternion::IC[Quaternion::index].push_back(ep);
                    } else {
                        string *n1 = (dynamic_cast<Print_Obj *>(b->chd[0]))->EXT[j++]->getTarget();
                        ex = new FuncParamCall(n1);
                        Quaternion::IC[Quaternion::index].push_back(ex);
                        ep = new FuncNameCall(new string("printf"));
                        Quaternion::IC[Quaternion::index].push_back(ep);
                    }


                    i++;
                    t = new string();
                } else {
                    *t += (*p)[i];
                }
            }
            if (!(*t).empty()) {
                *t = "\"" + *t + "\"";
                Quaternion::str_G.push_back(*t);
                FuncParamCall *ex = new FuncParamCall(t);
                Quaternion::IC[Quaternion::index].push_back(ex);
                FuncNameCall *ep = new FuncNameCall(new string("printf"));
                Quaternion::IC[Quaternion::index].push_back(ep);
            }
            i += dynamic_cast<Print_Obj *>(b->chd[i])->dT;
            //VarDecl_B(b->chd[i]);
        } else if (b->chd[i]->tp == Block) {
            shell_NPlus();
            BlockItem_B(b->chd[i]);
            shell_NSub();
        }
    }
}

void IR_Old::MainFunc_B(BNode *b) {
    FuncNameDefine *ex = new FuncNameDefine(&repr.find(INTTK)->second, new string("main"), create_label());
    Quaternion::IC[Quaternion::index].push_back(ex);

    //b->chd[0]->getTarget();
    //Block_B(b->chd.back());
    tag_num = temp_tag;
    //shell_NSub();
}

void IR_Old::ConstDecl_B(BNode *b) {
    for (int i = 0; i < b->chd.size(); i++) {
        b->chd[i]->getTarget();
    }
}

void IR_Old::VarDecl_B(BNode *b) {
    for (int i = 0; i < b->chd.size(); i++) {
        b->chd[i]->getTarget();
    }
}
