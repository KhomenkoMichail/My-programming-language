#ifndef PARSER_FUNCTIONS_H
#define PARSER_FUNCTIONS_H

struct lexAnalysisResult* lexicalAnalysis (const char* nameOfProgramFile);

nodeType_t getTokenType (char** bufPos);

int tokenIsNumber (char** bufPos);

int tokenIsOperator (char** bufPos);

int tokenIsIdentifier (char** bufPos);

node_t* processToken (char** bufPos, nodeType_t tokenType, size_t curLine);

node_t* processOperator (char** bufPos, size_t curLine);

node_t* newNodeCtor (nodeType_t curType, nodeValue_t curValue, size_t curLine);

node_t* processNumber (char** bufPos, size_t curLine);

node_t* processIdentifier (char** bufPos, size_t curLine);

void fprintfLexAnalysisDump (FILE* dumpFile, char* bufPos, node_t** nodeArray, size_t numOfNodes);

#endif
