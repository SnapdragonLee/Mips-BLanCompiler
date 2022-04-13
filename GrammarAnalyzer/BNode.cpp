//
// Created by SNAPDRAGON_11800H on 2021/11/24.
//

#include "BNode.h"

bool is_global = true;

int shell_N = 0;

int tag_num = 0;
int temp_tag;

int label_num = 0;

string *tag_s;
string *tag_label;

string *create_tag() {
    if (is_global) {
        tag_s = new string("%" + to_string(++tag_num));
    } else {
        tag_s = new string("@" + to_string(++tag_num));
    }
    return tag_s;
}

string *create_label() {
    tag_label = new string("label_" + to_string(++label_num));
    return tag_label;
}

void add_decl(const string &l, int tag_n) {
    if (Quaternion::tag_repr.find(l) == Quaternion::tag_repr.end()) {
        vector<sym> vec{sym{tag_n, shell_N}};
        Quaternion::tag_repr.insert(pair<string, vector<sym>>(l, vec));
    } else {
        vector<sym> *vec = &Quaternion::tag_repr.find(l)->second;
        vec->push_back(sym{tag_n, shell_N});
    }
}

void shell_NPlus() {
    shell_N++;
};

void shell_NSub() {
    for (auto & iter : Quaternion::tag_repr) {
        for (auto inner = iter.second.rbegin();
             inner != iter.second.rend(); inner++) {
            if ((*inner).shell_q >= shell_N) {
                iter.second.erase((inner + 1).base());
            }
        }
    }
    shell_N--;
};


string *BNode::getTarget() {
    if (!this->chd.empty()) {
        return this->chd[0]->getTarget();
    }
    return nullptr;
}

BNode::~BNode() {
    for (auto & i : chd) {
        delete i;
    }
}
