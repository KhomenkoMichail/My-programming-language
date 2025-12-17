#ifndef SOURCE_FILE_PARSER_H
#define SOURCE_FILE_PARSER_H

struct line {
    char* ptrToString;
    size_t lengthOfString;
};

struct sourceFile {
    char* text;
    struct line* arrOfStringStructs;
    size_t sizeOfText;
    size_t numberOfStrings;

    const char* fileName;
};

const int NUM_OF_CLOSEST_LINES = 2;

char* copyFileContent (sourceFile* srcFile, const char* fileName);

void getStructSourceFile (sourceFile* srcFile, const char* fileName);

void getArrOfStringStructs (sourceFile* srcFile);

void getLengthOfStrings (sourceFile* srcFile);

void freeStructSourceFile (sourceFile* srcFile);

size_t getNumberOfSymbols (char* text, char searchedSymbol);

void replaceSymbols (char* text, char targetSymbol, char replacementSymbol);

void fprintfCommentsToAsm (node_t* node, sourceFile* srcFile, FILE* asmFile);

#endif
