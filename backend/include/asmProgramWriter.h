#ifndef ASM_PROGRAM_WRITER_H
#define ASM_PROGRAM_WRITER_H

int rewriteAstToAsmCode (tree_t* tree, const char* nameOfAsmFile,
                                       const char* sourceFileName);

int rewriteNodeToAsmCode (tree_t* tree, node_t* node, FILE* asmFile,
                          sourceFile* srcFile);

int rewriteOpNodeToAsmCode (tree_t* tree, node_t* node, FILE* asmFile,
                            sourceFile* srcFile);

int rewriteOpCalcToAsmCode (tree_t* tree, node_t* node, FILE* asmFile,
                            sourceFile* srcFile);

int rewriteOpAssignToAsmCode (tree_t* tree, node_t* node, FILE* asmFile,
                              sourceFile* srcFile);

int writeVarAddressToAsm (tree_t* tree, node_t* varNode, FILE* asmFile);

int rewriteVarNodeToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int rewriteOpWhileToAsmCode (tree_t* tree, node_t* node, FILE* asmFile,
                             sourceFile* srcFile);

int rewriteOpIfToAsmCode (tree_t* tree, node_t* node, FILE* asmFile,
                          sourceFile* srcFile);

int rewriteOpInToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int rewriteOpOutToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int rewriteOpRetToAsmCode (tree_t* tree, node_t* node, FILE* asmFile,
                           sourceFile* srcFile);

int rewriteOpCompareToAsmCode (tree_t* tree, node_t* node, FILE* asmFile,
                               sourceFile* srcFile);

int rewriteIdNodeToAsmCode (tree_t* tree, node_t* node, FILE* asmFile,
                            sourceFile* srcFile);

int rewriteFuncBodyToAsmCode (tree_t* tree, node_t* node, FILE* asmFile,
                              sourceFile* srcFile);

int fprintfGettingParamsToAsmCode (tree_t* tree, node_t* node, FILE* asmFile);

int rewriteFuncCallNodeToAsmCode (tree_t* tree, node_t* node, FILE* asmFile,
                                  sourceFile* srcFile);


#endif
