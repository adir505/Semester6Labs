#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_KEYWORDS 32
#define MAX_TOKEN_LEN 64

const char *keywords[MAX_KEYWORDS] = {
    "auto", "break", "case", "char", "const", "continue", "default",
    "do", "double", "else", "enum", "extern", "float", "for",
    "goto", "if", "int", "long", "register", "return",
    "short", "signed", "sizeof", "static", "struct",
    "switch", "typedef", "union", "unsigned",
    "void", "volatile", "while"
};

/* Check if token is a keyword */
int is_keyword(const char *token) {
    for (int i = 0; i < MAX_KEYWORDS; i++) {
        if (strcmp(token, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

/* Print token in uppercase */
void print_upper(const char *token) {
    for (int i = 0; token[i]; i++)
        putchar(toupper((unsigned char)token[i]));
    putchar('\n');
}

void lexical_analyze(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int ch, next;
    char token[MAX_TOKEN_LEN];
    int idx;

    while ((ch = fgetc(fp)) != EOF) {

        /* ---------- Handle comments ---------- */
        if (ch == '/') {
            next = fgetc(fp);

            /* Single-line comment */
            if (next == '/') {
                while ((ch = fgetc(fp)) != EOF && ch != '\n');
                continue;
            }

            /* Multi-line comment */
            if (next == '*') {
                int prev = 0;
                while ((ch = fgetc(fp)) != EOF) {
                    if (prev == '*' && ch == '/')
                        break;
                    prev = ch;
                }
                continue;
            }

            ungetc(next, fp);
        }

        /* ---------- Skip string literals ---------- */
        if (ch == '"') {
            while ((ch = fgetc(fp)) != EOF) {
                if (ch == '\\')
                    fgetc(fp);      /* escape */
                else if (ch == '"')
                    break;
            }
            continue;
        }

        /* ---------- Skip character literals ---------- */
        if (ch == '\'') {
            while ((ch = fgetc(fp)) != EOF) {
                if (ch == '\\')
                    fgetc(fp);      /* escape */
                else if (ch == '\'')
                    break;
            }
            continue;
        }

        /* ---------- Identifier / Keyword ---------- */
        if (isalpha(ch) || ch == '_') {
            idx = 0;
            token[idx++] = ch;

            while ((ch = fgetc(fp)) != EOF &&
                   (isalnum(ch) || ch == '_')) {
                if (idx < MAX_TOKEN_LEN - 1)
                    token[idx++] = ch;
            }

            token[idx] = '\0';

            if (is_keyword(token))
                print_upper(token);

            if (ch != EOF)
                ungetc(ch, fp);
        }
    }

    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    lexical_analyze(argv[1]);
    return EXIT_SUCCESS;
}