#ifndef REVERSE_FRONTEND_DSL_H
#define REVERSE_FRONTEND_DSL_H

#define NODE_IS_OP_(value) ((*nodeType(node) == typeOperator) && (nodeValue(node)->opCode == value))

#define OP_NAME_(opCode) (operatorsArray[opCode]).opName

#define OP_C_NAME_(opCode) (operatorsArray[opCode]).opCLangName

#define ID_NAME nodeValue(node)->id.identifierName


enum errorCode_t {
    NoErrors = 0,
    UnexpectedFuncSignNode = 1,
    CantAddFuncToNameTable = 2,
    TreeConstructionError = 3,
    CantAddVarToNameTable = 4,
    UnexpectedOperator = 5,
};

const size_t MAX_NUM_LEN = 11;



#endif
