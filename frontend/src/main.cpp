#include <stdio.h>
#include <stdlib.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/treeFunctions.h"

#include "../include/lexicalAnalysis.h"
#include "../include/syntacticAnalysis.h"
#include "../include/saveTreeInFile.h"


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
