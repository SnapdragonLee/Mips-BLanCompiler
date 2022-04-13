cmake_minimum_required(VERSION 3.20)
project(BLanCompiler)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Compiler_sysY.h
        include_Base/Type_Key.h include_Base/Base_settings.h include_Base/PartType.h
        LexicalAnalyzer/LexicalAnalyzer.cpp LexicalAnalyzer/LexicalAnalyzer.h
        GrammarAnalyzer/GrammarAnalyzer.cpp GrammarAnalyzer/GrammarAnalyzer.h GrammarAnalyzer/BNode.h

        ErrorProcess/ErrorList.cpp ErrorProcess/ErrorList.h
        IR/IR_Old.cpp IR/IR_Old.h IR/Quaternion.h
        IR/Quaternion.cpp GrammarAnalyzer/BNode.cpp MIPS/MipsGenerate.cpp MIPS/MipsGenerate.h IR/BlockEntry.cpp
        IR/BlockEntry.h
        IR/Ctx.cpp
        IR/Ctx.h
        IR/Instr.cpp
        IR/Instr.h
        IR/LinkedList_IR.h
        IR/IrFunc.cpp
        IR/IrFunc.h
        IR/Module.cpp
        IR/Module.h
        IR/VarClass.cpp
        IR/VarClass.h
        MIPS/MipsGenerator.cpp
        MIPS/MipsGenerator.h
        GrammarAnalyzer/AstClass.cpp
        GrammarAnalyzer/AstClass.h
        GrammarAnalyzer/GrammarAnalyzer_Rework.cpp
        GrammarAnalyzer/GrammarAnalyzer_Rework.h
        GrammarAnalyzer/TableList.cpp
        GrammarAnalyzer/TableList.h
        GrammarAnalyzer/TypeDefine.cpp
        GrammarAnalyzer/TypeDefine.h )

aux_source_directory(./GrammarAnalyzer/Object_Def Obj_Main)

add_executable(BLanCompiler ${SOURCE_FILES} ${Obj_Main})
