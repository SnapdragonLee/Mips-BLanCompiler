//
// Created by SNAPDRAGON_11800H on 12/20/2021.
//

#ifndef E5_MIPS_REWORK_PARTTYPE_H
#define E5_MIPS_REWORK_PARTTYPE_H


enum PartType {
    DEFAULT,  //*
    Ident,  //*
    CompUnit,
    Decl,  //*
    ConstDecl,
    ConstDef,
    ConstInitVal,
    VarDecl,
    VarDef,
    InitVal,
    FuncDef,
    FuncType,
    FuncFParams,
    FuncFParam,
    MainFuncDef,
    Block,
    BlockItem,  //*
    Stmt,
    Exp,
    Cond,
    LVal,
    PrimaryExp,
    Number,
    UnaryExp,
    UnaryOp,
    FuncRParams,
    MulExp,
    AddExp,
    RelExp,
    EqExp,
    LAndExp,
    LOrExp,
    ConstExp,
    FormatString,
};

#endif //E5_MIPS_REWORK_PARTTYPE_H
