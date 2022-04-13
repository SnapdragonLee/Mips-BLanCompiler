//
// Created by SNAPDRAGON_11800H on 2021/9/26.
//

#include "LexicalAnalyzer.h"

#define IS_VAR (isalpha(currChar) || currChar == '_')
#define IS_DIGIT_ZERO (currChar == '0')
#define IS_DIGIT (isdigit(currChar))
#define IS_BLANK (currChar == ' ' || currChar == '\n' || currChar == '\t' || currChar == '\r')
#define IS_QUOTATION (currChar == '\"')
#define IS_FORMATSTRING (currChar == 32 || currChar == 33 ||(currChar == '%' && art[charNum]== 'd')|| (currChar >= 40 && currChar <= 126))

char elemThis[MAX_KEY_LENGTH];      // current element
Keyword keyThis;
int start;

Lexical_Analyzer::Lexical_Analyzer(char *article, int readCnt, ErrorList *error) {
    art = article;
    err = error;
    initial();
    getChar();
    do {
        LAnalyze();
    } while (charNum != readCnt + 1);
}


Lexical_Analyzer &Lexical_Analyzer::getAnalyzeResult(char *art, int cnt, ErrorList *error) {
    static Lexical_Analyzer real(art, cnt, error);
    return real;
}

void Lexical_Analyzer::initial() {
#define INSERT_KEY(str, type) keyMap.insert(pair<string, Keyword>(str, type))
    INSERT_KEY("main", MAINTK);
    INSERT_KEY("const", CONSTTK);
    INSERT_KEY("int", INTTK);
    INSERT_KEY("break", BREAKTK);
    INSERT_KEY("continue", CONTINUETK);
    INSERT_KEY("if", IFTK);
    INSERT_KEY("else", ELSETK);
    INSERT_KEY("while", WHILETK);
    INSERT_KEY("getint", GETINTTK);
    INSERT_KEY("printf", PRINTFTK);
    INSERT_KEY("return", RETURNTK);
    INSERT_KEY("void", VOIDTK);
#undef INSERT_KEY

    charNum = 0;
    lineNum = 1;
    columnNum = 0;
    elemNum = 0;
}

void Lexical_Analyzer::getChar() {
    currChar = art[charNum];
    charNum++;
    columnNum++;
    if (currChar == '\n') {
        columnNum = 0;
        lineNum++;
    }
}

void Lexical_Analyzer::LAnalyze() {
    start = 0;

    if (IS_VAR) {
        do {
            elemThis[start++] = currChar;
            getChar();
        } while (IS_VAR || IS_DIGIT);
        elemThis[start] = 0;

        auto iter = keyMap.find(elemThis);
        if (iter != keyMap.end()) {
            keyThis = iter->second;
        } else {
            keyThis = IDENFR;
        }
        add2ElemList();
    } else if (IS_DIGIT) {
        do {
            elemThis[start++] = currChar;
            getChar();
        } while (IS_DIGIT);
        elemThis[start] = 0;
        keyThis = INTCON;
        add2ElemList();
    } else if (IS_BLANK) {
        getChar();
        return;
    } else if (IS_QUOTATION) {
        do {
            elemThis[start++] = currChar;
            getChar();
        } while (IS_FORMATSTRING);
        if (currChar == '\"') {
            elemThis[start++] = '\"';
        } else {
#ifdef Error_Detect
            err->add2ErrList(lineNum, 'a');
            while (!IS_QUOTATION) {
                elemThis[start++] = currChar;
                getChar();
            }
            elemThis[start++] = currChar;
#endif
        }
        getChar();
        keyThis = STRCON;
        elemThis[start] = 0;
        add2ElemList();
    } else {
        elemThis[start++] = currChar;
        bool need_getChar = true;

        switch (currChar) {
            case '!':
                getChar();
                if (currChar == '=') {
                    elemThis[start++] = currChar;
                    keyThis = NEQ;
                } else {
                    need_getChar = false;
                    keyThis = NOT;
                }
                break;

            case '&':
                getChar();
                if (currChar == '&') {
                    elemThis[start++] = currChar;
                    keyThis = AND;
                }
                break;

            case '|':
                getChar();
                if (currChar == '|') {
                    elemThis[start++] = currChar;
                    keyThis = OR;
                }
                break;

            case '+':
                keyThis = PLUS;
                break;

            case '-':
                keyThis = MINU;
                break;

            case '*':
                keyThis = MULT;
                break;

            case '/':
                getChar();
                if (currChar == '/') {
                    do {
                        getChar();
                    } while (currChar != '\n');
                    getChar();
                    return;
                } else if (currChar == '*') {
                    do {
                        do {
                            getChar();
                        } while (currChar != '*');
                        while (currChar == '*') {
                            getChar();
                        }
                    } while (currChar != '/');
                    getChar();
                    return;
                } else {
                    need_getChar = false;
                    keyThis = DIV;
                }
                break;

            case '%':
                keyThis = MOD;
                break;

            case '<':
                getChar();
                if (currChar == '=') {
                    elemThis[start++] = currChar;
                    keyThis = LEQ;
                } else {
                    need_getChar = false;
                    keyThis = LSS;
                }
                break;

            case '>':
                getChar();
                if (currChar == '=') {
                    elemThis[start++] = currChar;
                    keyThis = GEQ;
                } else {
                    need_getChar = false;
                    keyThis = GRE;
                }
                break;

            case '=':
                getChar();
                if (currChar == '=') {
                    elemThis[start++] = currChar;
                    keyThis = EQL;
                } else {
                    need_getChar = false;
                    keyThis = ASSIGN;
                }
                break;

            case ';':
                keyThis = SEMICN;
                break;

            case ',':
                keyThis = COMMA;
                break;

            case '(':
                keyThis = LPARENT;
                break;

            case ')':
                keyThis = RPARENT;
                break;

            case '[':
                keyThis = LBRACK;
                break;

            case ']':
                keyThis = RBRACK;
                break;

            case '{':
                keyThis = LBRACE;
                break;

            case '}':
                keyThis = RBRACE;
                break;

            default:
                return;
        }
        elemThis[start] = 0;
        add2ElemList();
        if (need_getChar) {
            getChar();
        }
    }
}

void Lexical_Analyzer::add2ElemList() {
    elemList[elemNum].val = elemThis;
    elemList[elemNum].sym = keyThis;
    elemList[elemNum].row = lineNum;
    elemList[elemNum].col = columnNum;
    elemNum++;
}

string &Lexical_Analyzer::result() {
    for (int i = 0; i < elemNum; i++) {

#ifdef Compose_Lexical
        printElemType(elemList[i].sym);
        out.append(" " + elemList[i].val + "\n");
#endif

    }
    out += '\0';
    return out;
}
