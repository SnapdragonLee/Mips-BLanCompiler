//
// Created by SNAPDRAGON_11800H on 2021/9/26.
//

#ifndef E1_LEXICALANALYSIS_TYPE_KEY_H
#define E1_LEXICALANALYSIS_TYPE_KEY_H

#include <bits/stdc++.h>

using namespace std;


enum Keyword {
    UNKNOWN,    // unknown symbol
    END,        // end symbol
    Initial,
    IDENFR,    // Ident
    INTCON,    // IntConst, Const Int Number
    STRCON,    // FormatString
    MAINTK,    // main
    CONSTTK,    // const
    INTTK,    // int
    BREAKTK,    // break
    CONTINUETK,    // continue
    IFTK,    // if
    ELSETK,    // else
    WHILETK,    // while
    GETINTTK,    // getint
    PRINTFTK,    // printf
    RETURNTK,    // return
    VOIDTK,    // void
    NOT,    // !
    AND,    // &&
    OR,    // ||
    PLUS,    // +
    MINU,    // -
    MULT,    // *
    DIV,    // /
    MOD,    // %
    LSS,    // <
    LEQ,    // <=
    GRE,    // >
    GEQ,    // >=
    EQL,    // ==
    NEQ,    // !=
    ASSIGN,    // =
    SEMICN,    // ;
    COMMA,    // ,
    LPARENT,    // (
    RPARENT,    // )
    LBRACK,    // [
    RBRACK,    // ]
    LBRACE,    // {
    RBRACE,    // }

    //TODO: Add more key words here
};

static unordered_map<Keyword, string> repr{
        {VOIDTK, "void"},
        {INTTK,  "int"},
        {NOT,    "!"},
        {AND,    "&&"},
        {OR,     "||"},
        {PLUS,   "+"},
        {MINU,   "-"},
        {MULT,   "*"},
        {DIV,    "/"},
        {MOD,    "%"},
        {LSS,    "<"},
        {LEQ,    "<="},
        {GRE,    ">"},
        {GEQ,    ">="},
        {EQL,    "=="},
        {NEQ,    "!="},
        {ASSIGN, "="},
};

struct Element {
    Keyword sym;
    string val;
    int row, col;

    Element() = default;

    Element(Keyword symbol, string value, int line, int column) {
        sym = symbol;
        val = move(value);
        row = line;
        col = column;
    }

    bool operator==(const Element &t) const {
        return sym == t.sym && val == t.val;
    }

    bool operator!=(const Element &t) const {
        return sym != t.sym || val != t.val;
    }
};

#endif //E1_LEXICALANALYSIS_TYPE_KEY_H
