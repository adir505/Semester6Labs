/*Lexical analyzer to tokenize simple C programs. Takes filepath as input and outputs symbol table 
to a file.*/
#include "identifier_functions.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE *sourceFile = fopen("input.c", "r");
    if (sourceFile == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    PreprocessState preprocessState;
    initPreprocessState(&preprocessState);
    CharBuffer charBuffer;
    initBuffer(&charBuffer);

    Token token;
    do {
        token = getNextToken(sourceFile, &preprocessState, &charBuffer);
        if(token.type == TOKEN_ERROR) continue;
        printf("Token Type: %d, Lexeme: %s\n", token.type, token.lexeme);
    } while (token.type != TOKEN_EOF);

    fclose(sourceFile);
    return EXIT_SUCCESS;
}