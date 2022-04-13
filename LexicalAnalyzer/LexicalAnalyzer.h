//
// Created by SNAPDRAGON_11800H on 2021/9/26.
//

#ifndef E1_LEXICALANALYSIS_LEXICALANALYZER_H
#define E1_LEXICALANALYSIS_LEXICALANALYZER_H

#include "../include_Base/Base_settings.h"
#include "../ErrorProcess/ErrorList.h"

#ifdef Lexical
#define printElemType(i) do { \
                             switch (i) { \
                                 Case (UNKNOWN); \
                                 Case (IDENFR); \
                                 Case (INTCON); \
                                 Case (STRCON); \
                                 Case (MAINTK); \
                                 Case (CONSTTK); \
                                 Case (INTTK); \
                                 Case (BREAKTK); \
                                 Case (CONTINUETK); \
                                 Case (IFTK); \
                                 Case (ELSETK); \
                                 Case (WHILETK); \
                                 Case (GETINTTK); \
                                 Case (PRINTFTK); \
                                 Case (RETURNTK); \
                                 Case (VOIDTK); \
                                 Case (NOT); \
                                 Case (AND); \
                                 Case (OR); \
                                 Case (PLUS); \
                                 Case (MINU); \
                                 Case (MULT); \
                                 Case (DIV); \
                                 Case (MOD); \
                                 Case (LSS); \
                                 Case (LEQ); \
                                 Case (GRE); \
                                 Case (GEQ); \
                                 Case (EQL); \
                                 Case (NEQ); \
                                 Case (ASSIGN); \
                                 Case (SEMICN); \
                                 Case (COMMA); \
                                 Case (LPARENT); \
                                 Case (RPARENT); \
                                 Case (LBRACK); \
                                 Case (RBRACK); \
                                 Case (LBRACE); \
                                 Case (RBRACE); \
                                 default: \
                                    break; \
                             } \
                         } while (0)
#else
#define printElemType(i) do{}while(0)
#endif

class Lexical_Analyzer {
    /* private variety defined here */

    string out;
    int lineNum;
    int columnNum;
    char currChar;

    char *art = nullptr;
    int charNum;                        // iterator for art


    /* private function defined here */
    explicit Lexical_Analyzer(char *k, int cnt, ErrorList *error);

    void initial();

    void getChar();

    void LAnalyze();

    void add2ElemList();


public:
    int elemNum;                        // iterator for elemList

    ErrorList *err = nullptr;

    Element elemList[MAX_KEY_NUM];      // element list

    map<string, Keyword> keyMap;        // keymap for keyword

    static Lexical_Analyzer &getAnalyzeResult(char *k, int cnt, ErrorList *error);

    string &result();
};


#endif //E1_LEXICALANALYSIS_LEXICALANALYZER_H
