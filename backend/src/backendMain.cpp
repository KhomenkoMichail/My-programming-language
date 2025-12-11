#include <stdio.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/treeFunctions.h"

#include "../include/backendTreeCtor.h"


int main(int argc, const char* argv[]) {

    if (argc != 3) return printf("Usage: %s source.txt destination.asm\n", argv[0]), 1;

    const char* inputFile = argv[1];
    const char* outputFile = argv[2];

    tree_t programTree = {};
    dump dumpInfo = {};
    dumpInfo.nameOfDumpFile = "backend/DUMPS/treeDump.html";
    dumpInfo.nameOfGraphFile = "backend/DUMPS/graph.txt";

    readFileAndCreateTree (&programTree, &dumpInfo, inputFile);
    treeDump(&programTree, &dumpInfo, "program");

    return 0;
}



