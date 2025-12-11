#ifndef SAVE_TREE_IN_FILE_H
#define SAVE_TREE_IN_FILE_H

int saveTreeInFile (tree_t* tree, const char* nameOfSaveFile);

void fprintfNode(node_t* node, FILE* file);

void endFrontendProgram (tree_t* tree, lexAnalysisResult* lexResult);

#endif
