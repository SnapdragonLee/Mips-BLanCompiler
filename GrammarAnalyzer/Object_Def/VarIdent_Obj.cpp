//
// Created by SNAPDRAGON_11800H on 12/17/2021.
//

#include "VarIdent_Obj.h"

string *VarIdent_Obj::getTarget() {
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

    string *n = nullptr;
    if (init != nullptr) {
        n = init->getTarget();
    }
    string *temp = create_tag();
    add_decl(*name, tag_num);
    VarDefine *ex = new VarDefine(temp, n);

    Quaternion::IC[Quaternion::index].push_back(ex);
    //printf("%s = %s\n", ex->l->c_str(), ex->n->c_str());
    return temp;
}

VarIdent_Obj::VarIdent_Obj(string *t, PartType p, bool define) : BNode(p) {
    name = t;
    is_define = define;
}
