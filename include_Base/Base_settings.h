//
// Created by SNAPDRAGON_11800H on 2021/9/26.
//


#ifndef E1_LEXICALANALYSIS_BASE_SETTINGS_H
#define E1_LEXICALANALYSIS_BASE_SETTINGS_H

#include <bits/stdc++.h>
#include "Type_Key.h"

//#define Compose_Lexical
//#define Lexical
//#define Compose_Grammar
//#define Grammar
//#define Error_Detect
//#define IR
#define MIPSGenerate


#define ART_SIZE (64 * 1024)
#define MAX_KEY_NUM (16 * 1024)
#define MAX_KEY_LENGTH 1024


#define _STR(s) #s

#define Case(type) case type: \
            out += _STR(type); \
            break

#endif //E1_LEXICALANALYSIS_BASE_SETTINGS_H
