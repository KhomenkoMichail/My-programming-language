#ifndef BACKEND_TREE_CTOR_H
#define BACKEND_TREE_CTOR_H

#include <stdio.h>
#include "../../COMMON/include/structsAndConsts.h"

#define DUMP_MESSAGE(file, string, bufPos)\
    fprintf(file, string);\
    fprintf(file, "\n");\
    fprintf(file, "bufPos == %s\n", bufPos);\
    fprintf(file, "\n");\

int readFileAndCreateTree (tree_t* tree, dump* dumpInfo, const char* nameOfFile);

node_t* nodeCtorByReadBuffer (char** bufPos, tree_t* tree, node_t* parentNode, dump* dumpInfo, FILE* dumpFile);

nodeType_t getNodeType (char** bufPos);

int processNodeType (tree_t* tree, node_t* node, char** bufPos);

#endif
