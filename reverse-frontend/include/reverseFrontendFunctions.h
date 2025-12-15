#ifndef REVERSED_FRONTEND_FUNCTIONS_H
#define REVERSED_FRONTEND_FUNCTIONS_H

int fprintfFunctionSignatures (FILE* programFile, tree_t* tree, node_t* node);

int fprintfParamsForSignatures (FILE* programFile, node_t* node);

int fprintfAstToShiriiwookLang (const char* outputFileName, tree_t* tree);

int rewriteNodeToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent);

int rewriteNumNodeToShiriwookLang(FILE* outputFile, node_t* node);

int rewriteIdNodeToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent);

int rewriteFuncBodyToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent);

int rewriteFuncCallNodeToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent);

int rewriteVarNodeToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent);

int rewriteOpNodeToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent);

int rewriteInfixOpNodeToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent);

int rewriteAssignNodeToShiriiwookLang(FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent);

int rewriteOpIfOrWhileToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent);

int rewritePrefixOpNodeToShiriiwookLang (FILE* outputFile, tree_t* tree, node_t* node, size_t* leftIndent);

void fprintfLeftIndent (FILE* outputFile, size_t leftIndent);

int needBrackets(node_t* node);

int getOperatorPriority(operatorCode_t opCode);

#endif
