//
// Created by SNAPDRAGON_11800H on 12/17/2021.
//

#include "FuncIdent_Obj.h"
#include "VarIdent_Obj.h"

FuncIdent_Obj::FuncIdent_Obj(Keyword k, string *t, PartType p) : BNode(p) {
    returnType = k;
    name = t;
}

FuncIdent_Obj::FuncIdent_Obj(string *t, PartType p) : BNode(p) {
    returnType = Initial;
    name = t;
}

string *FuncIdent_Obj::getTarget() {
    if (Quaternion::tag_repr.find(*name) == Quaternion::tag_repr.end()) {
        FuncNameDefine *ex = new FuncNameDefine(&repr.find(returnType)->second, name, create_label());
        Quaternion::IC[Quaternion::index].push_back(ex);
        add_decl(*name, 0);
        temp_tag = tag_num;
        tag_num = 0;
        shell_NPlus();
        FuncParamDefine *ep = nullptr;
        for (int i = 0; i < funcParam.size(); i++) {
            string *temp = create_tag();
            add_decl(*((VarIdent_Obj *) funcParam[i]->chd[0])->name, tag_num);
            ep = new FuncParamDefine(temp);
            Quaternion::IC[Quaternion::index].push_back(ep);
        }
        return nullptr;
    } else {
        for (int i = 0; i < funcParam.size(); i++) {
            FuncParamCall *ex = new FuncParamCall(funcParam[i]->getTarget());
            Quaternion::IC[Quaternion::index].push_back(ex);
        }
        FuncNameCall *ep = new FuncNameCall(name);
        Quaternion::IC[Quaternion::index].push_back(ep);
        if (returnType == INTTK) {// todo:  test
            return new string("RET");
        } else {
            return nullptr;
        }
    }
}
