#include <stdio.h>
#include <stdlib.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/treeFunctions.h"
#include "../../COMMON/include/readTreeFromFileFunc.h"
#include "../../COMMON/include/saveTreeInFile.h"

#include "../include/middleendOptimization.h"

int main(int argc, const char* argv[]) {

    if (argc != 2) return printf("Usage: %s source.txt \n", argv[0]), 1;

    const char* astFile = argv[1];

    tree_t programTree = {};
    dump dumpInfo = {};

    dumpInfo.nameOfDumpFile = "DUMPS/middleEndTreeDump.html";
    dumpInfo.nameOfGraphFile = "DUMPS/middleGraph.txt";

    char* fileCopyBuffer = readFileAndCreateTree(&programTree, &dumpInfo, astFile);

    simplifyTree(&programTree, &dumpInfo);

    saveTreeInFile (&programTree, astFile);

    deleteTree(&programTree);
    free(fileCopyBuffer);
}

