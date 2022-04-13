#include "Compiler_sysY.h"

//#define DEBUG

#ifndef DEBUG
//#pragma GCC optimize("Ofast")
#endif

char article[ART_SIZE];
int readCnt;

int main() {
    FILE *fi = fopen("testfile.txt", "rb");
    FILE *fo = fopen("output.txt", "wb+");
    FILE *fr = fopen("error.txt", "wb+");
    FILE *fc = fopen("ir.txt", "wb+");
    FILE *fm = fopen("mips.txt", "wb+");

    readCnt = fread(article, 1, ART_SIZE, fi);

#ifdef DEBUG
    //setbuf(stdout, nullptr);
    printf("Debug Mode: On\n");

#endif

    ErrorList errorList = ErrorList::getErrorE();

    Lexical_Analyzer lexicalAnalyzer = Lexical_Analyzer::getAnalyzeResult(article, readCnt, &errorList);

#ifdef DEBUG
    printf("%s", lexicalAnalyzer.result().c_str());
    //fprintf(fo, "%s", lexicalAnalyzer.result().c_str());

#endif

    Grammar_Analyzer GrammarAnalyzer_old = Grammar_Analyzer::getGrammarResult(&lexicalAnalyzer, &errorList);

    if (errorList.getNum() > 0) {
        printf("There is some thing wrong in error detecting");
        return 0;
    }

    //IR_Old Ir = IR_Old::getIR(GrammarAnalyzer_old.compUnit);

    //MipsGenerate mipsGenerate = MipsGenerate::getMIPS(&Ir);

#ifdef DEBUG
    printf("%s", GrammarAnalyzer_old.result().c_str());
    //fprintf(fo, "%s", GrammarAnalyzer_Rework.result().c_str());

    printf("%s", errorList.getResult().c_str());

    //printf("%s", Ir.getIRResult().c_str());
#endif

    /* Rework GrammarAnalyzer*/
    GrammarAnalyzer_Rework GrammarAnalyzer_new(lexicalAnalyzer.elemList, lexicalAnalyzer.elemNum);

    AstClass ast(GrammarAnalyzer_new.getProgram());

#ifdef DEBUG
    //fprintf(fo, "%s", GrammarAnalyzer_Rework.result().c_str());
    //fprintf(fc, "%s", Ir.getIRResult().c_str()); //
    //printf("%s", mipsGenerate.getMIPSResult().c_str());
    //fprintf(fm, "%s", mipsGenerate.getMIPSResult().c_str()); //
#endif

    Module *module = ast.generateCode();

#ifdef DEBUG
    //fprintf(fc, "%s", module->getResult().c_str());
    //printf("%s", module->getResult().c_str());

#endif
    fprintf(fc, "%s", module->getResult().c_str());


    MipsGenerator generator(module);

#ifdef DEBUG
    //fprintf(fm, "%s", generator.getMIPSResult().c_str());
    //printf("%s", generator.getMIPSResult().c_str());

#endif
    fprintf(fm, "%s", generator.getMIPSResult().c_str());

    fclose(fi);
    fclose(fo);
    fclose(fr);
    fclose(fc);
    fclose(fm);
    return 0;
}
