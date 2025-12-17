#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../include/structsAndConsts.h"
#include "../include/structAccessFunctions.h"
#include "../include/helpingFunctions.h"
#include "../include/treeFunctions.h"
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

            fprintf (file, "%s:%lld ", (operatorsArray[(node->value).opCode]).opCLangName, *nodeLine(node));
            break;
        }
        case typeNumber:
            fprintf (file, "%d:%lld ", (node->value).constValue, *nodeLine(node));
            break;
        case typeIdentifier:
            if ((node->value).id.identifierHash == getStringHash(MAIN_FUNCTION)) {
                if(!strcmp(MAIN_FUNCTION, (node->value).id.identifierName))
                    fprintf (file, "main:%lld ", *nodeLine(node));
            }
            else
                fprintf (file, "%s:%lld ", (node->value).id.identifierName, *nodeLine(node));
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
