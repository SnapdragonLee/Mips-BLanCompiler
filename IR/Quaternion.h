//
// Created by SNAPDRAGON_11800H on 2021/11/23.
//

#ifndef E4_INTERMEDIATE_REPRESENTATION_Intermediate_H
#define E4_INTERMEDIATE_REPRESENTATION_Intermediate_H

#include"../include_Base/Base_settings.h"

struct sym {
    int tag;
    int shell_q;
};
typedef struct sym sym;

struct Intermediate {
    virtual void out(string *o) {};

    virtual ~Intermediate() {};
};

struct ExpCalc : Intermediate {
    string *l;
    string *n1;
    string *op;
    string *n2;

    ExpCalc(string *l_, string *n1_, string *op_, string *n2_) {
        l = l_;
        n1 = n1_;
        op = op_;
        n2 = n2_;
    }

    void out(string *o) override {
        *o += *l + " = " + *n1 + " " + *op + " " + *n2 + "\n";
        //printf("%s = %s %s %s", l->c_str(), n1->c_str(), op->c_str(), n2->c_str());
    }

    ~ExpCalc() override = default;
};

struct ConstVarDefine : Intermediate {
    string *l;
    string *n;

    ConstVarDefine(string *l_, string *n_) {
        l = l_;
        n = n_;
    }

    void out(string *o) override {
        *o += "CI " + *l + " = " + *n + "\n";
    }


    ~ConstVarDefine() override = default;
};

struct VarDefine : Intermediate {
    string *l;
    string *n;

    VarDefine(string *l_, string *n_) {
        l = l_;
        n = n_;
    }

    void out(string *o) override {
        if (n != nullptr) {
            *o += "I " + *l + " = " + *n + "\n";
        } else {
            *o += "I " + *l + "\n";
        }
    }

    ~VarDefine() override = default;
};

struct VarInUse : Intermediate {
    string *l;
    string *n;

    VarInUse(string *l_, string *n_) {
        l = l_;
        n = n_;
    }

    void out(string *o) override {
        *o += *l + " = " + *n + "\n";
    }

    ~VarInUse() override = default;
};

struct FuncNameDefine : Intermediate {
    string *type;
    string *l;
    string *label;

    FuncNameDefine(string *t, string *l_, string *lab) {
        type = t;
        l = l_;
        label = lab;
    }

    void out(string *o) override {
        *o += *type + " " + *l + "()\n" + *label + ": \n";
    }

    ~FuncNameDefine() override = default;
};

struct FuncParamDefine : Intermediate {
    string *l;

    FuncParamDefine(string *l_) {
        l = l_;
    }

    void out(string *o) override {
        *o += "param " + *l + "\n";
    }

    ~FuncParamDefine() override = default;
};

struct FuncReturn : Intermediate {
    string *l;

    FuncReturn(string *l_) {
        l = l_;
    }

    void out(string *o) override {
        *o += "RET " + *l + "\n";
    }

    ~FuncReturn() override = default;
};

struct FuncNameCall : Intermediate {
    string *l;

    FuncNameCall(string *l_) {
        l = l_;
    }

    void out(string *o) override {
        *o += "CALL " + *l + "()\n";
    }

    ~FuncNameCall() override = default;
};

struct FuncParamCall : Intermediate {
    string *l;

    FuncParamCall(string *l_) {
        l = l_;
    }

    void out(string *o) override {
        *o += "push " + *l + "\n";
    }

    ~FuncParamCall() override = default;
};

typedef struct Intermediate Intermediate;

class Quaternion {

public:

    static int index;
    static vector<string> str_G;
    static vector<vector<Intermediate *>> IC;
    static unordered_map<string, vector<sym>> tag_repr;
};
#endif //E4_INTERMEDIATE_REPRESENTATION_Intermediate_H
