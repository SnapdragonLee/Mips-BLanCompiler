//
// Created by SNAPDRAGON_11800H on 19/11/2021.
//

#ifndef E5_MIPS_REWORK_MIPSGENERATOR_H
#define E5_MIPS_REWORK_MIPSGENERATOR_H

#include "../include_Base/Base_settings.h"
#include "../IR/Module.h"

class MipsGenerator {
public:
    MipsGenerator();

    explicit MipsGenerator(Module *module);

    string &getMIPSResult();

private:
    Module *module;

    int str_id;
    map<string, int> strMap;
    vector<string> mips;

    string output;


    static string prodLabelTag(int id);

    static string prodArrayTag(IrArray *arr);


    void prodDataSegment();

    void prodTextSegment();

    string prodAsciizTag(const string &s);

    void assign(VarClass *var, int reg);

    void prodInst(Inst *inst);

    void prodBinaryInst(BinaryInst *inst);

    void prodAssignInst(AssignInst *inst);

    void prodReturnInst(ReturnInst *inst);

    void prodLoadAddrInst(LoadAddrInst *inst);

    void prodLoadInst(LoadInst *inst);

    void prodStoreInst(StoreInst *inst);

    void prodGetReturnInst(GetReturnInst *inst);

    void prodBranchInst(BranchInst *inst);

    void prodJumpInst(JumpInst *inst);


    void prodCallInst(CallInst *inst);

    void prodDeclInst(DeclInst *inst);

    void prodNotInst(NotInst *inst);

    void loadVar(VarClass *var, int reg);

    void loadWord(int addr, int reg);

    void storeWord(int addr, int reg);

    static string prodWordTag(int id, VarClass *init);

    void prodFuncDef(IrFunc *func);


    void loadConst(Constant *var, int reg);

    void loadAddr(VarClass *base, int reg);

    void prodBlock(BlockEntry *block);
};


#endif //E5_MIPS_REWORK_MIPSGENERATOR_H
