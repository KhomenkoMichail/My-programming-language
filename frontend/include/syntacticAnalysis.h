#ifndef SYNTACTIC_ANALYSIS_H
#define SYNTACTIC_ANALYSIS_H

void syntaxError (tree_t* tree, node_t** nodeArr, size_t* curNodeNum, const char* funcName);

node_t* getProgramTree (tree_t* tree, lexAnalysisResult* lexResult);

node_t* getFunction (tree_t* tree, node_t** nodeArr, size_t* curNodeNum);

node_t* getOperator (tree_t* tree, node_t** nodeArr, size_t* curNodeNum);

node_t* getOpIfOrWhile(tree_t* tree, node_t** nodeArr, size_t* curNodeNum);

node_t* getOpInOrOut(tree_t* tree, node_t** nodeArr, size_t* curNodeNum);

node_t* getOpRet(tree_t* tree, node_t** nodeArr, size_t* curNodeNum);

node_t* getOpUnited(tree_t* tree, node_t** nodeArr, size_t* curNodeNum);

node_t* getOpAssign(tree_t* tree, node_t** nodeArr, size_t* curNodeNum);

node_t* getCall(tree_t* tree, node_t** nodeArr, size_t* curNodeNum);

node_t* getExpressionNode(tree_t* tree, node_t** nodeArr, size_t* curNodeNum);

node_t* getADDandSUBnodes (tree_t* tree, node_t** nodeArr, size_t* curNodeNum);

node_t* getMULandDIVnodes (tree_t* tree, node_t** nodeArr, size_t* curNodeNum);

node_t* getBracketExpressionNodes (tree_t* tree, node_t** nodeArr, size_t* curNodeNum);

node_t* getSQRTnode(tree_t* tree, node_t** nodeArr, size_t* curNodeNum);

node_t* getVarIDNode (tree_t* tree, node_t** nodeArr, size_t* curNodeNum);

node_t* getOpInit(tree_t* tree, node_t** nodeArr, size_t* curNodeNum);

node_t* getFuncParam (tree_t* tree, node_t** nodeArr, size_t* curNodeNum);

int getFunctionsDeclarations(tree_t* tree, node_t** nodeArr, size_t* curNodeNum);

int checkAllFunctionsHaveBodies (tree_t* tree);



#endif
