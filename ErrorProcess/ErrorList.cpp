//
// Created by SNAPDRAGON_11800H on 2021/10/22.
//

#include "ErrorList.h"

ErrorList &ErrorList::getErrorE() {
    static ErrorList err;
    return err;
}

void ErrorList::add2ErrList(int line, char type) {
    ErrorSym *tempE = new ErrorSym{type, line};
    error.push_back(tempE);
    err_num++;
}

string &ErrorList::getResult() {

#ifdef Error_Detect
    sort(error.begin(), error.end(), Cmp);
    for (int i = 0; i < err_num; i++) {
        err_str += to_string(error[i]->eLine) + ' ' + error[i]->eType + '\n';
    }
#endif
    return err_str;
}

bool ErrorList::Cmp(ErrorSym *a, ErrorSym *b) {
    return a->eLine < b->eLine || (a->eLine == b->eLine && a->eType < b->eType);
}

int ErrorList::getNum() {
    return error.size();
}
