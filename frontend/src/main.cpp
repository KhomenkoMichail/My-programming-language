#include <stdio.h>
#include <stdlib.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/treeFunctions.h"

#include "../include/lexicalAnalysis.h"
#include "../include/syntacticAnalysis.h"
#include "../include/saveTreeInFile.h"


int main(int argc, const char* argv[]) {

    if (argc != 3) return printf("Usage: %s source.txt destination.txt\n", argv[0]), 1;

    const char* inputFile = argv[1];
    const char* outputFile = argv[2];

    dump dumpInfo = {};
    dumpInfo.nameOfDumpFile = "frontend/DUMPS/treeDump.html";
    dumpInfo.nameOfGraphFile = "frontend/DUMPS/graph.txt";

    struct lexAnalysisResult* lexResult = lexicalAnalysis(inputFile);
    if  (!lexResult) return printf("Error lexical Analysis.\n");

    tree_t programTree = {};
    programTree.rootNode = getProgramTree(&programTree, lexResult);

    if (!programTree.rootNode) {
        free(lexResult->programBuffer);
        free(lexResult->nodesArray);
        return printf("Error syntactic Analysis.\n");
    }

    treeDump (&programTree, &dumpInfo, "programTree");
    saveTreeInFile(&programTree, outputFile);

    endFrontendProgram (&programTree, lexResult);
    return 0;
}
