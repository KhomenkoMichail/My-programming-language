#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "../../COMMON/include/structsAndConsts.h"
#include "../../COMMON/include/structAccessFunctions.h"
#include "../../COMMON/include/helpingFunctions.h"

#include "../include/sourceFileParser.h"

char* copyFileContent (sourceFile* srcFile, const char* fileName) {
    assert(fileName);
    assert(srcFile);

    int fileDescriptor = open(fileName, O_RDONLY, 0);
    if (fileDescriptor == -1) {
        fprintf(stderr, "Error of opening file \"%s\"", fileName);
        perror("");
        return NULL;
    }

    unsigned int sizeOfFile = getSizeOfFile(fileDescriptor);
    if (sizeOfFile < 1) {
        close(fileDescriptor);
        return NULL;
    }

    char* fileCopyBuffer = (char*)calloc(sizeOfFile + 1, sizeof(char));

    size_t numOfReadSymbols = read(fileDescriptor, fileCopyBuffer, sizeOfFile);
    fileCopyBuffer[numOfReadSymbols] = '\0';

    if(close(fileDescriptor) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", fileName);
        perror("");
        return NULL;
    }

    srcFile->sizeOfText = numOfReadSymbols;

    return fileCopyBuffer;
}

void getStructSourceFile (sourceFile* srcFile, const char* fileName) {
    assert(srcFile);
    assert(fileName);

    char* buffer = copyFileContent(srcFile, fileName);
    assert(buffer);

    size_t numberOfStrings    = getNumberOfSymbols(buffer, '\n');
    char** arrOfPtrsToStrings = (char**)calloc(numberOfStrings, sizeof(*arrOfPtrsToStrings));

    srcFile->text               = buffer;
    srcFile->numberOfStrings    = numberOfStrings;

    getArrOfStringStructs(srcFile);
}

void getArrOfStringStructs (sourceFile* srcFile) {
    assert(srcFile);

    srcFile->arrOfStringStructs = (line*)calloc(srcFile->numberOfStrings, sizeof(line));
    size_t line = 0;
    (srcFile->arrOfStringStructs[line]).ptrToString = srcFile->text;
    line++;

    size_t numOfCharInText = 0;
    for( ; (srcFile->text[numOfCharInText] != '\0') && (line < srcFile->numberOfStrings) ; numOfCharInText++) {

        if (srcFile->text[numOfCharInText] == '\n') {
            (srcFile->arrOfStringStructs[line]).ptrToString = srcFile->text + numOfCharInText + 1;
            line++;
        }
    }

    getLengthOfStrings(srcFile);
    replaceSymbols (srcFile->text, '\n', '\0');
}

void getLengthOfStrings (sourceFile* srcFile) {
    assert(srcFile);

    for(size_t line = 0; line < srcFile->numberOfStrings - 1; line++)
        (srcFile->arrOfStringStructs[line]).lengthOfString = (size_t)((srcFile->arrOfStringStructs[line+1]).ptrToString - (srcFile->arrOfStringStructs[line]).ptrToString);
}

void freeStructSourceFile (sourceFile* srcFile) {
    assert(srcFile);

    free(srcFile->text);
    free(srcFile->arrOfStringStructs);
    free(srcFile);
}

size_t getNumberOfSymbols (char* text, char searchedSymbol) {
    assert(text);

    size_t numOfSymbolsFound = 0;
    for(size_t numOfChar = 0; text[numOfChar] != '\0'; numOfChar++)
        if (text[numOfChar] == searchedSymbol)
            numOfSymbolsFound++;

    return numOfSymbolsFound;
}

void replaceSymbols (char* text, char targetSymbol, char replacementSymbol) {
    assert(text);

    for(size_t numOfChar = 0; text[numOfChar] != '\0'; numOfChar++)
        if (text[numOfChar] == targetSymbol)
            text[numOfChar] = replacementSymbol;

    return;
}

void fprintfCommentsToAsm (node_t* node, sourceFile* srcFile, FILE* asmFile) {
    assert(node);
    assert(srcFile);
    assert(asmFile);

    if (*nodeType(node) == typeOperator && nodeValue(node)->opCode == opSEPARATOR)
        return;

    node_t* parentNode = *nodeParent(node);

    if (parentNode)
        if (!(*nodeType(parentNode) == typeOperator && nodeValue(parentNode)->opCode == opSEPARATOR))
            if (*nodeLine(node) == *nodeLine(parentNode))
                return;

    int numOfCurNodeLine = (int)(*nodeLine(node)) - 1;
    int numOfFirstLine = numOfCurNodeLine - NUM_OF_CLOSEST_LINES;
    int numOfLastLine = numOfCurNodeLine + NUM_OF_CLOSEST_LINES;

    fprintf(asmFile, "\n\n;-----------------------------------------------------------\n");
    fprintf(asmFile, ";  %s:%d\n", srcFile->fileName, (int)(*nodeLine(node)));
    fprintf(asmFile, ";-----------------------------------------------------------\n");

    for (int numOfLine = numOfFirstLine; numOfLine < numOfCurNodeLine; numOfLine++)
        if(numOfLine >= 0)
            fprintf(asmFile, ";         %s\n", ((srcFile->arrOfStringStructs)[numOfLine]).ptrToString);

    fprintf(asmFile, ";     ==> %s\n", ((srcFile->arrOfStringStructs)[numOfCurNodeLine]).ptrToString);

    for (size_t numOfLine = numOfCurNodeLine + 1; (int)numOfLine <= numOfLastLine; numOfLine++)
        if(numOfLine < srcFile->numberOfStrings)
            fprintf(asmFile, ";         %s\n", ((srcFile->arrOfStringStructs)[numOfLine]).ptrToString);
    fprintf(asmFile, ";-----------------------------------------------------------\n");


    fprintf(asmFile, "\n\n");
}
