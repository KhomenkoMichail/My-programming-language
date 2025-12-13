#ifndef ASM_PROGRAM_WRITER_H
#define ASM_PROGRAM_WRITER_H

int rewriteAstToAsmCode (tree_t* tree, const char* nameOfAsmFile);

int rewriteNodeToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int rewriteOpNodeToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int rewriteOpCalcToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int rewriteOpAssignToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int writeVarAddressToAsm (tree_t* tree, node_t* varNode, FILE* asmFile);

int rewriteVarNodeToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int rewriteOpWhileToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int rewriteOpIfToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int rewriteOpInToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int rewriteOpOutToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int rewriteOpRetToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int rewriteOpCompareToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int rewriteIdNodeToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int rewriteFuncBodyToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int fprintfGettingParamsToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int fprintfPushingCallParams (tree_t* tree, node_t* node, FILE* asmFile);

int rewriteFuncCallNodeToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);


#endif
