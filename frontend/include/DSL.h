#ifndef DSL_H
#define DSL_H

#define NODE_IS_OP_(opNameCode) ((nodeArr[*curNodeNum])->type == typeOperator && ((nodeArr[*curNodeNum])->value).opCode == opNameCode)

#define NODE_IS_OP (nodeArr[*curNodeNum])->type == typeOperator

#define NODE_IS_ID ((nodeArr[*curNodeNum])->type == typeIdentifier)

#define NODE_IS_COMPARE (NODE_IS_OP_(opEQUAL) || NODE_IS_OP_(opBELOW) || NODE_IS_OP_(opABOVE)|| NODE_IS_OP_(opNOT_EQUAL) || NODE_IS_OP_(opE_BELOW) || NODE_IS_OP_(opE_ABOVE))

#define NODE_IS_NUM (nodeArr[*curNodeNum])->type == typeNumber

#define CHECK_THE_NODE_IS_(opNameCode)  if (NODE_IS_OP_(opNameCode))\
                                            (*curNodeNum)++;\
                                        else {\
                                            syntaxError(tree, nodeArr, curNodeNum, __func__);\
                                            return NULL;\
                                        }\



#endif
