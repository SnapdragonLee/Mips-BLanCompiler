//
// Created by SNAPDRAGON_11800H on 2021/10/22.
//

#ifndef E3_ERROR_PROCESS_ERRORLIST_H
#define E3_ERROR_PROCESS_ERRORLIST_H

#include "../include_Base/Base_settings.h"

struct errorSym {
    char eType;             // error kind
    int eLine;              // which line caused error
};

typedef struct errorSym ErrorSym;

class ErrorList {
private:
    int err_num;
    string err_str;

    vector<ErrorSym *> error;

    static bool Cmp(ErrorSym *a, ErrorSym *b);

public:
    void add2ErrList(int line, char type);

    static ErrorList &getErrorE();

    string &getResult();

    int getNum();
};


#endif //E3_ERROR_PROCESS_ERRORLIST_H
