#ifndef TREE_FUNCTIONS_H
#define TREE_FUNCTIONS_H

int fprintfTreeGraphDump (tree_t* tree, const char* textGraphFileName);

int fprintfNodeGraph (node_t* node, int rank, FILE* graphFile, size_t* nodesPassed, size_t treeSize);

int fprintfNodeLinksForGraph (node_t* node, FILE* graphFile, size_t* nodesPassed, size_t treeSize);

void treeDump (struct tree_t* tree, struct dump* dumpInfo, const char* message);

void createGraphImageForDump (struct tree_t* tree, FILE* dumpFile, const char* nameOfTextGraphFile);

int nodeVerifier (node_t* node, int* errorCode, size_t* nodesPassed, size_t treeSize);

void fprintfTreeErrorsForDump (struct tree_t* tree, FILE* dumpFile, struct dump* dumpInfo);

int treeVerifier (tree_t* tree);

int deleteTree (tree_t* tree);

int deleteNode(tree_t* tree, node_t* node);

#endif
