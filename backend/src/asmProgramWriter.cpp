#include <stdio.h>
#include <assert.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/structAccessFunctions.h"
#include "../../COMMON/include/treeFunctions.h"
#include "../../COMMON/include/nameTableConsts.h"
#include "../../COMMON/include/nameTableStack.h"
#include "../../COMMON/include/helpingFunctions.h"

#include "../include/asmProgramWriter.h"
#include "../include/backendConsts.h"

int rewriteAstToAsmCode (tree_t* tree, const char* nameOfAsmFile) {
    assert(tree);
    assert(nameOfAsmFile);

    FILE* asmFile = fopen(nameOfAsmFile, "w");

    if (!asmFile) {
        fprintf(stderr, "Error of opening file \"%s\"", nameOfAsmFile);
        perror("");
        return 1;
    }

    initNameTables(tree);

    if (rewriteNodeToAsmCode (tree, *treeRoot(tree), asmFile)) {
        printf("Error of rewriting AST to Asm code.\n");
        return 1;
    }

    if (fclose(asmFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", nameOfAsmFile);
        perror("");
        return 1;
    }

    return 0;
}

int rewriteNodeToAsmCode (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(node);
    assert(asmFile);

    switch (*nodeType(node)) {
        case typeNumber:
            fprintf(asmFile, "PUSH %d\n", nodeValue(node)->constValue);
            return 0;
        case typeOperator:
            return rewriteOpNodeToAsmCode(tree, node, asmFile);
        case typeIdentifier:
            return rewriteIdNodeToAsmCode(tree, node, asmFile);
        case typeError:
        default:
            return 1;
    }
}

int rewriteOpNodeToAsmCode (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(node);
    assert(asmFile);

    switch ((*nodeValue(node)).opCode) {

        case opADD:
        case opSUB:
        case opMUL:
        case opDIV:
        case opSQRT:
        case opSEPARATOR:
        case opCOMMA:
            return rewriteOpCalcToAsmCode(tree, node, asmFile);

        case opASSIGN: return rewriteOpAssignToAsmCode (tree, node, asmFile);
        case opWHILE: return rewriteOpWhileToAsmCode (tree, node, asmFile);
        case opIF: return rewriteOpIfToAsmCode (tree, node, asmFile);
        case opIN: return rewriteOpInToAsmCode (tree, node, asmFile);
        case opOUT: return rewriteOpOutToAsmCode (tree, node, asmFile);
        case opRET: return rewriteOpRetToAsmCode (tree, node, asmFile);

        case opHLT: fprintf (asmFile, "HLT\n"); return 0;

        case opEQUAL:
        case opBELOW:
        case opABOVE:
        case opNOT_EQUAL:
        case opE_BELOW:
        case opE_ABOVE:
            return rewriteOpCompareToAsmCode(tree, node, asmFile);

        case opUNITED_ON:
        case opUNITED_OFF:
        case opQUOTES:
        case opINIT:
        case opBRACK_ON:
        case opBRACK_OFF:
        case opUNKNOWN:
        default:
            printf("Error! Unexpected operator opCode:{%d}\n", (*nodeValue(node)).opCode);
            return 1;
    }

    return 0;
}

int rewriteOpCalcToAsmCode (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(node);
    assert(asmFile);

    int errorCode = 0;

    if(*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode (tree, *nodeLeft(node), asmFile);

    if(*nodeRight(node))
        errorCode = rewriteNodeToAsmCode (tree, *nodeRight(node), asmFile);

    if (nodeValue(node)->opCode == opADD)
        fprintf(asmFile, "ADD\n");

    if (nodeValue(node)->opCode == opSUB)
        fprintf(asmFile, "SUB\n");

    if (nodeValue(node)->opCode == opMUL)
        fprintf(asmFile, "MUL\n");

    if (nodeValue(node)->opCode == opDIV)
        fprintf(asmFile, "DIV\n");

    if (nodeValue(node)->opCode == opSQRT)
        fprintf(asmFile, "SQRT\n");

    return errorCode;
}

int rewriteOpAssignToAsmCode (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    int errorCode = 0;

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeRight(node), asmFile);
    else {
        printf("Error! Assign node does not have LEFT.\n");
        return 1;
    }

    if (*nodeLeft(node))
        errorCode = writeVarAddressToAsm (tree, *nodeLeft(node), asmFile);
    else {
        printf("Error! Assign node does not have LEFT.\n");
        return 1;
    }

    fprintf(asmFile, "POPM [%s]\n", VAR_ADDR_REG);

    return errorCode;
}

int writeVarAddressToAsm (tree_t* tree, node_t* varNode, FILE* asmFile) {
    assert(tree);
    assert(varNode);
    assert(asmFile);

    if (*nodeType(varNode) != typeIdentifier) {
        printf("Error! In func %s unexpected type of node.\n", __func__);
        return 1;
    }

    char* varName = nodeValue(varNode)->id.identifierName;

    size_t numOfCurNameTable = tree->nameTableStack->size - 1;
    nameTable_t* curNameTable = (tree->nameTableStack->data)[numOfCurNameTable];

    identifierInfo* searchedVarId = findIdInTable(curNameTable, varName);

    if(!searchedVarId) {
        searchedVarId = addIdToCurrentScope(tree, varName, idVAR);
        searchedVarId->idInfo.varAddress = tree->numOfVars;
        tree->numOfVars++;
    }

    fprintf(asmFile, "PUSH %lld\n", searchedVarId->idInfo.varAddress);
    fprintf(asmFile, "POPREG %s\n", VAR_ADDR_REG);

    return 0;
}

int rewriteOpWhileToAsmCode (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    int errorCode = 0;

    static int whileCounter = 0;

    whileCounter++;
    int curWhileNum = whileCounter;

    fprintf(asmFile, ":while%d\n", curWhileNum);

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeLeft(node), asmFile);
    else {
        printf("Error! While node does not have LEFT.\n");
        return 1;
    }

    fprintf(asmFile, "PUSH 0\n");
    fprintf(asmFile, "JE :endwhile%d\n", curWhileNum);

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeRight(node), asmFile);
    else {
        printf("Error! While node does not have RIGHT.\n");
        return 1;
    }

    fprintf(asmFile, "JMP :while%d\n", curWhileNum);
    fprintf(asmFile, ":endwhile%d\n", curWhileNum);

    return errorCode;
}

int rewriteOpIfToAsmCode (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    int errorCode = 0;

    static int ifCounter = 0;
    ifCounter++;

    int curIfNum = ifCounter;

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeLeft(node), asmFile);
    else {
        printf("Error! If node does not have LEFT.\n");
        return 1;
    }

    fprintf(asmFile, "PUSH 0\n");
    fprintf(asmFile, "JE :endif%d\n", curIfNum);

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeRight(node), asmFile);
    else {
        printf("Error! If node does not have RIGHT.\n");
        return 1;
    }

    fprintf(asmFile, ":endif%d\n", curIfNum);

    return errorCode;
}

int rewriteOpInToAsmCode (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    int errorCode = 0;

    fprintf(asmFile, "IN\n");

    if (*nodeLeft(node))
        errorCode = writeVarAddressToAsm (tree, *nodeLeft(node), asmFile);
    else {
        printf("Error! In node does not have LEFT.\n");
        return 1;
    }

    fprintf(asmFile, "POPM [%s]\n", VAR_ADDR_REG);

    return errorCode;
}


int rewriteOpOutToAsmCode (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    int errorCode = 0;

    if (*nodeLeft(node))
        errorCode = writeVarAddressToAsm (tree, *nodeLeft(node), asmFile);
    else {
        printf("Error! Out node does not have LEFT.\n");
        return 1;
    }

    fprintf(asmFile, "PUSHM [%s]\n", VAR_ADDR_REG);
    fprintf(asmFile, "OUT\n");

    return errorCode;
}

int rewriteOpRetToAsmCode (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    int errorCode = 0;

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeLeft(node), asmFile);

    fprintf(asmFile, "RET\n");

    return errorCode;
}

int rewriteOpCompareToAsmCode (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    int errorCode = 0;

    static int compareCounter = 0;
    compareCounter++;

    int curCompareNum = compareCounter;

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeRight(node), asmFile);
    else {
        printf("Error! Compare node does not have RIGHT.\n");
        return 1;
    }

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeLeft(node), asmFile);
    else {
        printf("Error! Compare node does not have LEFT.\n");
        return 1;
    }
    fprintf(asmFile, "PUSH 0\n");
    fprintf(asmFile, "POPREG %s\n", COMPARE_VALUE_REG);

    if (nodeValue(node)->opCode == opEQUAL)
        fprintf(asmFile, "JNE ");
    if (nodeValue(node)->opCode == opNOT_EQUAL)
        fprintf(asmFile, "JE ");
    if (nodeValue(node)->opCode == opBELOW)
        fprintf(asmFile, "JAE ");
    if (nodeValue(node)->opCode == opABOVE)
        fprintf(asmFile, "JBE ");
    if (nodeValue(node)->opCode == opE_BELOW)
        fprintf(asmFile, "JA ");
    if (nodeValue(node)->opCode == opE_ABOVE)
        fprintf(asmFile, "JB ");

    fprintf(asmFile, ":endCompare%d\n", curCompareNum);
    fprintf(asmFile, "PUSH 1\n");
    fprintf(asmFile, "POPREG %s\n", COMPARE_VALUE_REG);
    fprintf(asmFile, ":endCompare%d\n", curCompareNum);

    fprintf(asmFile, "PUSHREG %s\n", COMPARE_VALUE_REG);

    return errorCode;
}

int rewriteIdNodeToAsmCode(tree_t* tree, node_t* node, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    if(*nodeRight(node))
        return rewriteFuncBodyToAsmCode(tree, node, asmFile);

    const char* idName = nodeValue(node)->id.identifierName;
    size_t numOfCurNameTable = tree->nameTableStack->size - 1;
    nameTable_t* curNameTable = (tree->nameTableStack->data)[numOfCurNameTable];

    identifierInfo* searchedId = findIdInTable(curNameTable, idName);

    if (searchedId)
        return rewriteVarNodeToAsmCode (tree, node, asmFile);
    else
        return rewriteFuncCallNodeToAsmCode (tree, node, asmFile);
}

int rewriteFuncBodyToAsmCode(tree_t* tree, node_t* node, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    int errorCode = 0;

    fprintf(asmFile, ":%s\n", nodeValue(node)->id.identifierName);
    enterNewScope(tree);

    if (*nodeLeft(node))
        errorCode = fprintfGettingParamsToAsmCode(tree, *nodeLeft(node), asmFile);

    if (*nodeRight(node))
        errorCode = rewriteNodeToAsmCode(tree, *nodeRight(node), asmFile);
    else {
        printf("Error! Func body does not have RIGHT!\n");
        return 1;
    }

    exitScope(tree);

    return errorCode;
}

int fprintfGettingParamsToAsmCode (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    int errorCode = 0;

    if(*nodeLeft(node) && *nodeRight(node)) {
        errorCode = fprintfGettingParamsToAsmCode (tree, *nodeRight(node), asmFile);
        errorCode = fprintfGettingParamsToAsmCode (tree, *nodeLeft(node), asmFile);
    }
    else {
        errorCode = writeVarAddressToAsm (tree, node, asmFile);
        fprintf(asmFile, "POPM [%s]\n", VAR_ADDR_REG);
    }

    return errorCode;
}

int rewriteFuncCallNodeToAsmCode (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    int errorCode = 0;

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode (tree, *nodeLeft(node), asmFile);

    fprintf(asmFile, "CALL :%s\n", nodeValue(node)->id.identifierName);

    return errorCode;
}

int fprintfPushingCallParams (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    int errorCode = 0;

    if(*nodeLeft(node) && *nodeRight(node)) {
        errorCode = fprintfPushingCallParams (tree, *nodeRight(node), asmFile);
        errorCode = fprintfPushingCallParams (tree, *nodeLeft(node), asmFile);
    }
    else
        errorCode = rewriteVarNodeToAsmCode (tree, node, asmFile);

    return errorCode;
}

int rewriteVarNodeToAsmCode (tree_t* tree, node_t* node, FILE* asmFile) {
    assert(tree);
    assert(node);
    assert(asmFile);

    int errorCode = 0;

    errorCode = writeVarAddressToAsm (tree, node, asmFile);
    fprintf(asmFile, "PUSHM [%s]\n", VAR_ADDR_REG);

    return errorCode;
}
