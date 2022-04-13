//
// Created by SNAPDRAGON_11800H on 2021/10/5.
//

#ifndef E2_GRAMMAR_ANALYSIS_BNODE_H
#define E2_GRAMMAR_ANALYSIS_BNODE_H

#include "../include_Base/Base_settings.h"
#include "../include_Base/PartType.h"
#include "../IR/Quaternion.h"

extern bool is_global;

extern int shell_N;

extern int tag_num;
extern int temp_tag;

extern int label_num;

extern string *tag_s;
extern string *tag_label;

string *create_tag();

string *create_label();

void shell_NPlus();

void shell_NSub();

void add_decl(const string &l, int tag_n);

struct BNode {
    PartType tp;            // PartType
    vector<BNode *> chd;
// Children
    explicit BNode(PartType p) {
        tp = p;
    }

    virtual string *getTarget();

    virtual ~BNode();
};








#endif //E2_GRAMMAR_ANALYSIS_BNODE_H
