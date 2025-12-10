#include <stdio.h>
#include <assert.h>
#include <strings.h>
#include <ctype.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/structAccessfunctions.h"
#include "../../COMMON/include/helpingFunctions.h"

#include "../include/lexicalAnalysis.h"

struct lexAnalysisResult* lexicalAnalysis (const char* nameOfProgramFile) {
    assert(nameOfProgramFile);

    char* bufferStart = copyFileContent(nameOfProgramFile);
    if (!bufferStart) {
        printf("Error of copying tree from file\n");
        return NULL;
    }

    FILE* dumpFile1 = fopen("DUMPS/lexDump.txt", "w"); //FIXME
    fclose(dumpFile1);


    node_t** nodeArray = (node_t**)calloc(NUM_OF_PROGRAM_NODES, sizeof(node_t*));
    size_t sizeOfNodeArr = NUM_OF_PROGRAM_NODES;

    char* bufPos = bufferStart;
    size_t curLine = 1;


    size_t numOfNodes = 0;
    for ( ; *bufPos != '\0'; numOfNodes++) {
        skipSpaces(&bufPos, &curLine);

        FILE* dumpFile = fopen("lexDump.txt", "a");
        fprintfLexAnalysisDump (dumpFile, bufPos, nodeArray, numOfNodes);
        fclose(dumpFile);

        if (numOfNodes > sizeOfNodeArr) {
            sizeOfNodeArr *= 2;
            nodeArray = (node_t**)realloc(nodeArray, sizeOfNodeArr);
            if (!nodeArray) {
                printf("ERROR realloc!\n");
                return NULL;
            }
        }

        nodeType_t tokenType = getTokenType(&bufPos);

        if (tokenType == typeError) {
            printf("ERROR! Unable to recognize token type in %s:%lld\n", nameOfProgramFile, curLine);
            return NULL;
        }

        nodeArray[numOfNodes] = processToken(&bufPos, tokenType, curLine);

        if (!(nodeArray[numOfNodes])) {
            printf("ERROR! Unable to process token in %s:%lld\n", nameOfProgramFile, curLine);
            return NULL;
        }

        skipSpaces(&bufPos, &curLine);
    }

    struct lexAnalysisResult* result = (struct lexAnalysisResult*)calloc(1, sizeof(struct lexAnalysisResult));
    result->programBuffer = bufferStart;
    result->nodesArray = nodeArray;
    result->numOfNodes = numOfNodes;

    return result;
}

nodeType_t getTokenType (char** bufPos) {
    assert(bufPos);
    assert(*bufPos);

    if (tokenIsNumber(bufPos))
        return typeNumber;

    if (tokenIsOperator(bufPos))
        return typeOperator;

    if (tokenIsIdentifier(bufPos))
        return typeIdentifier;

    return typeError;
}

int tokenIsNumber (char** bufPos) {
    assert(bufPos);
    assert(*bufPos);

    char* curPos = *bufPos;

    if (*curPos == '~')
        curPos++;

    #include "../include/numbersArray.h"

    for (int i = NUM_OF_NUMBERS - 1; i >= 0; i--)
            if (!strncmp(curPos, (numbersArray[i]).pronunciation, (numbersArray[i]).length))
                return 1;


    return 0;
}

int tokenIsOperator (char** bufPos) {
    assert(bufPos);
    assert(*bufPos);

    if (ispunct(**bufPos))
        return 1;

    #include "../../COMMON/include/operatorsArray.h"

    for (size_t i = 0; i < NUM_OF_OPERATORS; i++)
        if (!strncmp(*bufPos, ((operatorsArray[i]).opName), ((operatorsArray[i]).opLength)))
            return 1;

    return 0;
}

int tokenIsIdentifier (char** bufPos) {
    assert(bufPos);
    assert(*bufPos);

    char identifier[STR_SIZE] = {};

    if (sscanf(*bufPos, "%[a-zA-Z0-9]", identifier))
        return 1;

    return 0;
}

node_t* processToken (char** bufPos, nodeType_t tokenType, size_t curLine) {
    assert(bufPos);
    assert(*bufPos);

    switch (tokenType) {
        case typeOperator:
            return processOperator(bufPos, curLine);
        case typeNumber:
            return processNumber(bufPos, curLine);
        case typeIdentifier:
            return processIdentifier(bufPos, curLine);
        case typeError:
        default:
            return NULL;
    }

    return NULL;
}

node_t* processOperator(char** bufPos, size_t curLine) {
    assert(bufPos);
    assert(*bufPos);

    #include "../../COMMON/include/operatorsArray.h"

    operatorCode_t operCode = opUNKNOWN;

    for (size_t i = 0; i < NUM_OF_OPERATORS; i++)
        if (!strncmp(*bufPos, (operatorsArray[i]).opName, (operatorsArray[i]).opLength)) {
            operCode = (operatorsArray[i]).opCode;
            (*bufPos) += (operatorsArray[i]).opLength;
            break;
        }

    if (operCode == opUNKNOWN) {
        printf("Error! Unable to recognize an operator \"%s\"!\n", *bufPos);
        return NULL;
    }

    return newNodeCtor(typeOperator, {.opCode = operCode}, curLine);
}

node_t* newNodeCtor(nodeType_t curType, nodeValue_t curValue, size_t curLine) {

    node_t* newNode = (node_t*)calloc(1, sizeof(node_t));

    *nodeType(newNode) = curType;
    *nodeValue(newNode) = curValue;

    *nodeLine(newNode) = curLine;

    return newNode;
}

node_t* processNumber(char** bufPos, size_t curLine) {
    assert(bufPos);
    assert(*bufPos);

    #include "numbersArray.h"

    int sign = 1;
    int value = 0;

    char* startPos = *bufPos;

    if (**bufPos == '~') {
        sign = -1;
        (*bufPos)++;
    }

    while (**bufPos != ' ' && **bufPos != '!') {
        int curNum = NUM_OF_NUMBERS -1;
        for ( ; curNum >= 0; curNum--) {
                if (!strncmp(*bufPos, (numbersArray[curNum]).pronunciation, (numbersArray[curNum]).length)) {
                    value = value*10 + (numbersArray[curNum]).value;
                    (*bufPos) += (numbersArray[curNum]).length;
                    if (**bufPos == '`')
                        (*bufPos)++;
                    break;
                }
        }

        if(curNum < 0) {
            printf("Error! Unable to recognize number: %s\n", startPos);
        }
    }

    return newNodeCtor(typeNumber, {.constValue = sign*value}, curLine);
}

node_t* processIdentifier(char** bufPos, size_t curLine) {
    assert(bufPos);
    assert(*bufPos);

    char identifier[STR_SIZE] = {};
    int identifierLen = 0;

    if (sscanf(*bufPos, "%[a-zA-Z0-9]%n", identifier, &identifierLen)) {

        unsigned long long identifierHash = getStringHash(identifier);
        //node_t* newNode = newNodeCtor(typeIdentifier, {.id = {identifierHash, *bufPos}}, curLine); //FIXME
        nodeValue_t value;
        value.id.identifierHash = identifierHash;
        value.id.identifierName = strdup(identifier); // NOTE may cause problems
        node_t* newNode = newNodeCtor(typeIdentifier, value, curLine);

        (*bufPos) += identifierLen;
        //**bufPos = '\0';
        //(*bufPos)++;

        return newNode;
    }
    else {
        printf("Error! unable to get identifier: %s\n", *bufPos);
        return NULL;
    }
}

void fprintfLexAnalysisDump (FILE* dumpFile, char* bufPos, node_t** nodeArray, size_t numOfNodes) {
    assert(bufPos);
    assert(*bufPos);
    assert(dumpFile);
    assert(nodeArray);

    fprintf(dumpFile, "current buffer position: -->>%s\n", bufPos);
    fprintf(dumpFile, "number of nodes == %lld\n", numOfNodes);

    for (size_t i = 0; i < numOfNodes; i++) {
        fprintf (dumpFile, "[%lld] == ", i);
        switch ((nodeArray[i])->type) {
            case typeOperator:
                fprintf(dumpFile, "operator {%d}\n", ((nodeArray[i])->value).opCode);
                break;
            case typeNumber:
                fprintf(dumpFile, "number {%d}\n", ((nodeArray[i])->value).constValue);
                break;
            case typeIdentifier:
                fprintf(dumpFile, "identifier \"%s\" {%lld}\n", ((nodeArray[i])->value).id.identifierName,
                                                    ((nodeArray[i])->value).id.identifierHash);
                break;
            case typeError:
            default:
                break;
        }
    }

    fprintf(dumpFile, "-----------------------------------------------------------\n\n\n");
}
