#include <stdio.h>
#include <assert.h>

#include "nameTableConsts.h"
#include "structsAndConsts.h"
#include "structAccessFunctions.h"
#include "syntacticAnalysis.h"
#include "nameTableStack.h"
#include "nameTableStack.h"
#include "DSL.h"

void syntaxError (tree_t* tree, node_t** nodeArr, size_t* curNodeNum, const char* funcName) {
    assert(nodeArr);
    assert(curNodeNum);
    assert(funcName);

    tree->errorCode |= treeSyntaxError;

    printf("Func %s detected a syntax error!\n", funcName);

    printf ("In line : %lld ", (nodeArr[*curNodeNum])->numOfLine);
    switch ((nodeArr[*curNodeNum])->type) {
        case typeOperator:
            printf("operator {%d}\n", ((nodeArr[*curNodeNum])->value).opCode);
            break;
        case typeNumber:
            printf("number {%d}\n", ((nodeArr[*curNodeNum])->value).constValue);
            break;
        case typeIdentifier:
            printf("identifier \"%s\" {%lld}\n", ((nodeArr[*curNodeNum])->value).id.identifierName,
                                                ((nodeArr[*curNodeNum])->value).id.identifierHash);
            break;
        case typeError:
        default:
            break;
    }
    printf("Is unexpected.\n");
}

node_t* getProgramTree (tree_t* tree, node_t** nodeArr, size_t numOfNodes) {
    assert(tree);
    assert(nodeArr);

    size_t nodeNum = 0;
    size_t* curNodeNum = &nodeNum;

    if (tree->errorCode)
        return NULL;

    node_t* firstOperator = NULL;
    if (NODE_IS_ID)
        firstOperator = getFunction(tree, nodeArr, curNodeNum);
    else {
        syntaxError(tree, nodeArr, curNodeNum, __func__);
        return NULL;
    }


    while (nodeNum < numOfNodes - 1) { // NOTE may cause prblm
        node_t* secondOperator = NULL;

        secondOperator = getFunction(tree, nodeArr, curNodeNum);

        node_t* linkNode = nodeArr[*curNodeNum - 1];
        *nodeLeft(linkNode) = firstOperator;
        *nodeRight(linkNode) = secondOperator;

        firstOperator = linkNode;
    }//FIXME проверить что прочитали все

    return firstOperator;
}

node_t* getFunction (tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(nodeArr);
    assert(curNodeNum);
    assert(tree);

    if (tree->errorCode)
        return NULL;

    if ((nodeArr[*curNodeNum])->type != typeIdentifier) {
        syntaxError(tree, nodeArr, curNodeNum, __func__);
        return NULL;
    }

    node_t* funcNode = nodeArr[*curNodeNum]; //FIXME nametable
    (*curNodeNum)++;

    CHECK_THE_NODE_IS_(opBRACK_ON);

    node_t* funcParam = NULL;
    if (NODE_IS_ID) {
        funcParam = (nodeArr[*curNodeNum]); //FIXME nametable
        (*curNodeNum)++;
    }


    while(NODE_IS_OP_(opCOMMA)) {

        (*curNodeNum)++;
        node_t* secondParam = nodeArr[*curNodeNum]; //FIXME nametable

        node_t* nodeComma = (nodeArr[*curNodeNum - 1]);
        *nodeLeft(nodeComma) = funcParam;
        *nodeRight(nodeComma) = secondParam;

        (*curNodeNum)++; //FIXME
        funcParam = nodeComma;
    }

    *nodeLeft(funcNode) = funcParam;

    CHECK_THE_NODE_IS_(opBRACK_OFF);

    *nodeRight(funcNode) = getOperator(tree, nodeArr, curNodeNum);

    return funcNode;
}

node_t* getOperator (tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(tree);
    assert(nodeArr);
    assert(curNodeNum);

    if (tree->errorCode)
        return NULL;

    if (NODE_IS_OP) {
        switch (((nodeArr[*curNodeNum])->value).opCode) {
            case opIF:
                return getOpIfOrWhile(tree, nodeArr, curNodeNum);
            case opWHILE:
                return getOpIfOrWhile(tree, nodeArr, curNodeNum);
            case opIN:
                return getOpInOrOut(tree, nodeArr, curNodeNum);
            case opOUT:
                return getOpInOrOut(tree, nodeArr, curNodeNum);
            case opRET:
                return getOpRet(tree, nodeArr, curNodeNum);
            case opHLT:
                return nodeArr[*curNodeNum];
            case opUNITED_ON:
                return getOpUnited(tree, nodeArr, curNodeNum);
            case opUNKNOWN:
            case opADD:
            case opSUB:
            case opMUL:
            case opDIV:
            case opSEPARATOR:
            case opBRACK_ON:
            case opBRACK_OFF:
            case opSQRT:
            case opCOMMA:
            case opASSIGN:
            case opUNITED_OFF:
            case opQUOTES:
            case opEQUAL:
            case opBELOW:
            case opABOVE:
            case opNOT_EQUAL:
            case opE_BELOW:
            case opE_ABOVE:
            default:
            syntaxError(tree, nodeArr, curNodeNum, __func__);
            return NULL;
        }
    }
    else if (NODE_IS_ID) {
        if ((nodeArr[*curNodeNum + 1])->type == typeOperator) {

            if (((nodeArr[*curNodeNum + 1])->value).opCode == opASSIGN)
                return getOpAssign(tree, nodeArr, curNodeNum);

            if (((nodeArr[*curNodeNum + 1])->value).opCode == opBRACK_ON)
                return getCall(tree, nodeArr, curNodeNum);
        }
       syntaxError(tree, nodeArr, curNodeNum, __func__);
       return NULL;
    }

    syntaxError(tree, nodeArr, curNodeNum, __func__);
    return NULL;
}

node_t* getOpIfOrWhile(tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(tree);
    assert(nodeArr);
    assert(curNodeNum);

    if (tree->errorCode)
        return NULL;

    node_t* newNode = nodeArr[*curNodeNum];
    (*curNodeNum)++;

    CHECK_THE_NODE_IS_(opBRACK_ON);

    *nodeLeft(newNode) = getExpressionNode(tree, nodeArr, curNodeNum);


    CHECK_THE_NODE_IS_(opBRACK_OFF);
    *nodeRight(newNode) = getOperator (tree, nodeArr, curNodeNum);

    return newNode;
}

node_t* getOpInOrOut(tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(tree);
    assert(nodeArr);
    assert(curNodeNum);

    if (tree->errorCode)
        return NULL;

    node_t* newNode = nodeArr[*curNodeNum];
    (*curNodeNum)++;

    CHECK_THE_NODE_IS_(opQUOTES);

    *nodeLeft(newNode) = getVarIDNode(tree, nodeArr, curNodeNum); //FIXME

    CHECK_THE_NODE_IS_(opQUOTES);
    CHECK_THE_NODE_IS_(opSEPARATOR);

    return newNode;
}

node_t* getOpRet(tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(tree);
    assert(nodeArr);
    assert(curNodeNum);

    if (tree->errorCode)
        return NULL;

    node_t* nodeRet = nodeArr[*curNodeNum];
    (*curNodeNum)++;

    *nodeLeft(nodeRet) = getExpressionNode(tree, nodeArr, curNodeNum);

    CHECK_THE_NODE_IS_(opSEPARATOR);

    return nodeRet;
}

node_t* getOpUnited(tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(tree);
    assert(nodeArr);
    assert(curNodeNum);

    if (tree->errorCode)
        return NULL;

    (*curNodeNum)++;

    if (NODE_IS_OP_(opUNITED_OFF)) {
        (*curNodeNum)++;
        if (NODE_IS_OP_(opSEPARATOR)) {
            (*curNodeNum)++;
            return NULL;
        }
        else {
            syntaxError(tree, nodeArr, curNodeNum, __func__);
            return NULL;
        }
    }

    node_t* firstOperator = NULL;
    //if (NODE_IS_OP)
        firstOperator = getOperator(tree, nodeArr, curNodeNum);
    /*else {
        syntaxError(tree, nodeArr, curNodeNum, __func__);
        return NULL;
    }*/


    while (!(NODE_IS_OP_(opUNITED_OFF))) {
        node_t* secondOperator = NULL;

        secondOperator = getOperator(tree, nodeArr, curNodeNum);

        node_t* linkNode = nodeArr[*curNodeNum - 1];
        *nodeLeft(linkNode) = firstOperator;
        *nodeRight(linkNode) = secondOperator;

        firstOperator = linkNode;
    }

    CHECK_THE_NODE_IS_(opUNITED_OFF);
    CHECK_THE_NODE_IS_(opSEPARATOR);

    return firstOperator;
}

node_t* getOpAssign(tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(tree);
    assert(nodeArr);
    assert(curNodeNum);

    node_t* varNode = getVarIDNode(tree, nodeArr, curNodeNum);

    node_t* assignNode = nodeArr[*curNodeNum];
    (*curNodeNum)++;

    *nodeLeft(assignNode) = varNode;
    *nodeRight(assignNode) = getExpressionNode(tree, nodeArr, curNodeNum);

    CHECK_THE_NODE_IS_(opSEPARATOR);

    return assignNode;
}

node_t* getCall(tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(tree);
    assert(nodeArr);
    assert(curNodeNum);

    if (tree->errorCode)
        return NULL;

    node_t* funcNode = nodeArr[*curNodeNum];
    (*curNodeNum)++;

    CHECK_THE_NODE_IS_(opBRACK_ON);

    node_t* funcParam = NULL;
    if (NODE_IS_ID)
        funcParam = getVarIDNode(tree, nodeArr, curNodeNum); //FIXME nametable

    while(NODE_IS_OP_(opCOMMA)) {

        (*curNodeNum)++;
        node_t* secondParam = getVarIDNode(tree, nodeArr, curNodeNum); //FIXME nametable

        node_t* nodeComma = (nodeArr[*curNodeNum - 2]);
        *nodeLeft(nodeComma) = funcParam;
        *nodeRight(nodeComma) = secondParam;

        //(*curNodeNum)++; //FIXME
        funcParam = nodeComma;
    }

    *nodeLeft(funcNode) = funcParam;

    CHECK_THE_NODE_IS_(opBRACK_OFF);
    CHECK_THE_NODE_IS_(opSEPARATOR);

    return funcNode;
}

node_t* getExpressionNode(tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(tree);
    assert(nodeArr);
    assert(curNodeNum);

    if (tree->errorCode)
        return NULL;

    node_t* newExpressionNode = getADDandSUBnodes(tree, nodeArr, curNodeNum);

    if (NODE_IS_COMPARE) {
        node_t* leftOperand = newExpressionNode;

        newExpressionNode = nodeArr[*curNodeNum];
        (*curNodeNum)++;

        node_t* rightOperand = getADDandSUBnodes(tree, nodeArr, curNodeNum);

        *nodeLeft(newExpressionNode) = leftOperand;
        *nodeRight(newExpressionNode) = rightOperand;
    }

    return newExpressionNode;
}

node_t* getADDandSUBnodes (tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(tree);
    assert(nodeArr);
    assert(curNodeNum);

    if (tree->errorCode)
        return NULL;

    node_t* nodeExpression = getMULandDIVnodes(tree, nodeArr, curNodeNum);

    while ((NODE_IS_OP_(opADD)) || (NODE_IS_OP_(opSUB))) {
        node_t* leftOperand = nodeExpression;

        nodeExpression = nodeArr[*curNodeNum];
        (*curNodeNum)++;

        *nodeLeft(nodeExpression) = leftOperand;
        *nodeRight(nodeExpression) = getMULandDIVnodes(tree, nodeArr, curNodeNum);
    }

    return nodeExpression;
}

node_t* getMULandDIVnodes (tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(tree);
    assert(nodeArr);
    assert(curNodeNum);

    if (tree->errorCode)
        return NULL;

    node_t* nodeExpression = getBracketExpressionNodes(tree, nodeArr, curNodeNum);

    while ((NODE_IS_OP_(opMUL)) || (NODE_IS_OP_(opDIV))) {
        node_t* leftOperand = nodeExpression;

        nodeExpression = nodeArr[*curNodeNum];
        (*curNodeNum)++;

        *nodeLeft(nodeExpression) = leftOperand;
        *nodeRight(nodeExpression) = getBracketExpressionNodes(tree, nodeArr, curNodeNum);
    }

    return nodeExpression;
}

node_t* getBracketExpressionNodes (tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(tree);
    assert(nodeArr);
    assert(curNodeNum);

    if (tree->errorCode)
        return NULL;

    if (NODE_IS_OP_(opBRACK_ON)) {
        (*curNodeNum)++;
        node_t* newNode = getADDandSUBnodes(tree, nodeArr, curNodeNum);
        if (NODE_IS_OP_(opBRACK_OFF))
            (*curNodeNum)++;
        else
            syntaxError(tree, nodeArr, curNodeNum, __func__);

        return newNode;
    }
    else if (NODE_IS_OP_(opSQRT))
        return getSQRTnode(tree, nodeArr, curNodeNum);

    else if (NODE_IS_NUM) {
        node_t* newNode = nodeArr[*curNodeNum];
        (*curNodeNum)++;
        return newNode;
    }
    else if (NODE_IS_ID) {
        if ((nodeArr[*curNodeNum])->type == typeOperator && ((nodeArr[*curNodeNum])->value).opCode == opBRACK_ON)
            return getCall(tree, nodeArr, curNodeNum);
        else
            return getVarIDNode(tree, nodeArr, curNodeNum);
    }
    else {
        syntaxError(tree, nodeArr, curNodeNum, __func__);
        return NULL;
    }
}

node_t* getSQRTnode(tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(tree);
    assert(nodeArr);
    assert(curNodeNum);

    if (tree->errorCode)
        return NULL;

    node_t* sqrtNode = nodeArr[*curNodeNum];
    (*curNodeNum)++;

    CHECK_THE_NODE_IS_(opBRACK_ON);

    *nodeLeft(sqrtNode) = getADDandSUBnodes(tree, nodeArr, curNodeNum);

    CHECK_THE_NODE_IS_(opBRACK_OFF);

    return sqrtNode;
}

node_t* getVarIDNode (tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(tree);
    assert(nodeArr);
    assert(curNodeNum);

    node_t* newNode = nodeArr[*curNodeNum];

    (*curNodeNum)++;

    return newNode;
}

//getFuncIDNode //NOTE
//getVarIDNode  //NOTE


//NOTE skip comments
