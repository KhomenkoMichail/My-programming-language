#ifndef NAME_TABLE_STACK_H
#define NAME_TABLE_STACK_H

#define STACK_CTOR(stackName, stackInfoName, capacity)\
    stackInfoName.nameOfFile = __FILE__;\
    stackInfoName.nameOfFunct = __func__;\
    stackInfoName.numOfLine = __LINE__;\
    stackCtor(&stackName, capacity, #stackName, stackInfoName);\

    int stackCtor (stack_t* stack, ssize_t capacity, const char* nameOfStack, struct info creationInfo);

    int stackPush (stack_t* stack, stackElement_t value);

    int stackPop (stack_t* stack, stackElement_t* ptrToVariable);

    int stackDtor (stack_t* stack);

    nameTable_t* nameTableCtor (size_t capacity);

    void nameTableDtor (nameTable_t* nameTable);

    //identifierInfo* addIdentifierInNameTable (nameTable_t* nameTable, const char* name, idType_t idType, size_t scopeLevel);
    identifierInfo* addIdentifierInNameTable (nameTable_t* nameTable, char* name, idType_t idType, size_t scopeLevel);

    identifierInfo* findIdInTable (const nameTable_t* nameTable, const char* idName);

    void initNameTables(tree_t* tree);

    void destroyNameTables(tree_t* tree);

    void enterNewScope(tree_t* tree);

    void exitScope(tree_t* tree);

    //identifierInfo* addIdToCurrentScope(tree_t* tree, const char* name, idType_t idType);
    identifierInfo* addIdToCurrentScope(tree_t* tree, char* name, idType_t idType);

    identifierInfo* findIdInAllScopes(tree_t* tree, const char* idName);

#endif
