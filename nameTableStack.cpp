#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <typeinfo>
#include <errno.h>

#pragma GCC diagnostic ignored "-Wredundant-tags"


#include "nameTableConsts.h"
#include "structsAndConsts.h"
#include "nameTableStack.h"
#include "helpingFunctions.h"

const stackElement_t POISON = NULL;
const int MAX_CAPACITY = 100000000;

int stackCtor (stack_t* stack, ssize_t capacity, const char* nameOfStack, struct info creationInfo) {
    assert(stack);
    assert(nameOfStack);

    stack->size = 0;
    stack->capacity = capacity;
    stack->nameOfStack = nameOfStack;
    stack->errorCode = 0;

    stack->stackInfo.nameOfFunct = creationInfo.nameOfFunct;
    stack->stackInfo.nameOfFile = creationInfo.nameOfFile;
    stack->stackInfo.numOfLine = creationInfo.numOfLine;

    if ((capacity <= 0) || (capacity > MAX_CAPACITY)) {
        stack->errorCode |= badCapacity;
        return stack->errorCode;
    }


    stack->data = (stackElement_t*)calloc(capacity, sizeof(stackElement_t));
    for(ssize_t numOfElement = 0; numOfElement < capacity; numOfElement++)
        stack->data[numOfElement] = POISON;


    return stack->errorCode;
}

int stackPush (stack_t* stack, stackElement_t value) {
    assert(stack);

    stack->data[stack->size] = value;
    stack->size++;

    if (stack->size == stack->capacity) {
        stack->capacity *= 2;
        stack->data = (stackElement_t*)realloc(stack->data, stack->capacity);

        if (stack->data == NULL)
            stack->errorCode = badRealloc;

        else {
            for(ssize_t numOfElement = stack->size; numOfElement < stack->capacity; numOfElement++)
                stack->data[numOfElement] = POISON;
        }
    }

    return stack->errorCode;
}

int stackPop (stack_t* stack, stackElement_t* ptrToVariable) {
    assert(ptrToVariable);
    assert(stack);

    if (stack->size == 0) {
        stack->errorCode |= noElementsForPop;
        return stack->errorCode;
    }

    stack->size--;

    *ptrToVariable = stack->data[stack->size];
    stack->data[stack->size] = POISON;

    return stack->errorCode;
}


int stackDtor (stack_t* stack) {
    assert(stack);

    free(stack->data);

    return(stack->errorCode);
}

nameTable_t* nameTableCtor (size_t capacity) {
    nameTable_t* newTable = (nameTable_t*)calloc(1, sizeof(nameTable_t));
    if (!newTable) {
        printf("ERROR nameTableCalloc!");
        return NULL;
    }

    newTable->idArray = (identifierInfo*)calloc(capacity, sizeof(identifierInfo));
    if (!newTable->idArray) {
        free(newTable);
        return NULL;
    }

    newTable->capacity = capacity;
    newTable->size = 0;

    return newTable;
}

void nameTableDtor (nameTable_t* nameTable) {
    assert(nameTable);

    free(nameTable->idArray);
    free(nameTable);
}

identifierInfo* addIdentifierInNameTable (nameTable_t* nameTable, const char* name, idType_t idType, size_t scopeLevel) { //FIXME scopeLavel
    assert(nameTable);
    assert(name);

    if (nameTable->size >= nameTable->capacity) {
        size_t newCapacity = nameTable->capacity * 2;
        identifierInfo* newIdArr = (identifierInfo*)realloc(nameTable->idArray, newCapacity * sizeof(identifierInfo));

        if (!newIdArr) {
            printf("Error nameTable idArray realloc!\n");
            return NULL;
        }


        nameTable->idArray = newIdArr;
        nameTable->capacity = newCapacity;

        qsort(nameTable->idArray, nameTable->capacity, sizeof(identifierInfo), structIdentifierComparator);
    }

    identifierInfo* newID = nameTable->idArray;

    newID->identifierName = name;
    newID->identifierHash = getStringHash(name);

    newID->idType = idType;
    newID->scopeLevel = scopeLevel;

    newID->paramCount = 0;

    qsort(nameTable->idArray, nameTable->capacity, sizeof(identifierInfo), structIdentifierComparator);

    nameTable->size++;
    return newID;
}

identifierInfo* findIdInTable (const nameTable_t* nameTable, const char* idName) {
    assert(nameTable);
    assert(idName);

    size_t searchedIdHash = getStringHash(idName);

    identifierInfo* searchedId = (identifierInfo*)bsearch(&searchedIdHash,
    nameTable->idArray, nameTable->capacity, sizeof(identifierInfo), bsearchHashComparator);

    if  (searchedId && !(strcmp(searchedId->identifierName, idName)))
        return searchedId;

    return NULL;
}

void initNameTables(tree_t* tree) {
    assert(tree);

    tree->nameTableStack = (stack_t*)calloc(1, sizeof(stack_t));
    struct info stackInfo = {};
    STACK_CTOR(*(tree->nameTableStack), stackInfo, INITIAL_STACK_CAPACITY);

    tree->currentScopeLevel = 0;

    nameTable_t* globalTable = nameTableCtor(INITIAL_TABLE_CAPACITY);
    stackPush(tree->nameTableStack, globalTable);
}

void destroyNameTables(tree_t* tree) {
    assert(tree);
    assert(tree->nameTableStack);

    while (tree->nameTableStack->size > 0) {
        nameTable_t* table;
        stackPop(tree->nameTableStack, &table);
        nameTableDtor(table);
    }

    stackDtor(tree->nameTableStack);
    free(tree->nameTableStack);
    tree->nameTableStack = NULL;
}

void enterNewScope(tree_t* tree) {
    assert(tree);
    assert(tree->nameTableStack);

    tree->currentScopeLevel++;

    nameTable_t* newTable = nameTableCtor(INITIAL_TABLE_CAPACITY);
    stackPush(tree->nameTableStack, newTable);
}

void exitScope(tree_t* tree) {
    assert(tree);
    assert(tree->nameTableStack);

    if (tree->nameTableStack->size > 1) {
        nameTable_t* table;
        stackPop(tree->nameTableStack, &table);
        nameTableDtor(table);
        tree->currentScopeLevel--;
    }
}

identifierInfo* addSymbolToCurrentScope(tree_t* tree, const char* name, idType_t idType) {
    assert(tree);
    assert(tree->nameTableStack);

    nameTable_t* currentTable = tree->nameTableStack->data[tree->nameTableStack->size - 1];

    identifierInfo* newID = addIdentifierInNameTable(currentTable, name, idType, tree->currentScopeLevel);

    return newID;
}
