#include <stdio.h>
#include <stdlib.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/structAccessFunctions.h"
#include "../../COMMON/include/readTreeFromFileFunc.h"
#include "../../COMMON/include/treeFunctions.h"

#include "../include/reverseFrontendFunctions.h"

int main(int argc, const char* argv[]) {

    if (argc != 3) return printf("Usage: %s source.txt destination.txt\n", argv[0]), 1;

    const char* inputFile = argv[1];
    const char* outputFile = argv[2];

    tree_t programTree = {};
    dump dumpInfo = {};
    dumpInfo.nameOfDumpFile = "DUMPS/reversed-frontendTreeDump.html";
    dumpInfo.nameOfGraphFile = "DUMPS/reFrontGraph.txt";

    char* fileBuffer = readFileAndCreateTree (&programTree, &dumpInfo, inputFile);



    deleteTree(&programTree);
    free(fileBuffer);
    return 0;
}
