#ifndef MIDDLE_END_OPTIMIZATION_H
#define MIDDLE_END_OPTIMIZATION_H

const int NOT_CONVOLUTED = 0xBADBEEF;

const int CANT_SOLVE_NODE = 0xDEAD2BAD;

#define IS_NUM(side, value) ((*nodeType(*node##side(node)) == typeNumber) && (nodeValue(*node##side(node))->constValue == value))

void simplifyTree (tree_t* tree, dump* dumpInfo);

int constConvolution (tree_t* tree, node_t* node);

int solveNode (tree_t* tree, node_t* node);

int deleteNeutralNode (tree_t* tree, node_t* node);

void makeZeroNode(tree_t* tree, node_t* node);

void deleteRightNode(tree_t* tree, node_t* node);

void deleteLeftNode(tree_t* tree, node_t* node);

#endif
