//
// Created by SNAPDRAGON_11800H on 12/17/2021.
//

#include "ConstIdent_Obj.h"

string *ConstIdent_Obj::getTarget() {
    if (Quaternion::tag_repr.find(*name) != Quaternion::tag_repr.end()) {
        vector<sym> *vec = &Quaternion::tag_repr.find(*name)->second;
        for (int i = vec->size() - 1; i > -1; i--) {
            if ((*vec)[i].shell_q == shell_N) {
                if (is_global) {
                    return new string("%" + to_string((*vec)[i].tag));
                } else {
                    return new string("@" + to_string((*vec)[i].tag));
                }
            }
        }
    }
    string *n = this->init->getTarget();
    string *temp = create_tag();
    add_decl(*name, tag_num);
    ConstVarDefine *ex = new ConstVarDefine(temp, n);
    Quaternion::IC[Quaternion::index].push_back(ex);
    //printf("%s = %s\n", ex->l->c_str(), ex->n->c_str());
    return temp;
}

ConstIdent_Obj::ConstIdent_Obj(string *t, PartType p) : BNode(p) {
    name = t;
}
