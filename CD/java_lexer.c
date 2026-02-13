#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

// Global variables for Scope Tracking
int braceCount = 0;       // Tracks { } nesting
int parenthesisCount = 0; // Tracks ( ) nesting

typedef struct SymbolTable {
    char lexeme[100];
    char type[100];
    char scope[100];      // Added scope column
} SymbolTable;

// Check if character can start an Identifier (Letter, _, $)
int isIdStart(int c) {
    if (isalpha(c) || c == '_' || c == '$')
        return 1;
    return 0;
}

// Check if character is part of Identifier (Letter, Digit, _, $)
int isIdBody(int c) {
    if (isalnum(c) || c == '_' || c == '$')
        return 1;
    return 0;
}

int isKeyword(char* buffer) {
    char keywords[16][20] = {
        "class", "public", "static", "void", "main", "String", 
        "int", "float", "char", "boolean", 
        "if", "else", "for", "while", "return", "new"
    };
    for(int i = 0; i < 16; i++) {
        if(strcmp(keywords[i], buffer) == 0)
            return 1;
    }
    return 0;
}

int lookup(SymbolTable *table, char *buffer, int tableindex) {
    for(int i = 0; i < tableindex; i++) {
        if(strcmp(table[i].lexeme, buffer) == 0)
            return 1;
    }
    return 0;
}

void insert(SymbolTable *table, char *buffer, int tableindex) {
    strcpy(table[tableindex].lexeme, buffer);
    strcpy(table[tableindex].type, "IDENTIFIER");
    
    // Simple heuristic for Scope based on Brace Count
    if (braceCount == 0)
        strcpy(table[tableindex].scope, "GLOBAL/PACKAGE");
    else if (braceCount == 1)
        strcpy(table[tableindex].scope, "CLASS MEMBER");
    else if (parenthesisCount > 0)
        strcpy(table[tableindex].scope, "METHOD PARAMETER");
    else
        strcpy(table[tableindex].scope, "LOCAL VARIABLE");
}

void printTable(SymbolTable *table, int tableindex) {
    printf("\n\n");
    printf("-----------------------------SYMBOL TABLE------------------------------\n");
    printf("| %-25s | %-15s | %-20s |\n", "LEXEME", "TYPE", "SCOPE");
    printf("-----------------------------------------------------------------------\n");
    for(int i = 0; i < tableindex; i++) {
        printf("| %-25s | %-15s | %-20s |\n", table[i].lexeme, table[i].type, table[i].scope);
    }
    printf("-----------------------------------------------------------------------\n");
}

int main() {
    FILE *input = fopen("sample.txt", "r"); // Make sure this matches your input file
    if(input == NULL) {
        printf("Error: Could not open sample.txt\n");
        return 0;
    }

    SymbolTable table[1000];
    int symboltableindex = 0;
    int c;

    while((c = fgetc(input)) != EOF) {
        
        // 1. Handle Comments and Division
        if(c == '/') {
            int next = fgetc(input);
            if(next == '/') { // Single line comment
                while((c = fgetc(input)) != EOF && c != '\n');
            }
            else if(next == '*') { // Multi-line comment
                while((c = fgetc(input)) != EOF) {
                    if(c == '*') {
                        if((c = fgetc(input)) == '/') break;
                        ungetc(c, input);
                    }
                }
            }
            else { // It's the division operator
                ungetc(next, input);
                printf("[Operator: /]\n");
            }
        }
        
        // 2. Handle String Literals ("...")
        else if(c == '"') {
            printf("[String Literal: \"");
            while((c = fgetc(input)) != EOF && c != '"') {
                printf("%c", c);
            }
            printf("\"]\n");
        }
        
        // 3. Handle Identifiers and Keywords
        else if(isIdStart(c)) {
            char buffer[100];
            int idx = 0;
            buffer[idx++] = c;
            
            while((c = fgetc(input)) != EOF && isIdBody(c)) {
                buffer[idx++] = c;
            }
            buffer[idx] = '\0';
            ungetc(c, input);

            if(isKeyword(buffer)) {
                printf("[Keyword: %s]\n", buffer);
            } else {
                printf("[Identifier: %s]\n", buffer);
                if(!lookup(table, buffer, symboltableindex)) {
                    insert(table, buffer, symboltableindex++);
                }
            }
        }
        
        // 4. Handle Numbers
        else if(isdigit(c)) {
            char buffer[100];
            int idx = 0;
            buffer[idx++] = c;
            while((c = fgetc(input)) != EOF && isdigit(c)) {
                buffer[idx++] = c;
            }
            buffer[idx] = '\0';
            ungetc(c, input);
            printf("[Number: %s]\n", buffer);
        }
        
        // 5. Handle Separators and Braces (Scope Logic)
        else if(strchr("(){};,", c)) {
            printf("[Separator: %c]\n", c);
            if(c == '{') braceCount++;
            if(c == '}') braceCount--;
            if(c == '(') parenthesisCount++;
            if(c == ')') parenthesisCount--;
        }
        
        // 6. Handle Operators (Simple & Compound)
        else if(strchr("+-*=<>&|!", c)) {
            char buffer[3] = {c, '\0', '\0'};
            int next = fgetc(input);
            
            // Check for compound operators like <=, >=, ==, !=, ++
            if((c == '<' || c == '>' || c == '=' || c == '!') && next == '=') {
                buffer[1] = '=';
            } else if((c == '+' && next == '+') || (c == '-' && next == '-')) {
                buffer[1] = next;
            } else {
                ungetc(next, input);
            }
            printf("[Operator: %s]\n", buffer);
        }
        
        // Ignore whitespace
        else if(isspace(c)) {
            continue;
        }
    }

    printTable(table, symboltableindex);
    fclose(input);
    return 0;
}
