#include <TXLib.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#pragma GCC diagnostic ignored "-Wredundant-tags"

#include "../include/structsAndConsts.h"
#include "../include/treeFunctions.h"
#include "../include/structAccessFunctions.h"

int fprintfTreeGraphDump (tree_t* tree, const char* textGraphFileName) {
    assert(tree);
    assert(textGraphFileName);

    FILE* graphFile = fopen(textGraphFileName, "w");

    if (graphFile == NULL) {
        fprintf(stderr, "Error of opening file \"%s\"", textGraphFileName);
        perror("");
        return 1;
    }

    fprintf(graphFile, "digraph Tree {\n");
    fprintf(graphFile, "    rankdir = TB;\n");
    fprintf(graphFile, "    node [shape = Mrecord, color = black];\n");

    size_t graphNodesMade = 0;
    fprintfNodeGraph(*treeRoot(tree), 0, graphFile, &graphNodesMade, *treeSize(tree));
    size_t graphLinksMade = 0;
    fprintfNodeLinksForGraph(*treeRoot(tree), graphFile, &graphLinksMade, *treeSize(tree));

    fprintf(graphFile, " }\n");
    if (fclose(graphFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", textGraphFileName);
        perror("");
        return 1;
    }

    return 0;
}

int fprintfNodeGraph (node_t* node, int rank, FILE* graphFile, size_t* nodesPassed, size_t treeSize) {
    assert(node);
    assert(graphFile);

    //(*nodesPassed) += 1;
    //if (*nodesPassed > treeSize)
    //    return tooManyRecursiveCalls;

    char leftPtr[STR_SIZE] = "NULL";
    if (*(nodeLeft(node)) != NULL)
        snprintf(leftPtr, sizeof(leftPtr), "0x%p", *(nodeLeft(node)));

    char rightPtr[STR_SIZE] = "NULL";
    if (*(nodeRight(node)) != NULL)
        snprintf(rightPtr, sizeof(rightPtr), "0x%p", *(nodeRight(node)));

    const char* typeName = 0;
    switch (*nodeType(node)) {
        case typeOperator:
            typeName = "OP";
            break;
        case typeIdentifier:
            typeName = "ID";
            break;
        case typeNumber:
            typeName = "NUM";
            break;
        case typeError:
        default:
            typeName = "ERROR";
            break;
    }

    if(*nodeType(node) == typeOperator) {
        #include "../include/operatorsArray.h"

        size_t numOfOp = 0;
        for (numOfOp = 0; numOfOp < NUM_OF_OPERATORS; numOfOp++)
            if ((operatorsArray[numOfOp]).opCode == (nodeValue(node))->opCode)
                break;

        if (nodeValue(node)->opCode == opBELOW || nodeValue(node)->opCode == opABOVE ||
            nodeValue(node)->opCode == opE_BELOW || nodeValue(node)->opCode == opE_ABOVE)

            fprintf(graphFile, "    node0x%p [rank = %d, label = \"{ <addr>type: %s| val = \\%s|{<left>LEFT| <right>RIGHT}}\", style = filled, fillcolor = \"%s\", color = black];\n",
                node, rank, typeName, (operatorsArray[numOfOp]).opCLangName, (operatorsArray[numOfOp]).dumpColor);
        else
            fprintf(graphFile, "    node0x%p [rank = %d, label = \"{ <addr>type: %s| val = %s|{<left>LEFT| <right>RIGHT}}\", style = filled, fillcolor = \"%s\", color = black];\n",
                    node, rank, typeName, (operatorsArray[numOfOp]).opCLangName, (operatorsArray[numOfOp]).dumpColor);
    }

    else if (*nodeType(node) == typeIdentifier)
        fprintf(graphFile, "    node0x%p [rank = %d, label = \"{ <addr>type: %s| val = %s|{<left>LEFT| <right>RIGHT}}\", style = filled, fillcolor = \"#f3d2f0ff\", color = black];\n",
                node, rank, typeName, ((nodeValue(node))->id).identifierName);

    else
        fprintf(graphFile, "    node0x%p [rank = %d, label = \"{ <addr>type: %s| val = %d|{<left>LEFT| <right>RIGHT}}\", style = filled, fillcolor = \"#d1e6ffff\", color = black];\n",
                node, rank, typeName, (nodeValue(node))->constValue);

    node_t** left = nodeLeft(node);
    if((left != NULL) && (*left != NULL) && !(_txIsBadReadPtr(*left)))
        fprintfNodeGraph(*nodeLeft(node), rank + 1, graphFile, nodesPassed, treeSize);

    node_t** right = nodeRight(node);
    if((right != NULL) && (*right != NULL) && !(_txIsBadReadPtr(*right)))
        fprintfNodeGraph(*nodeRight(node), rank + 1, graphFile, nodesPassed, treeSize);

    return 0;
}

int fprintfNodeLinksForGraph (node_t* node, FILE* graphFile, size_t* nodesPassed, size_t treeSize) {
    assert(node);
    assert(graphFile);

    //(*nodesPassed) += 1;
    //if (*nodesPassed > treeSize)
    //    return tooManyRecursiveCalls;

    node_t** left = nodeLeft(node);
    if((left != NULL) && (*left != NULL) && !(_txIsBadReadPtr(*left))) {
        fprintf(graphFile, "    node0x%p:left -> node0x%p:addr [color = \"#666350ff\"];\n", node, *nodeLeft(node));
        fprintfNodeLinksForGraph(*nodeLeft(node), graphFile, nodesPassed, treeSize);
    }

    if((left != NULL) && (*left != NULL) && (_txIsBadReadPtr(*left))) {
        fprintf(graphFile, "    errorNode0x%p [label = \"ERROR!\\n 0x%p \", style = filled, fillcolor = \"#be3131ff\", color = black, fontcolor = white, shape = doubleoctagon];\n",
                *left, *left);
        fprintf(graphFile, "    node0x%p:left -> errorNode0x%p [color = \"#f90d0dff\"];\n", node, *left);
    }

    node_t** right = nodeRight(node);
    if((right != NULL) && (*right != NULL) && !(_txIsBadReadPtr(*right))) {
        fprintf(graphFile, "    node0x%p:right -> node0x%p:addr [color = \"#666350ff\"];\n", node, *nodeRight(node));
        fprintfNodeLinksForGraph(*nodeRight(node), graphFile, nodesPassed, treeSize);
    }

    if((right != NULL) && (*right != NULL) && (_txIsBadReadPtr(*right))) {
        fprintf(graphFile, "    errorNode0x%p [label = \"ERROR!\\n 0x%p \", style = filled, fillcolor = \"#be3131ff\", color = black, fontcolor = white, shape = doubleoctagon];\n",
                *right, *right);
        fprintf(graphFile, "    node0x%p:right -> errorNode0x%p [color = \"#f90d0dff\"];\n", node, *right);
    }

    return 0;
}

void treeDump (struct tree_t* tree, struct dump* dumpInfo, const char* message) {
    assert(tree);
    assert(dumpInfo);
    assert(message);

    const char* nameOfTextGraphFile = dumpInfo->nameOfGraphFile;

    FILE* dumpFile = 0;
    if(dumpInfo->dumpFileWasOpened)
        dumpFile = fopen(dumpInfo->nameOfDumpFile, "a");
    else {
        dumpFile = fopen(dumpInfo->nameOfDumpFile, "w");
        dumpInfo->dumpFileWasOpened = 1;
    }

    if (dumpFile == NULL) {
        fprintf(stderr, "Error of opening file \"%s\"", dumpInfo->nameOfDumpFile);
        perror("");
        return;
    }

    fprintf(dumpFile, "<pre>\n");
    fprintf(dumpFile, "<h3>treeDump() <font color=red>from %s at %s:%ud</font></h3>\n",
    dumpInfo->nameOfFunc, dumpInfo->nameOfFile, dumpInfo->numOfLine);

    fprintf(dumpFile, "<h2><font color=blue>%s</font></h2>\n", message);
    fprintfTreeErrorsForDump (tree, dumpFile, dumpInfo);

    fprintf(dumpFile, "Root Node == 0x%p\n", *treeRoot(tree));
    fprintf(dumpFile, "Tree size == %llu\n", *treeSize(tree));
    fprintf(dumpFile, "ErrorCode == %d\n", tree->errorCode);

    //fprintf(dumpFile, "number of variables == %llu\n", tree->identifierArrSize);
    //for (size_t numOfVar = 0; numOfVar < tree->identifierArrSize; numOfVar++)
    //    fprintf(dumpFile, "var[%llu] == \"%s\"; value == [%lf]; hash == %llu\n", numOfVar,
    //    (tree->identifierArr[numOfVar]).identifierName, (tree->identifierArr[numOfVar]).identifierValue,
    //    (tree->identifierArr[numOfVar]).identifierHash);

    createGraphImageForDump (tree, dumpFile, nameOfTextGraphFile);

    if (fclose(dumpFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", dumpInfo->nameOfGraphFile);
        perror("");
        return;
    }
}

void createGraphImageForDump (struct tree_t* tree, FILE* dumpFile, const char* nameOfTextGraphFile) {
    assert(tree);
    assert(dumpFile);
    assert(nameOfTextGraphFile);

    static int graphImageCounter = 0;
    graphImageCounter++;

    fprintfTreeGraphDump(tree, nameOfTextGraphFile);

    char graphvizCallCommand[STR_SIZE] = {};
    snprintf(graphvizCallCommand, sizeof(graphvizCallCommand), "dot -Tpng %s -o DUMPS/graph%d.png", nameOfTextGraphFile, graphImageCounter); //FIXME
    system(graphvizCallCommand);
    fprintf(dumpFile, "Image:\n <img src=graph%d.png width=1000px>\n", graphImageCounter);

}

int nodeVerifier (node_t* node, int* errorCode, size_t* nodesPassed, size_t treeSize) {
    assert(node);

    //(*nodesPassed) += 1;
    //if (*nodesPassed > treeSize)
    //    return tooManyRecursiveCalls;

    if (_txIsBadReadPtr(*nodeRight(node)) && (*nodeRight(node) != NULL))
        (*errorCode) |= badRight;

    if (_txIsBadReadPtr(*nodeLeft(node)) && (*nodeLeft(node) != NULL))
        (*errorCode) |= badLeft;

    if  (!(_txIsBadReadPtr(*nodeRight(node))) && (*nodeRight(node)) != NULL)
        nodeVerifier(*nodeRight(node), errorCode, nodesPassed, treeSize);

    if  (!(_txIsBadReadPtr(*nodeLeft(node))) && (*nodeLeft(node)) != NULL)
        nodeVerifier(*nodeLeft(node), errorCode, nodesPassed, treeSize);

    return (*errorCode);
}

void fprintfTreeErrorsForDump (struct tree_t* tree, FILE* dumpFile, struct dump* dumpInfo) {
    assert(tree);
    assert(dumpInfo);
    assert(dumpFile);

    if (tree->errorCode & badLeft) {
        fprintf(dumpFile, "<h2><font color=red>ERROR! BAD LEFT NODE LINK! errorcode = %d</font></h2>\n", badLeft);
        printf("ERROR! BAD LEFT NODE LINK! errorcode = %d; In func %s from %s:%ud\n",
        badLeft, dumpInfo->nameOfFunc, dumpInfo->nameOfFile, dumpInfo->numOfLine);
    }

    if (tree->errorCode & badRight) {
        fprintf(dumpFile, "<h2><font color=red>ERROR! RIGHT LEFT NODE LINK! errorcode = %d</font></h2>\n", badRight);
        printf("ERROR! BAD RIGHT NODE LINK! errorcode = %d; In func %s from %s:%ud\n",
        badRight, dumpInfo->nameOfFunc, dumpInfo->nameOfFile, dumpInfo->numOfLine);
    }

    if (tree->errorCode & tooManyRecursiveCalls) {
        fprintf(dumpFile, "<h2><font color=red>ERROR! TOO MANY RECURSIVE CALLS! errorcode = %d</font></h2>\n", tooManyRecursiveCalls);
        printf("ERROR!TOO MANY RECURSIVE CALLS! errorcode = %d; In func %s from %s:%ud\n",
        tooManyRecursiveCalls, dumpInfo->nameOfFunc, dumpInfo->nameOfFile, dumpInfo->numOfLine);
    }
}

int treeVerifier (tree_t* tree) {
    assert(tree);

    node_t* rootNode = *treeRoot(tree);

    size_t numOfVerifiedNodes = 0;
    int error = nodeVerifier(rootNode, &(tree->errorCode), &numOfVerifiedNodes, *treeSize(tree));

    if (error & tooManyRecursiveCalls)
        tree->errorCode |= tooManyRecursiveCalls;

    return tree->errorCode;
}

int deleteNode(tree_t* tree, node_t* node) {
    assert(node);
    assert(tree);

    node_t** left = nodeLeft(node);
    if((left != NULL) && (*left != NULL)
        && !(_txIsBadReadPtr(*left)))
        deleteNode(tree, *nodeLeft(node));

    node_t** right = nodeRight(node);
    if((right != NULL) && (*right != NULL)
        && !(_txIsBadReadPtr(*right)))
        deleteNode(tree, *nodeRight(node));

    free(node);
    *treeSize(tree) -= 1;
    return 0;
}

int deleteTree (tree_t* tree) {
    assert(tree);

    node_t* rootNode = *treeRoot(tree);

    deleteNode(tree, rootNode);

    return noErrors;
}


node_t* newNodeCtor(nodeType_t curType, nodeValue_t curValue, size_t curLine) {

    node_t* newNode = (node_t*)calloc(1, sizeof(node_t));

    *nodeType(newNode) = curType;
    *nodeValue(newNode) = curValue;

    *nodeLine(newNode) = curLine;

    return newNode;
}

