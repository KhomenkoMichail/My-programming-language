#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/structAccessFunctions.h"
#include "../../COMMON/include/helpingFunctions.h"
#include "../../COMMON/include/treeFunctions.h"

#include "../include/saveTreeInFile.h"

int saveTreeInFile (tree_t* tree, const char* nameOfSaveFile) {
    assert(tree);
    assert(nameOfSaveFile);

    FILE* saveFile = fopen(nameOfSaveFile, "w");

    if (saveFile == NULL) {
        fprintf(stderr, "Error of opening file \"%s\"", nameOfSaveFile);
        perror("");
        return 1;
    }

    fprintfNode(*treeRoot(tree), saveFile);

    if (fclose(saveFile) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", nameOfSaveFile);
        perror("");
        return 1;
    }

    return 0;
}

void fprintfNode(node_t* node, FILE* file) {
    assert(node);
    assert(file);

    fprintf(file, "( ");

    switch (*nodeType(node)) {
        case typeOperator: {
            #include "../../COMMON/include/operatorsArray.h"
            (void)NUM_OF_OPERATORS;

            fprintf (file, "\"%s\" ", (operatorsArray[(node->value).opCode]).opCLangName);
            break;
        }
        case typeNumber:
            fprintf (file, "\"%d\" ", (node->value).constValue);
            break;
        case typeIdentifier:
            if ((node->value).id.identifierHash == getStringHash(MAIN_FUNCTION)) {
                if(!strcmp(MAIN_FUNCTION, (node->value).id.identifierName))
                    fprintf (file, "\"main\" ");
            }
            else
                fprintf (file, "\"%s\" ", (node->value).id.identifierName);
            break;
        case typeError:
        default:
            fprintf(file, "nil ");
    }

    if(*nodeLeft(node) != NULL)
        fprintfNode(*nodeLeft(node), file);
    else
        fprintf(file, "nil ");

    if(*nodeRight(node) != NULL)
        fprintfNode(*nodeRight(node), file);
    else
        fprintf(file, "nil ");

    fprintf(file, ") ");
}

void endFrontendProgram (tree_t* tree, lexAnalysisResult* lexResult) {
    assert(tree);
    assert(lexResult);

    deleteTree (tree);
    free(lexResult->programBuffer);
    free(lexResult->nodesArray);
}
