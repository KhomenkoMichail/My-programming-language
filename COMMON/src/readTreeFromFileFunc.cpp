#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "../include/structsAndConsts.h"
#include "../include/helpingFunctions.h"
#include "../include/treeFunctions.h"
#include "../include/nameTableConsts.h"
#include "../include/nameTableStack.h"
#include "../include/structAccessFunctions.h"
#include "../include/readTreeFromFileFunc.h"

char* readFileAndCreateTree (tree_t* tree, dump* dumpInfo, const char* nameOfFile) {
    assert(tree);
    assert(dumpInfo);
    assert(nameOfFile);

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
        return NULL;
    }

    char* bufferStart = copyFileContent(nameOfFile);
    char* bufferForFree = bufferStart;

    if (bufferStart == NULL) {
        printf("Error of copying tree from file\n");
        return NULL;
    }
    else
        *treeRoot(tree) = nodeCtorByReadBuffer(&bufferStart, tree, NULL, dumpInfo, dumpFile);

    if (fclose(dumpFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", dumpInfo->nameOfDumpFile);
        perror("");
        return NULL;
    }

    treeDump(tree, dumpInfo, "Вот созданное финальное дерево:");

    return bufferForFree;
}

node_t* nodeCtorByReadBuffer(char** bufPos, tree_t* tree, node_t* parentNode, dump* dumpInfo, FILE* dumpFile) {
    assert(bufPos);
    assert(dumpInfo);
    assert(tree);
    assert(dumpFile);

    DUMP_MESSAGE(dumpFile, "Зашла в функцию создания узла\n", *bufPos);
    skipSpaces(bufPos, NULL);
    if(**bufPos == '(') {
        *treeSize(tree) += 1;
        (*bufPos)++;
        skipSpaces(bufPos, NULL);

        DUMP_MESSAGE(dumpFile, "Прочитала (\n", *bufPos);

        node_t* newNode = (node_t*)calloc(1, sizeof(node_t));

        *nodeType(newNode) = getNodeType(bufPos);
        processNodeType(tree, newNode, bufPos);

        *(nodeParent(newNode)) = parentNode;

        DUMP_MESSAGE(dumpFile, "Прочитала имя узла.\n", *bufPos);

        DUMP_MESSAGE(dumpFile, "<h3>Сейчас зайду в левое поддерево.\n</font></h3>", *bufPos);
        *nodeLeft(newNode) = nodeCtorByReadBuffer(bufPos, tree, newNode, dumpInfo, dumpFile);


        DUMP_MESSAGE(dumpFile, "<h3>Сейчас зайду в правое поддерево.\n</font></h3>", *bufPos);
        *nodeRight(newNode) = nodeCtorByReadBuffer(bufPos, tree, newNode, dumpInfo, dumpFile);


        skipSpaces(bufPos, NULL);
        (*bufPos)++;

        DUMP_MESSAGE(dumpFile, "Прочитала ).\n", *bufPos);

        return newNode;
    }

    if (strncmp(*bufPos, "nil", 3) == 0) {
        DUMP_MESSAGE(dumpFile, "Нашла nil.\n", *bufPos);
        (*bufPos) += 4; //FIXME
        DUMP_MESSAGE(dumpFile, "Прочитала nil.\n", *bufPos);
        return NULL;
    }

    return NULL;
}

nodeType_t getNodeType(char** bufPos) {
    assert(bufPos);
    assert(*bufPos);

    if (isdigit(**bufPos))
        return typeNumber;

    if (**bufPos == '-') {
        if (isdigit(*(*bufPos + 1)))
            return typeNumber;
        else
            return typeOperator;
    }

    char operatorName[MAX_OP_NAME_LEN] = {};
    sscanf(*bufPos, "%64s", operatorName);

    #include "../../COMMON/include/operatorsArray.h"
    for (size_t numOfOp = 0; numOfOp < NUM_OF_OPERATORS; numOfOp++)
        if (!strcmp((operatorsArray[numOfOp]).opCLangName, operatorName))
             return typeOperator;

    return typeIdentifier;
}

int processNodeType (tree_t* tree, node_t* node, char** bufPos) {
    assert(tree);
    assert(bufPos);
    assert(*bufPos);
    assert(node);

    int lenOfValue = 0;
    char valueString[STR_SIZE] = {};

    switch (*nodeType(node)) {
        case typeNumber:
            sscanf(*bufPos, "%64s%n", valueString, &lenOfValue);
            (*bufPos) += lenOfValue;

            (nodeValue(node))->constValue = (int)atof(valueString);
            break;

        case typeIdentifier: {
            sscanf(*bufPos, "%*s%n", &lenOfValue);
            (nodeValue(node))->id.identifierName = *bufPos;

            (*bufPos) += lenOfValue;
            (**bufPos) = '\0';
            translateRussianWorlds ((nodeValue(node))->id.identifierName); //NOTE
            (*bufPos)++;
            (nodeValue(node))->id.identifierHash = getStringHash((nodeValue(node))->id.identifierName);

            break;
        }
        case typeOperator: {
            sscanf(*bufPos, "%64s%n", valueString, &lenOfValue);
            (*bufPos) += lenOfValue;

            #include "../../COMMON/include/operatorsArray.h"
            int curOpCode = -1;

            for (size_t numOfOp = 0; numOfOp < NUM_OF_OPERATORS; numOfOp++)
                if (strncmp((operatorsArray[numOfOp]).opCLangName, valueString, MAX_OP_NAME_LEN) == 0) {
                    curOpCode = (operatorsArray[numOfOp]).opCode;
                    break;
                }

            if (curOpCode == -1) {
                printf("ERROR! Unexpected operator name \"%s\"\n", valueString);
                return 1;
            }

            (nodeValue(node))->opCode = (operatorCode_t)curOpCode;
            break;
        }
        case typeError:
        default:
            break;
    }

    return 0;
}


char* translateRussianWorlds (char* str) {
    assert(str);

    if (str[0] - 'А' < 0 || str[0] - 'А' > 63)
        return str;

    char rusLetters[64] = {'A', 'B', 'V', 'G', 'D', 'E', 'J', 'Z', 'I', 'I', 'K',
                           'L', 'M', 'N', 'O', 'P', 'R', 'S', 'T', 'U', 'F', 'H',
                           'C', 'C', 'S', 'S', 'I', 'I', 'I' ,'E', 'U', 'A', 'a',
                           'b', 'v', 'g', 'd', 'e', 'j', 'z', 'i', 'i', 'k', 'l',
                           'm', 'n', 'o', 'p', 'r', 's', 't', 'u', 'f', 'h', 'c',
                           'c', 's', 's', 'i', 'i', 'i','e', 'u', 'a'};

    size_t stringLenth = strlen(str);

    for (size_t i = 0; i < stringLenth; i++)
        str[i] = rusLetters[(str[i]) - 'А'];

    return str;
}
