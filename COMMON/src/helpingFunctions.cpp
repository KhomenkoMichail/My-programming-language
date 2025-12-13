#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>

#include "../include/structsAndConsts.h"
#include "../include/helpingFunctions.h"

char* copyFileContent (const char* nameOfFile) {
    assert(nameOfFile);

    int fileDescriptor = open(nameOfFile, O_RDONLY, 0);
    if (fileDescriptor == -1) {

        fprintf(stderr, "Error of opening file \"%s\"", nameOfFile);
        perror("");
        return NULL;
    }

    unsigned int sizeOfFile = getSizeOfFile(fileDescriptor);
    if (sizeOfFile == 0) {
        close(fileDescriptor);
        return NULL;
    }

    char* fileCopyBuffer = (char*)calloc(sizeOfFile + 1, sizeof(char));

    size_t numOfReadSymbols = read(fileDescriptor, fileCopyBuffer, sizeOfFile);
    fileCopyBuffer[numOfReadSymbols] = '\0';

    if(close(fileDescriptor) != 0) {
        fprintf(stderr, "Error of closing file \"%s\"", nameOfFile);
        perror("");
        return NULL;
    }

    return fileCopyBuffer;
}

unsigned int getSizeOfFile (int fileDescriptor) {
    struct stat fileInfo = {};

    if (fstat(fileDescriptor, &fileInfo) == 0)
        return fileInfo.st_size;

    perror("Error of getting the size of the file");
    return 0;
}

void skipSpaces(char** bufPos, size_t* curLine) {
    assert(bufPos);
    assert(*bufPos);

    while (**bufPos == ' ' || **bufPos == '\t' || **bufPos == '\n') {
        if (**bufPos == '\n')
            if(curLine)
                (*curLine)++;

        (*bufPos)++;
    }
}

int bsearchHashComparator(const void* firstParam, const void* secondParam) {
    assert(firstParam);
    assert(secondParam);

    const unsigned long long* idHash = (const unsigned long long*)firstParam;
    const struct identifierInfo* searchedVariable = (const struct identifierInfo*)secondParam;

    return (int)(*idHash - searchedVariable->identifierHash);
}

unsigned long long getStringHash(const char* string) {
    assert(string);
    unsigned long long hash = 5381;

    for(ssize_t numOfElement = 0; string[numOfElement] != '\0'; numOfElement++)
        hash = ((hash << 5) + hash) + (unsigned long long)(string[numOfElement]);

    return hash;
}

int structIdentifierComparator(const void* firstStruct, const void* secondStruct) {
    assert(firstStruct);
    assert(secondStruct);

    const struct identifierInfo* firstVariable = (const struct identifierInfo*)firstStruct;
    const struct identifierInfo* secondVariable = (const struct identifierInfo*)secondStruct;

    return (int)(firstVariable->identifierHash - secondVariable->identifierHash);
}

int compareDouble (double first, double second) {
    const double EPSILON = 1e-10;

    if (isnan(first) && isnan(second))      //both numbers are nan
        return 1;
    else if (isinf(first) && isinf(second))      //both numbers are inf
        return 1;
    else {
        double fabsOfDifference = NAN;
        fabsOfDifference = fabs(first - second);

        if (fabsOfDifference < EPSILON)
            return 1;
        return 0;
    }

    return 0;
}

size_t getSize_t (void) {
    int ch = 0;
    size_t num = 0;
    char ch1 = '\0';

    while ((scanf ("%llu%c", &num, &ch1) != 2) || (ch1 != '\n')) {

        putchar(ch1);
        while ((ch = getchar()) != '\n')
            putchar (ch);

        printf (" is not a unsigned number.\nEnter a unsigned number like 3, 44, 68\n");
    }
    return num;
}

unsigned long long getFactorial(size_t num) {

    unsigned long long result = 1;

    for (size_t i = 2; i <= num; i++) {
        result *= i;
    }

    return result;
}

int my_strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
