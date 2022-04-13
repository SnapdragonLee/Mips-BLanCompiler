//
// Created by SNAPDRAGON_11800H on 2021/11/22.
//

#ifndef E4_INTERMEDIATE_REPRESENTATION_IR_H
#define E4_INTERMEDIATE_REPRESENTATION_IR_H

#include "../include_Base/Base_settings.h"
#include "../GrammarAnalyzer/GrammarAnalyzer.h"
#include "Quaternion.h"

extern Quaternion quaternion;

class IR_Old {
    string outIR;
    void IRGenarate();

    explicit IR_Old(BNode *b);

    void Decl_B(BNode *b);

    void ConstDecl_B(BNode *b);

    void VarDecl_B(BNode *b);

    void Func_B(BNode *b);

    void Block_B(BNode *b);

    void BlockItem_B(BNode *b);

    void Stmt_B(BNode *b);

    void MainFunc_B(BNode *b);

public:
    Quaternion quaternion;
    BNode *compUnit = nullptr;

    string &getIRResult();

    static IR_Old &getIR(BNode *b);
};


#endif //E4_INTERMEDIATE_REPRESENTATION_IR_H
