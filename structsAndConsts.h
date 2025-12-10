#ifndef STRUCTS_AND_CONSTS_H
#define STRUCTS_AND_CONSTS_H

#include "nameTableConsts.h"

const size_t NUM_OF_PROGRAM_NODES = 128;

const size_t NUM_OF_IDENTIFIERS = 256;

const size_t MAX_OP_NAME_LEN = 5;

const size_t NODE_DESCRIPTION_SIZE = 64;

const size_t MAX_TREE_SIZE = 500;

const size_t COMMAND_LENGTH = 512;

const size_t STR_SIZE = 64;

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
    opUNKNOWN = 0,
    opADD = 1,
    opSUB = 2,
    opMUL = 3,
    opDIV = 4,
    opSEPARATOR = 5,
    opBRACK_ON = 6,
    opBRACK_OFF = 7,
    opASSIGN = 8,
    opWHILE = 9,
    opIF = 10,
    opIN = 11,
    opOUT = 12,
    opRET = 13,
    opHLT = 14,
    opSQRT = 15,
    opCOMMA = 16,
    opUNITED_ON = 18,
    opUNITED_OFF = 19,
    opQUOTES = 20,
    opEQUAL = 21,
    opBELOW = 22,
    opABOVE = 23,
    opNOT_EQUAL = 24,
    opE_BELOW = 25,
    opE_ABOVE = 26,
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
};

struct operatorInfo {
    const char* opName;
    operatorCode_t opCode;
    size_t opLength;

    const char* opDumpName;
    const char* dumpColor;
};

#endif
