#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/structAccessFunctions.h"
#include "../../COMMON/include/treeFunctions.h"

#include "../include/middleendOptimization.h"

void simplifyTree (tree_t* tree, dump* dumpInfo) {
    assert(tree);
    assert(dumpInfo);

    treeDump(tree, dumpInfo, "program tree BEFORE optimization:");

    size_t oldTreeSize = 0;
    do {
        oldTreeSize = *treeSize(tree);
        constConvolution(tree, (*treeRoot(tree)));
        deleteNeutralNode(tree, (*treeRoot(tree)));
    } while (oldTreeSize != *treeSize(tree));

    treeDump(tree, dumpInfo, "program tree AFTER optimization:");
}

int constConvolution (tree_t* tree, node_t* node) {
    assert(tree);
    assert(node);

    int leftConvolution = NOT_CONVOLUTED;
    int rightConvolution = NOT_CONVOLUTED;

    switch (*nodeType(node)) {
        case typeNumber:
            return (nodeValue(node))->constValue;
        case typeIdentifier:
            if(*nodeLeft(node))
                leftConvolution = constConvolution (tree, *nodeLeft(node));
            if (*nodeRight(node))
                rightConvolution = constConvolution (tree, *nodeRight(node));
            return NOT_CONVOLUTED;
        case typeOperator:
            if(*nodeLeft(node))
                leftConvolution = constConvolution (tree, *nodeLeft(node));
            if (*nodeRight(node))
                rightConvolution = constConvolution (tree, *nodeRight(node));

            if ((leftConvolution != NOT_CONVOLUTED) && (rightConvolution != NOT_CONVOLUTED)) {

                nodeValue(node)->constValue = solveNode(tree, node);
                *nodeType(node) = typeNumber;

                if(*nodeLeft(node)) {
                    free(*nodeLeft(node));
                    *treeSize(tree) -= 1;
                }

                if(*nodeRight(node)) {
                    free(*nodeRight(node));
                    *treeSize(tree) -= 1;
                }

                *nodeLeft(node) = NULL;
                *nodeRight(node) = NULL;

                return nodeValue(node)->constValue;
            }
            return NOT_CONVOLUTED;
        case typeError:
        default: return NOT_CONVOLUTED;
    }
    return NOT_CONVOLUTED;
}


int solveNode (tree_t* tree, node_t* node) {
    assert(tree);
    assert(node);

    switch (*nodeType(node)) {
        case typeNumber:
            return (nodeValue(node))->constValue;
            break;
        case typeOperator: {
            int leftValue = 0;
            int rightValue = 0;

            if(*nodeLeft(node))
                leftValue = solveNode(tree, *nodeLeft(node));

            if(*nodeRight(node))
                rightValue = solveNode(tree, *nodeRight(node));

            switch ((nodeValue(node))->opCode) {
                case opADD:
                    return (leftValue + rightValue);
                case opSUB:
                    return (leftValue - rightValue);
                case opMUL:
                    return (leftValue * rightValue);
                case opSQRT:
                    return (int)sqrt(leftValue);
                case opDIV:
                    if (rightValue != 0)
                        return (leftValue / rightValue);
                    else return CANT_SOLVE_NODE;
                case opUNKNOWN:
                case opSEPARATOR:
                case opBRACK_ON:
                case opBRACK_OFF:
                case opASSIGN:
                case opWHILE:
                case opIF:
                case opIN:
                case opOUT:
                case opRET:
                case opHLT:
                case opCOMMA:
                case opUNITED_ON:
                case opUNITED_OFF:
                case opQUOTES:
                case opEQUAL:
                case opBELOW:
                case opABOVE:
                case opNOT_EQUAL:
                case opE_BELOW:
                case opE_ABOVE:
                case opINIT:

                default: return CANT_SOLVE_NODE;
            }
        }
        case typeIdentifier:
        case typeError:
        default: return CANT_SOLVE_NODE;
    }

    return CANT_SOLVE_NODE;
}

int deleteNeutralNode (tree_t* tree, node_t* node) {
    assert(tree);
    assert(node);

    if (*nodeLeft(node))
        deleteNeutralNode(tree, *nodeLeft(node));

    if (*nodeRight(node))
        deleteNeutralNode(tree, *nodeRight(node));

    if (*nodeType(node) == typeOperator) {
        if (nodeValue(node)->opCode == opMUL) {

            if ((IS_NUM(Left, 0)) || (IS_NUM(Right, 0))) {

                makeZeroNode(tree, node);
                return 1;
            }

            if (IS_NUM(Left, 1)) {

                deleteLeftNode(tree, node);
                return 1;
            }

            if (IS_NUM(Right, 1)) {

                deleteRightNode(tree, node);
                return 1;
            }
        }
        if (nodeValue(node)->opCode == opDIV) {

            if (IS_NUM(Left, 0)) {

                makeZeroNode(tree, node);
                return 1;
            }
            if (IS_NUM(Right, 1)) {

                deleteRightNode(tree, node);
            }
        }
        if (nodeValue(node)->opCode == opADD) {
            if (IS_NUM(Left, 0)) {

                deleteLeftNode(tree, node);
                return 1;
            }
            if (IS_NUM(Right, 0)) {

                deleteRightNode(tree, node);
                return 1;
            }
        }
        if (nodeValue(node)->opCode == opSUB) {
            if (IS_NUM(Right, 0)) {

                deleteRightNode(tree, node);
                return 1;
            }
        }
    }

    return 0;
}

void makeZeroNode(tree_t* tree, node_t* node) {
    assert(tree);
    assert(node);

    deleteNode(tree, *nodeLeft(node));
    deleteNode(tree, *nodeRight(node));

    *nodeLeft(node) = NULL;
    *nodeRight(node) = NULL;

    *nodeType(node) = typeNumber;
    nodeValue(node)->constValue = 0;
}

void deleteRightNode(tree_t* tree, node_t* node) {
    assert(tree);
    assert(node);

    free(*nodeRight(node));

    node_t* parNode = *nodeParent(node);
    if (parNode) {
        if (*nodeLeft(parNode) == node)
            *nodeLeft(parNode) = *nodeLeft(node);
        else
            *nodeRight(parNode) = *nodeLeft(node);
    }
    else
        *treeRoot(tree) = *nodeLeft(node);

    *nodeParent(*nodeLeft(node)) = parNode;

    free(node);
    *treeSize(tree) -= 2;
}

void deleteLeftNode(tree_t* tree, node_t* node) {
    assert(tree);
    assert(node);

    free(*nodeLeft(node));

    node_t* parNode = *nodeParent(node);
    if (parNode) {
        if (*nodeLeft(parNode) == node)
            *nodeLeft(parNode) = *nodeRight(node);
        else
            *nodeRight(parNode) = *nodeRight(node);
    }
    else
        *treeRoot(tree) = *nodeRight(node);

    *nodeParent(*nodeRight(node)) = parNode;

    free(node);
    *treeSize(tree) -= 2;
}
