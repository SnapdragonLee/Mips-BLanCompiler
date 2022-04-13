//
// Created by SNAPDRAGON_11800H on 29/10/2021.
//

#ifndef E5_MIPS_REWORK_TYPEDEFINE_H
#define E5_MIPS_REWORK_TYPEDEFINE_H

#include "../include_Base/Base_settings.h"

class TypeDefine {
public:
    TypeDefine();
    explicit TypeDefine(Element type);
    explicit TypeDefine(Element type, vector<int> dims,
                        bool isConst = false, bool isParam = false, bool isPointer = false);

    bool operator==(const TypeDefine& t);
    bool operator!=(const TypeDefine& t);

    bool getConst() const;
    bool getParam() const;
    bool getPointer() const;
    int getDim() const;
    Element getType() const;
    vector<int> getDims() const;
    string getString() const;

    void addDim(int dim);
    void addPointer();
    void addParam();
private:
    Element type;
    bool isConst, isParam, isPointer;
    vector<int> dims;

    bool checkDim(const TypeDefine& t) const;
};


#endif //E5_MIPS_REWORK_TYPEDEFINE_H
