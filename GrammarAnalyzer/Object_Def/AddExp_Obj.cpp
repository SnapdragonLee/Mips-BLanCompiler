//
// Created by SNAPDRAGON_11800H on 12/17/2021.
//

#include "AddExp_Obj.h"

AddExp_Obj::AddExp_Obj(PartType p) : BNode(p) {

}

void AddExp_Obj::addsrc(BNode *s1, BNode *o, BNode *s2) {
    src1 = s1;
    op = dynamic_cast<Operation_Obj *>(o);
    src2 = s2;
}

/*int AddExp_Obj::getConst() {
    if (op == nullptr) {
        return src1->getConst();
    } else {
        return (op->relation == PLUS) ? (src1->getConst() + src2->getConst()) : (src1->getConst() - src2->getConst());
    }
}*/
