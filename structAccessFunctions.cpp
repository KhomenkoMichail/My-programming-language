#include <assert.h>
#include <stdio.h>

#include "structsAndConsts.h"
#include "structAccessFunctions.h"

node_t** nodeParent (node_t* node) {
    assert(node);

    return &(node->parent);
}

node_t** nodeLeft (node_t* node) {
    assert(node);

    return &(node->left);
}

node_t** nodeRight (node_t* node) {
    assert(node);

    return &(node->right);
}

nodeType_t* nodeType (node_t* node) {
    assert(node);

    return &(node->type);
}

nodeValue_t* nodeValue (node_t* node) {
    assert(node);

    return &(node->value);
}

node_t** treeRoot (tree_t* treePtr) {
    assert(treePtr);

    return &(treePtr->rootNode);
}

size_t* treeSize (tree_t* treePtr) {
    assert(treePtr);

    return &(treePtr->size);
}

size_t* nodeLine (node_t* node) {
    assert(node);

    return &(node->numOfLine);
}
