#include <stdio.h>
#include <stdlib.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/treeFunctions.h"
#include "../../COMMON/include/readTreeFromFileFunc.h"

#include "../include/sourceFileParser.h"
#include "../include/asmProgramWriter.h"


int main(int argc, const char* argv[]) {

    if (argc != 4) return printf("Usage: %s source.txt destination.asm programSource.txt\n", argv[0]), 1;

    const char* inputFile = argv[1];
    const char* outputFile = argv[2];
    const char* sourceFileName = argv[3];

    tree_t programTree = {};
    dump dumpInfo = {};
    dumpInfo.nameOfDumpFile = "DUMPS/backendTreeDump.html";
    dumpInfo.nameOfGraphFile = "DUMPS/backGraph.txt";

    char* fileBuffer = readFileAndCreateTree (&programTree, &dumpInfo, inputFile);

    rewriteAstToAsmCode(&programTree, outputFile, sourceFileName);

    deleteTree(&programTree);
    free(fileBuffer);
    return 0;
}



