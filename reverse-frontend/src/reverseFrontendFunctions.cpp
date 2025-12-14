#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/structAccessFunctions.h"
#include "../../COMMON/include/treeFunctions.h"
#include "../../COMMON/include/nameTableConsts.h"
#include "../../COMMON/include/nameTableStack.h"

#include "../include/reverseFrontendFunctions.h"
#include "../include/reverseFrontendDSL.h"

int fprintfFunctionSignatures (FILE* programFile, tree_t* tree, node_t* node) {
    assert(programFile);
    assert(tree);
    assert(node);

    int errorCode = NoErrors;

    if (NODE_IS_OP_(opSEPARATOR)) {
        if (*nodeLeft(node))
            errorCode = fprintfFunctionSignatures(programFile, *nodeLeft(node));

        if (*nodeRight(node))
            errorCode = fprintfFunctionSignatures(programFile, *nodeRight(node));
    }
    else if (*nodeType(node) == typeIdentifier) {
        if (strcmp(ID_NAME, "main")) {
            #include "../../COMMON/include/operatorsArray.h"
            (void)NUM_OF_OPERATORS;

            fprintf(programFile, "%s %s", OP_NAME_(opINIT), ID_NAME);
            fprintf(programFile, "%s", OP_NAME_(opBRACK_ON));

            identifierInfo* newFunc = addIdToCurrentScope(tree, ID_NAME, idFUNC);
            if (!newFunc) {
                printf("Error of adding func signature to name table!\n");
                errorCode = CantAddFuncToNameTable;
            }

            if (*nodeLeft(node))
                errorCode = fprintfParamsForSignatures(programFile, *nodeLeft(node));

            fprintf(programFile, "%s%s\n\n", OP_NAME_(opBRACK_OFF), OP_NAME_(opSEPARATOR));
        }
    }
    else
        errorCode = UnexpectedFuncSignNode;

    return errorCode;
}

int fprintfParamsForSignatures (FILE* programFile, node_t* node) {
    assert(programFile);
    assert(node);

    int errorCode = NoErrors;

    if (NODE_IS_OP_(opCOMMA)) {
        if (*nodeLeft(node))
            errorCode = fprintfParamsForSignatures(programFile, *nodeLeft(node));

        fprintf(programFile, ", ");

        if (*nodeRight(node))
            errorCode = fprintfParamsForSignatures(programFile, *nodeRight(node));
    }
    else if (*nodeType(node) == typeIdentifier)
        fprintf(programFile, "%s", ID_NAME);
    else
        errorCode = UnexpectedFuncSignNode;

    return errorCode;
}

int fprintfAstToShiriiwookLang (const char* outputFileName, tree_t* tree) {
    assert(outputFileName);
    assert(tree);

    FILE* outputFile = fopen(outputFileName, "w");

    if (!outputFile) {
        fprintf(stderr, "Error of opening file \"%s\"", outputFileName);
        perror("");
        return 1;
    }

    initNameTables(tree);

    if (fprintfFunctionSignatures(outputFile, tree, *treeRoot(tree))) {
        printf("Error of getting functions signatures!\n");
        return UnexpectedFuncSignNode;
    }

    int leftIndent = 0;

    if (rewriteNodeToShiriwookLang (outputFile, tree, *treeRoot(tree), &leftIndent)) {
        printf("Error of rewriting AST to shiriiWook.\n");
        return 1;
    }


    if (fclose(outputFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", outputFileName);
        perror("");
        return 1;
    }

    return 0;
}

int rewriteNodeToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, int* leftIndent) {
    assert(outputFile);
    assert(tree);
    assert(node);
    assert(leftIndent);

    switch (*nodeType(node)) {
        case typeNumber:
            return rewriteNumNodeToShiriwookLang(outputFile, node);
        case typeOperator:
            return rewriteOpNodeToShiriiwookLang(outputFile, tree, node, leftIndent);
        case typeIdentifier:
            return rewriteIdNodeToShiriiwookLang(outputFile, tree, node, leftIndent);
        case typeError:
        default:
            return 1;
    }
}

int rewriteNumNodeToShiriwookLang(FILE* outputFile, node_t* node) {
    assert(outputFile);
    assert(node);

    char numberString[MAX_NUM_LEN];
    sprintf(numberString, "%d", nodeValue(node)->constValue);

    #include "../../COMMON/include/numbersArray.h"

    for (size_t i = 0; numberString[i]; i++) {
        if(i != 0)
            fprintf(outputFile, "`");

        if(numberString[i] == '-') {
            fprintf(outputFile, "~");
            i++;
        }

        fprintf(outputFile, "%s", numbersArray[numberString[i] - '0']);
    }

    return 0;
}


int rewriteIdNodeToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent) {
    assert(outputFile);
    assert(tree);
    assert(node);
    assert(leftIndent);

    if(*nodeRight(node))
        return rewriteFuncBodyToShiriiwookLang(outputFile, tree, node, leftIndent);

    const char* idName = nodeValue(node)->id.identifierName;

    identifierInfo* searchedId = findIdInAllScopes(tree, idName);

    if (!searchedId)
        return rewriteVarNodeToShiriiwookLang(outputFile, tree, node, leftIndent);
    else if (searchedId->idType == idFUNC)
        return rewriteFuncCallNodeToShiriiwookLang(outputFile, tree, node, leftIndent);
    else return rewriteVarNodeToShiriiwookLang(outputFile, tree, node, leftIndent);
}

int rewriteFuncBodyToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent) {
    assert(outputFile);
    assert(tree);
    assert(node);
    assert(leftIndent);

    int errorCode = NoErrors;

    #include"../../COMMON/include/operatorsArray.h"
    (void)NUM_OF_OPERATORS;

    fprintf(outputFile, "%s%s", ID_NAME, OP_NAME_(opBRACK_ON));

    enterNewScope(tree);

    if (*nodeLeft(node))
        errorCode = rewriteNodeToShiriiwookLang(outputFile, tree, *nodeLeft(node), leftIndent);
    fprintf(outputFile, "%s %s\n", OP_NAME_(opBRACK_OFF), OP_NAME_(opUNITED_ON));

    (*leftIndent)++;
    fprintfLeftIndent(outputFile, *leftIndent);

    if (*nodeRight(node))
        errorCode = rewriteNodeToShiriiwookLang(outputFile, tree, *nodeRight(node), leftIndent);
    else {
        printf("Error! Func body does not have RIGHT!\n");
        return TreeConstructionError;
    }

    (*leftIndent)--;
    exitScope(tree);

    fprintf(outputFile, "%s", OP_NAME_(opUNITED_OFF));

    return errorCode;
}

void fprintfLeftIndent (FILE* outputFile, size_t leftIndent) {
    assert(outputFile);

    for (int i = 0; i < leftIndent; i++)
        fprintf(outputFile, "\t")
}

int rewriteFuncCallNodeToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent) {
    assert(outputFile);
    assert(tree);
    assert(node);
    assert(leftIndent);

    int errorCode = 0;

    fprintf(outputFile, "%s%s", ID_NAME, OP_NAME_(opBRACK_ON));

    if (*nodeLeft(node))
        errorCode = rewriteNodeToAsmCode (tree, *nodeLeft(node), asmFile);

    fprintf(outputFile, "%s", OP_NAME_(opBRACK_OFF))

    return errorCode;
}

int rewriteVarNodeToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent) {
    assert(outputFile);
    assert(tree);
    assert(node);
    assert(leftIndent);

    int errorCode = NoErrors;

    const char* idName = nodeValue(node)->id.identifierName;
    size_t numOfCurNameTable = tree->nameTableStack->size - 1;
    nameTable_t* curNameTable = (tree->nameTableStack->data)[numOfCurNameTable];

    identifierInfo* searchedId = findIdInTable(curNameTable, idName);

    if (!searchedId) {
        identifierInfo* newVar = addIdToCurrentScope(tree, ID_NAME, idVAR);
            if (!newVar) {
                printf("Error of adding variable to name table!\n");
                errorCode = CantAddVarToNameTable;
            }
    }

    fprintf(outputFile, "%s", ID_NAME);

    return errorCode;
}

int rewriteOpNodeToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent) {
    assert(outputFile);
    assert(tree);
    assert(node);
    assert(leftIndent);

    switch (nodeValue(node)->opCode) {
        case opADD:
        case opSUB:
        case opMUL:
        case opDIV:
        case opSEPARATOR:
        case opCOMMA:
        case opEQUAL:
        case opBELOW:
        case opABOVE:
        case opNOT_EQUAL:
        case opE_BELOW:
        case opE_ABOVE:
            return rewriteInfixOpNodeToShiriiwookLang(outputFile, tree, node, leftIndent);

        case opASSIGN:
            return rewriteAssignNodeToShiriiwookLang(outputFile, tree, node, leftIndent);

        case opWHILE:
        case opIF:
            return rewriteOpIfOrWhileToShiriiwookLang (outputFile, tree, node, leftIndent);

        case opIN:
        case opOUT:
        case opRET:
        case opHLT:
        case opSQRT:
            return rewritePrefixOpNodeToShiriiwookLang (outputFile, tree, node, leftIndent);

        case opBRACK_ON:
        case opBRACK_OFF:
        case opUNITED_ON:
        case opUNITED_OFF:
        case opQUOTES:
        case opINIT:
        default:
            printf("Error! Unexpected operator in AST!\n");
            return UnexpectedOperator;
    }
}

int rewriteInfixOpNodeToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent) {
    assert(outputFile);
    assert(tree);
    assert(node);
    assert(leftIndent);

    int errorCode = NoErrors;
    operatorCode_t curOp = nodeValue(node)->opCode;

    #include "../../COMMON/include/operatorsArray.h"
    (void)NUM_OF_OPERATORS;

    if (*nodeLeft(node))
        errorCode = rewriteNodeToShiriiwookLang (outputFile, tree, *nodeLeft(node), leftIndent);
    else
        return printf("Error! Infix op {%s} does not have LEFT!\n", OP_C_NAME_(curOp)), TreeConstructionError;

    if (curOp == opSEPARATOR) {
        fprintf(outputFile, "%s\n", OP_NAME_(curOp));
        fprintfLeftIndent(outputFile, *leftIndent);
    }
    else if (curOp == opCOMMA)
        fprintf(outputFile, "%s ", OP_NAME_(curOp));
    else
        fprintf(outputFile, " %s ", OP_NAME_(curOp));

    if (*nodeRight(node))
        errorCode = rewriteNodeToShiriiwookLang (outputFile, tree, *nodeRight(node), leftIndent);
    else
        return printf("Error! Infix op {%s} does not have LEFT!\n", OP_C_NAME_(curOp)), TreeConstructionError;

    return errorCode;
}

int rewriteAssignNodeToShiriiwookLang(FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent) {
    assert(outputFile);
    assert(tree);
    assert(node);
    assert(leftIndent);

    int errorCode = NoErrors;

    #include "../../COMMON/include/operatorsArray.h"
    (void)NUM_OF_OPERATORS;

    if (*nodeLeft(node)) {
        node_t* varNode = *nodeLeft(node);
        const char* varName = nodeValue(varNode)->id.identifierName;
        identifierInfo* searchedVar = findIdInAllScopes(tree, varName);

        if (!searchedVar)
            fprintf("%s ", OP_NAME_(opINIT));

        errorCode = rewriteVarNodeToShiriiwookLang(outputFile, tree, *nodeLeft(node), leftIndent);
    }
    else
        return printf("Error! Infix op {%s} does not have LEFT!\n", OP_C_NAME_(curOp)), TreeConstructionError;

    fprintf(" %s ", OP_NAME_(opASSIGN));

    if (*nodeRight(node))
        errorCode = rewriteNodeToShiriiwookLang (outputFile, tree, *nodeRight(node), leftIndent);
    else
        return printf("Error! Infix op {%s} does not have LEFT!\n", OP_C_NAME_(curOp)), TreeConstructionError;

    return errorCode;
}

int rewriteOpIfOrWhileToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent) {
    assert(outputFile);
    assert(tree);
    assert(node);
    assert(leftIndent);

    int errorCode = NoErrors;

    #include "../../COMMON/include/operatorsArray.h"
    (void)NUM_OF_OPERATORS;

    operatorCode_t curOp = nodeValue(node)->opCode;

    fprintf(outputFile, "%s %s", OP_NAME_(curOP), OP_NAME(opBRACK_ON));

    if (*nodeLeft(node))
        errorCode = rewriteNodeToShiriiwookLang (outputFile, tree, *nodeLeft(node), leftIndent);
    else
        return printf("Error! Operator {%s} does not have LEFT!\n", OP_C_NAME_(curOp)), TreeConstructionError;

    fprintf(outputFile, "%s", OP_NAME_(opBRACK_OFF));

    (*leftIndent)++;

    if (*nodeRight(node)) {
        node_t* rightNode = *nodeRight(node);

        if ((*nodeType(rightNode) == typeOperator) && (nodeValue(rightNode)->opCode == opSEPARATOR))
            fprintf(outputFile, " %s", OP_NAME_(opUNITED_ON));

        fprintf(outputFile, "\n");
        fprintfLeftIndent(outputFile, *leftIndent);

        errorCode = rewriteNodeToShiriiwookLang (outputFile, tree, *nodeRight(node), leftIndent);
        (*leftIndent)--;

        if ((*nodeType(rightNode) == typeOperator) && (nodeValue(rightNode)->opCode == opSEPARATOR))
            fprintf(outputFile, "\b\b\b\b%s", OP_NAME_(opUNITED_OFF)); //FIXME
    }//NOTE problem place
    else
        return printf("Error! Operator {%s} does not have RIGHT!\n", OP_C_NAME_(curOp)), TreeConstructionError;

    return errorCode;
}

int rewritePrefixOpNodeToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent) {
    assert(outputFile);
    assert(tree);
    assert(node);
    assert(leftIndent);

    int errorCode = NoErrors;

    #include "../../COMMON/include/operatorsArray.h"
    (void)NUM_OF_OPERATORS;

    operatorCode_t curOp = nodeValue(node)->opCode;

    fprintf(outputFile, "%s ", OP_NAME_(curOp));

    if ((curOp == opIN) || (curOp == opOUT))
        fprintf(outputFile, "%s", OP_NAME_(opQUOTES));

    if (curOp == opSQRT)
        fprintf(outputFile, "%s", OP_NAME_(opBRACK_ON));

    if (*nodeLeft(node))
        errorCode = rewriteNodeToShiriiwookLang (outputFile, tree, *nodeLeft(node), leftIndent);

    if ((curOp == opIN) || (curOp == opOUT))
        fprintf(outputFile, "%s", OP_NAME_(opQUOTES));

    if (curOp == opSQRT)
        fprintf(outputFile, "%s", OP_NAME_(opBRACK_OFF));

    return errorCode;
}
