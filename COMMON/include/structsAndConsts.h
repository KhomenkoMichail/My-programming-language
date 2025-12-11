#ifndef STRUCTS_AND_CONSTS_H
#define STRUCTS_AND_CONSTS_H

#include <cstddef>
#include <sys/types.h>

#include "nameTableConsts.h"

const size_t NUM_OF_PROGRAM_NODES = 128;

const size_t NUM_OF_IDENTIFIERS = 256;

const size_t MAX_OP_NAME_LEN = 5;

const size_t NODE_DESCRIPTION_SIZE = 64;

const size_t MAX_TREE_SIZE = 500;

const size_t COMMAND_LENGTH = 512;

const size_t STR_SIZE = 64;

#define MAIN_FUNCTION "atti"

struct shyriiwookNum {
    const char* pronunciation;
    size_t length;
    int value;
};

enum nodeType_t {
    typeOperator = 1,
    typeIdentifier = 2,
    typeNumber = 3,
    typeError = 0,
};

enum operatorCode_t {
    opUNKNOWN = -1,
    opADD = 0,
    opSUB = 1,
    opMUL = 2,
    opDIV = 3,
    opSEPARATOR = 4,
    opBRACK_ON = 5,
    opBRACK_OFF = 6,
    opASSIGN = 7,
    opWHILE = 8,
    opIF = 9,
    opIN = 10,
    opOUT = 11,
    opRET = 12,
    opHLT = 13,
    opSQRT = 14,
    opCOMMA = 15,
    opUNITED_ON = 16,
    opUNITED_OFF = 17,
    opQUOTES = 18,
    opEQUAL = 19,
    opBELOW = 20,
    opABOVE = 21,
    opNOT_EQUAL = 22,
    opE_BELOW = 23,
    opE_ABOVE = 24,
    opINIT = 25,
};

union nodeValue_t {
    operatorCode_t opCode;
    int constValue;

    struct identifierInfo id;
};

struct node_t {
    nodeType_t type;
    nodeValue_t value;

    size_t numOfLine;

    node_t* parent;

    node_t* left;
    node_t* right;
};

struct lexAnalysisResult {
    char* programBuffer;

    node_t** nodesArray;
    size_t numOfNodes;
};

struct tree_t {
    node_t* rootNode;
    size_t size;

    stack_t* nameTableStack;
    size_t currentScopeLevel;

    int errorCode;
};

struct dump {
    const char* nameOfFile;
    const char* nameOfFunc;
    unsigned int numOfLine;

    const char* nameOfDumpFile;
    const char* nameOfGraphFile;
    int dumpFileWasOpened;

};

enum treeErr_t {
    noErrors = 0x00,
    badLeft = 0x02,
    badRight = 0x04,
    tooManyRecursiveCalls = 0x08,
    treeSyntaxError = 0x10,
    treeNameTableError = 0x20,
};

struct operatorInfo {
    const char* opName;
    operatorCode_t opCode;
    size_t opLength;

    const char* opCLangName;
    const char* dumpColor;
};

#endif
