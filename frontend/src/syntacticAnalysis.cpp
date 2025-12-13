#include <txLib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../../COMMON/include/nameTableConsts.h"
#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/structAccessFunctions.h"
#include "../../COMMON/include/nameTableStack.h"

#include "../include/syntacticAnalysis.h"
#include "../include/DSL.h"

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

node_t* getProgramTree (tree_t* tree, lexAnalysisResult* lexResult) {
    assert(tree);
    assert(lexResult);

    node_t** nodeArr = lexResult->nodesArray;
    size_t numOfNodes = lexResult->numOfNodes;

    size_t nodeNum = 0;
    size_t* curNodeNum = &nodeNum;

    initNameTables(tree);

    if (tree->errorCode)
        return NULL;
$$
    while (NODE_IS_OP_(opINIT)) {
        if (getFunctionsDeclarations(tree, nodeArr, curNodeNum))
            return NULL;
    }
$$
    node_t* firstOperator = NULL;
    if (NODE_IS_ID)
        firstOperator = getFunction(tree, nodeArr, curNodeNum);
    else {
        syntaxError(tree, nodeArr, curNodeNum, __func__);
        return NULL;
    }

$$
    while (nodeNum < numOfNodes - 1) {
        node_t* secondOperator = NULL;

        secondOperator = getFunction(tree, nodeArr, curNodeNum);

        node_t* linkNode = nodeArr[*curNodeNum - 1];
        *nodeLeft(linkNode) = firstOperator;
        *nodeRight(linkNode) = secondOperator;

        firstOperator = linkNode;
    }
$$
    if (checkAllFunctionsHaveBodies (tree))
        return NULL;
$$
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

    node_t* funcNode = nodeArr[*curNodeNum];
    const char* funcName = (funcNode->value).id.identifierName;

    identifierInfo* funcId = findIdInAllScopes(tree, funcName);
    if (!funcId && strcmp(funcName, MAIN_FUNCTION)) {
        printf("ERROR! No previous declaration for func \"%s\".\n", funcName);
        tree->errorCode |= treeNameTableError;
        return NULL;
    }
    (*curNodeNum)++;

    size_t numOfFuncParamsExpected = 0;
    if (funcId) {
        numOfFuncParamsExpected = funcId->idInfo.funcInfo.paramCount;
        funcId->idInfo.funcInfo.haveBody = 1;
    }

    size_t curNumOfParams = 0;

    CHECK_THE_NODE_IS_(opBRACK_ON);
    enterNewScope(tree);

    node_t* funcParam = NULL;
    if (NODE_IS_ID) {
        funcParam = getFuncParam (tree, nodeArr, curNodeNum);
        curNumOfParams++;
    }


    while(NODE_IS_OP_(opCOMMA)) {
        node_t* nodeComma = (nodeArr[*curNodeNum]);
        (*curNodeNum)++;

        node_t* secondParam = getFuncParam (tree, nodeArr, curNodeNum);
        curNumOfParams++;

        *nodeLeft(nodeComma) = funcParam;
        *nodeRight(nodeComma) = secondParam;

        funcParam = nodeComma;
    }

    if (numOfFuncParamsExpected != curNumOfParams) {
        printf("Error! Incorrect number of func params of func \"%s\".\n", funcName);
        syntaxError(tree, nodeArr, curNodeNum, __func__);
        return NULL;
    }

    *nodeLeft(funcNode) = funcParam;

    CHECK_THE_NODE_IS_(opBRACK_OFF);

    *nodeRight(funcNode) = getOperator(tree, nodeArr, curNodeNum);
    exitScope(tree);

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
            case opUNITED_ON:
                return getOpUnited(tree, nodeArr, curNodeNum);
            case opINIT:
                return getOpInit(tree, nodeArr, curNodeNum);
            case opHLT: {
                node_t* hltNode = nodeArr[(*curNodeNum)++];
                CHECK_THE_NODE_IS_(opSEPARATOR);
                return hltNode; }
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

            if (((nodeArr[*curNodeNum + 1])->value).opCode == opBRACK_ON) {
                node_t* callNode = getCall(tree, nodeArr, curNodeNum);
                CHECK_THE_NODE_IS_(opSEPARATOR);
                return callNode;
            }
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

    *nodeLeft(newNode) = getVarIDNode(tree, nodeArr, curNodeNum);

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

    enterNewScope(tree);

    node_t* firstOperator = NULL;

    firstOperator = getOperator(tree, nodeArr, curNodeNum);


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

    exitScope(tree);

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
    const char* funcName = (funcNode->value).id.identifierName;

    identifierInfo* funcInfo = findIdInAllScopes(tree, funcName);

    if ((!funcInfo) || (funcInfo->idType != idFUNC)) {
        printf("Undeclared function: %s\n", funcName);
        syntaxError(tree, nodeArr, curNodeNum, __func__);
        return NULL;
    }

    size_t numOfFuncParams = funcInfo->idInfo.funcInfo.paramCount;

    (*curNodeNum)++;

    CHECK_THE_NODE_IS_(opBRACK_ON);

    node_t* funcParam = NULL;
    size_t numOfCallParams = 0;

    //if (NODE_IS_ID) {
    if (!NODE_IS_OP_(opBRACK_OFF)) {
        //funcParam = getVarIDNode(tree, nodeArr, curNodeNum);
        funcParam = getExpressionNode(tree, nodeArr, curNodeNum);
        numOfCallParams++;
    }

    while (NODE_IS_OP_(opCOMMA)) {
        node_t* nodeComma = (nodeArr[*curNodeNum]);
        (*curNodeNum)++;

        //node_t* secondParam = getVarIDNode(tree, nodeArr, curNodeNum);
        node_t* secondParam = getExpressionNode(tree, nodeArr, curNodeNum);

        //node_t* nodeComma = (nodeArr[*curNodeNum - 2]);
        *nodeLeft(nodeComma) = funcParam;
        *nodeRight(nodeComma) = secondParam;

        funcParam = nodeComma;
        numOfCallParams++;
    }

    *nodeLeft(funcNode) = funcParam;

    CHECK_THE_NODE_IS_(opBRACK_OFF);
    //CHECK_THE_NODE_IS_(opSEPARATOR); //FIXME

    if (numOfCallParams != numOfFuncParams) {
        printf("Error! Incorrect number of func params of func \"%s\".\n", funcName);
        syntaxError(tree, nodeArr, curNodeNum, __func__);
        return NULL;
    }


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
        if ((nodeArr[*curNodeNum + 1])->type == typeOperator && ((nodeArr[*curNodeNum + 1])->value).opCode == opBRACK_ON)
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

node_t* getVarIDNode(tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(tree);
    assert(nodeArr);
    assert(curNodeNum);

    if (tree->errorCode)
        return NULL;

    node_t* newNode = nodeArr[*curNodeNum];
    const char* varName = (newNode->value).id.identifierName;

    identifierInfo* idInfo = findIdInAllScopes(tree, varName);
    if (!idInfo) {
        printf("Undeclared identifier: %s\n", varName);
        syntaxError(tree, nodeArr, curNodeNum, __func__);
        return NULL;
    }

    (*curNodeNum)++;
    return newNode;
}

node_t* getOpInit(tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(tree);
    assert(nodeArr);
    assert(curNodeNum);

    if (tree->errorCode)
        return NULL;

    (*curNodeNum)++;

     if (!NODE_IS_ID) {
        syntaxError(tree, nodeArr, curNodeNum, __func__);
        return NULL;
    }

    node_t* idNode = nodeArr[*curNodeNum];
    const char* idName = (idNode->value).id.identifierName;

    if (findIdInTable ((tree->nameTableStack->data)[(tree->nameTableStack->size) - 1], idName)) {
        printf("Error! Redeclaration of identifier \"%s\".\n", idName);
        syntaxError(tree, nodeArr, curNodeNum, __func__);
        return NULL;
    };

    identifierInfo* newId = addIdToCurrentScope(tree, idName, idVAR);

    if (!newId) {
        printf("Failed to add identifier %s to name table\n", idName);
        tree->errorCode |= treeNameTableError;
        return NULL;
    }

    (*curNodeNum)++;

    if (!NODE_IS_OP_(opASSIGN)) {
        syntaxError(tree, nodeArr, curNodeNum, __func__);
        return NULL;
    }

    node_t* assignNode = nodeArr[*curNodeNum];
    (*curNodeNum)++;

    *nodeLeft(assignNode) = idNode;
    *nodeRight(assignNode) = getExpressionNode(tree, nodeArr, curNodeNum);

    CHECK_THE_NODE_IS_(opSEPARATOR);

    return assignNode;
}

int getFunctionsDeclarations(tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(tree);
    assert(nodeArr);
    assert(curNodeNum);

    if (tree->errorCode)
        return 1;

    (*curNodeNum)++;

    if (!NODE_IS_ID) {
        syntaxError(tree, nodeArr, curNodeNum, __func__);
        return 1;
    }

    node_t* funcNode = nodeArr[*curNodeNum];
    const char* funcName = (funcNode->value).id.identifierName;

    identifierInfo* newFunc = addIdToCurrentScope(tree, funcName, idFUNC);

    if (!newFunc) {
        printf("Failed to add identifier %s to name table\n", funcName);
        tree->errorCode |= treeNameTableError;
        return 1;
    }

    (*curNodeNum)++;
    CHECK_THE_NODE_IS_(opBRACK_ON);

    size_t numOfParams = 0;

    if (NODE_IS_ID) {
        numOfParams++;
        (*curNodeNum)++;
    }

    while(NODE_IS_OP_(opCOMMA)) {
        (*curNodeNum)++;

        if (NODE_IS_ID) {
            numOfParams++;
            (*curNodeNum)++;
        }
        else {
            syntaxError(tree, nodeArr, curNodeNum, __func__);
            return 1;
        }
    }

    newFunc->idInfo.funcInfo.paramCount = numOfParams;

    CHECK_THE_NODE_IS_(opBRACK_OFF);
    CHECK_THE_NODE_IS_(opSEPARATOR);

    return 0;
}

node_t* getFuncParam (tree_t* tree, node_t** nodeArr, size_t* curNodeNum) {
    assert(tree);
    assert(nodeArr);
    assert(curNodeNum);

    node_t* paramNode = nodeArr[*curNodeNum];
    const char* paramName = paramNode->value.id.identifierName;

    identifierInfo* paramId = addIdToCurrentScope(tree, paramName, idVAR);
    if (!paramId) {
        printf("Failed to add parameter %s to name table\n", paramName);
        tree->errorCode |= treeNameTableError;
        exitScope(tree);
        return NULL;
    }

    (*curNodeNum)++;

    return paramNode;
}

int checkAllFunctionsHaveBodies (tree_t* tree) {
    assert(tree);

    int funcsWithoutBodies = 0;

    nameTable_t* globalTable = tree->nameTableStack->data[0];
    size_t globalTableSize = globalTable->size;

    for (size_t curId = 0; curId < globalTableSize; curId++)
        if ((globalTable->idArray)[curId].idType == idFUNC)
            if ((globalTable->idArray)[curId].idInfo.funcInfo.haveBody == 0) {
                funcsWithoutBodies++;
                printf("Error! undefined reference to func `%s'\n", (globalTable->idArray)[curId].identifierName);
            }

    return funcsWithoutBodies;
}
