#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

// Global variables for Scope Tracking
int currentIndent = 0;
int previousIndent = 0;
int scopeLevel = 0; // 0 = Global, 1+ = Local/Function Block

typedef struct SymbolTable {
    char lexeme[100];
    char type[100];
    int scopeLevel; // Store the numeric scope level
} SymbolTable;

int isIdStart(int c) {
    if (isalpha(c) || c == '_')
        return 1;
    return 0;
}

int isIdBody(int c) {
    if (isalnum(c) || c == '_')
        return 1;
    return 0;
}

int isKeyword(char* buffer) {
    char keywords[20][20] = {
        "def", "return", "if", "elif", "else", 
        "for", "while", "break", "continue", 
        "class", "import", "from", "as", 
        "True", "False", "None", "and", "or", "not", "in"
    };
    for(int i = 0; i < 20; i++) {
        if(strcmp(keywords[i], buffer) == 0)
            return 1;
    }
    return 0;
}

int lookup(SymbolTable *table, char *buffer, int tableindex) {
    for(int i = 0; i < tableindex; i++) {
        // In a real compiler, we'd check scope here too (shadowing variables)
        // For this lab, we just check if the name exists.
        if(strcmp(table[i].lexeme, buffer) == 0)
            return 1;
    }
    return 0;
}

void insert(SymbolTable *table, char *buffer, int tableindex) {
    strcpy(table[tableindex].lexeme, buffer);
    strcpy(table[tableindex].type, "IDENTIFIER");
    table[tableindex].scopeLevel = scopeLevel;
}

void printTable(SymbolTable *table, int tableindex) {
    printf("\n\n");
    printf("-----------------------------SYMBOL TABLE------------------------------\n");
    printf("| %-25s | %-15s | %-20s |\n", "LEXEME", "TYPE", "SCOPE LEVEL");
    printf("-----------------------------------------------------------------------\n");
    for(int i = 0; i < tableindex; i++) {
        char scopeName[20];
        if (table[i].scopeLevel == 0) strcpy(scopeName, "GLOBAL");
        else sprintf(scopeName, "LOCAL (Level %d)", table[i].scopeLevel);
        
        printf("| %-25s | %-15s | %-20s |\n", table[i].lexeme, table[i].type, scopeName);
    }
    printf("-----------------------------------------------------------------------\n");
}

int main() {
    FILE *input = fopen("sample_python.txt", "r");
    if(input == NULL) {
        printf("Error: Could not open sample_python.txt\n");
        return 0;
    }

    SymbolTable table[1000];
    int symboltableindex = 0;
    int c;

    // Initial check for indentation at the very start of file
    // (Usually 0, but good practice)
    
    while((c = fgetc(input)) != EOF) {
        
        // 1. Handle Comments (#)
        if(c == '#') {
            while((c = fgetc(input)) != EOF && c != '\n');
            // We hit newline, let the newline logic below handle the scope check
            ungetc(c, input); 
        }

        // 2. Handle Newline & Indentation (Scope Logic)
        else if(c == '\n') {
            currentIndent = 0;
            // Peek ahead to count spaces
            while((c = fgetc(input)) != EOF) {
                if(c == ' ') currentIndent++;
                else if(c == '\t') currentIndent += 4; // Assume tab = 4 spaces
                else {
                    ungetc(c, input); // Not whitespace, put it back
                    break;
                }
            }
            
            // Check if indentation changed
            if (currentIndent > previousIndent) {
                scopeLevel++;
                printf("[INDENT: Level %d]\n", scopeLevel);
            }
            else if (currentIndent < previousIndent) {
                // In real python, we might dedent multiple levels (e.g. from 8 spaces to 0)
                // Here we just decrement once for simplicity, or reset if 0.
                while(currentIndent < previousIndent) {
                    scopeLevel--;
                    previousIndent -= 4; // roughly step back
                    printf("[DEDENT: Level %d]\n", scopeLevel);
                }
                // Safety clamp
                if(scopeLevel < 0) scopeLevel = 0;
            }
            previousIndent = currentIndent;
        }
        
        // 3. Handle Strings ('...' or "...")
        else if(c == '"' || c == '\'') {
            char quoteType = c; // Remember if it was ' or "
            printf("[String Literal: %c", quoteType);
            while((c = fgetc(input)) != EOF && c != quoteType) {
                printf("%c", c);
            }
            printf("%c]\n", quoteType);
        }
        
        // 4. Handle Identifiers and Keywords
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
        
        // 5. Handle Numbers
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
        
        // 6. Handle Operators and Separators
        else if(strchr("+-*/%=&|<>!^", c)) {
             char buffer[3] = {c, '\0', '\0'};
             int next = fgetc(input);
             
             // Check for ==, <=, >=, !=, //, **
             if(next == '=' || (c == '*' && next == '*') || (c == '/' && next == '/')) {
                 buffer[1] = next;
             } else {
                 ungetc(next, input);
             }
             printf("[Operator: %s]\n", buffer);
        }
        
        else if(strchr("():,.", c)) {
             printf("[Separator: %c]\n", c);
        }
        
        // Ignore other whitespace (spaces between words)
        else if(isspace(c)) {
            continue;
        }
    }

    printTable(table, symboltableindex);
    fclose(input);
    return 0;
}
