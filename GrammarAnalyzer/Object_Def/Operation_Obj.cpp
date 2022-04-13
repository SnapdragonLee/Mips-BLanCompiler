//
// Created by SNAPDRAGON_11800H on 12/17/2021.
//

#include "Operation_Obj.h"

Operation_Obj::Operation_Obj(Keyword k, PartType e) : BNode(e) {
    relation = k;
}

string *Operation_Obj::getTarget() {
    if (this->chd.size() == 2) {
        string *n1 = this->chd[0]->getTarget();
        string *op = &repr.find(this->relation)->second;
        string *n2 = this->chd[1]->getTarget();
        ExpCalc *ex = new ExpCalc(create_tag(), n1, op, n2);
        Quaternion::IC[Quaternion::index].push_back(ex);
        return tag_s;
    }
    return this->chd[0]->getTarget();
}
