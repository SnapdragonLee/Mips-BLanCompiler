//
// Created by SNAPDRAGON_11800H on 12/17/2021.
//

#include "MulExp_Obj.h"

MulExp_Obj::MulExp_Obj(PartType p) : BNode(p) {

}

void MulExp_Obj::addsrc(BNode *s1, BNode *o, BNode *s2) {
    src1 = s1;
    op = dynamic_cast<Operation_Obj *>(o);
    src2 = s2;
}
