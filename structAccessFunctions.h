#ifndef STRUCT_ACCESS_FUNCTIONS_H
#define STRUCT_ACCESS_FUNCTIONS_H

node_t** nodeParent (node_t* node);

node_t** nodeLeft (node_t* node);

node_t** nodeRight (node_t* node);

nodeType_t* nodeType (node_t* node);

nodeValue_t* nodeValue (node_t* node);

node_t** treeRoot (tree_t* treePtr);

size_t* treeSize (tree_t* treePtr);

size_t* nodeLine (node_t* node);

#endif
