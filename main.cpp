#include <stdio.h>
#include <stdlib.h>

#include "structsAndConsts.h"
#include "parserFunctions.h"
#include "syntacticAnalysis.h"
#include "treeFunctions.h"


int main(void) {

    tree_t programTree = {};
    dump dumpInfo = {};
    dumpInfo.nameOfDumpFile = "DUMPS/treeDump.html";
    dumpInfo.nameOfGraphFile = "DUMPS/graph.txt";

    struct lexAnalysisResult* lexResult = lexicalAnalysis("program.txt");

    programTree.rootNode = getProgramTree(&programTree, lexResult->nodesArray, lexResult->numOfNodes);

    if (programTree.rootNode)
        treeDump (&programTree, &dumpInfo, "programTree");

    if (programTree.rootNode)
        deleteTree (&programTree);

    free(lexResult->programBuffer);
    free(lexResult->nodesArray);


    return 0;
}
