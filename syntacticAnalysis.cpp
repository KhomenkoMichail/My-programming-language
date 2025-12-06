#include <stdio.h>

#include "structsAndConsts.h"
#include "structAccessFunctions.h"
#include "syntacticAnalysis.h"

void syntaxError (node_t** curNode, const char* funcName) {
    assert(nodeArray);

    printf("Func %s detected a syntax error!\n", funcName);

    printf ("in line : %lld ", (*curNode)->numOfLine);
    switch ((*curNode)->type) {
        case typeOperator:
            printf("operator {%d}\n", ((*curNode)->value).opCode);
            break;
        case typeNumber:
            printf("number {%d}\n", ((*curNode)->value).constValue);
            break;
        case typeIdentifier:
            printf("identifier \"%s\" {%lld}\n", ((*curNode)->value).id.idName,
                                                ((*curNode)->value).id.identifierHash);
            break;
        case typeError:
        default:
            break;
    }

    assert(0);
}

node_t* getProgramTree (node_t** curNode) {
    assert(curNode);

    node_t* firstOperator = 0;
    if ((*curNode)->type == typeIdentifier)
        firstOperator = getFunction(curNode);
    else
        syntaxError(curNode, __func__);


    while ((*curNode)->type == typeIdentifier) {
        node_t* secondOperator = 0;

        secondOperator = getFunction(curNode);

        node_t* linkNode = *curNode - 1;
        *nodeLeft(linkNode) = firstOperator;
        *nodeRight(linkNode) = secondOperator;

        firstOperator = linkNode;
    }

    return firstOperator;
}

node_t* getFunction (node_t** )


