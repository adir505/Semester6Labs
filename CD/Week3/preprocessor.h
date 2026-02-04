/*Reads input C files and readies them for lexical analysis.*/
#include <stdio.h>
//Structure for identifying state of preprocessing.
typedef struct {
    int in_string;
    int in_char;
    int at_line_start;
    int in_directive;
    int prev_was_space;
    int line;
    int column;
} PreprocessState;

//Creates buffer between source and processing, to prevent overwriting source code.
int getCleanChar(FILE *src, PreprocessState *st) {
    int ch;

    while ((ch = fgetc(src)) != EOF) {

        /* ---------- Handle strings ---------- */
        if (ch == '"' && !st->in_char) {
            st->in_string = !st->in_string;
            return ch;
        }

        /* ---------- Handle char literals ---------- */
        if (ch == '\'' && !st->in_string) {
            st->in_char = !st->in_char;
            return ch;
        }
        if (!st->in_string && !st->in_char && ch == '/') {
            int next = fgetc(src);

            // Case 1: Single-line comment (//)
            if (next == '/') {
                // Consume everything until newline
                while ((ch = fgetc(src)) != EOF && ch != '\n');
                if (ch == '\n') ungetc(ch, src);
                continue; // Restart loop to fetch the next valid char
            } 
            
            // Case 2: Multi-line comment (/* ... */)
            else if (next == '*') {
                int prev_c = 0;
                // Consume until we find "*/"
                while ((ch = fgetc(src)) != EOF) {
                    if (prev_c == '*' && ch == '/') {
                        break; 
                    }
                    prev_c = ch;
                }
                ch = ' '; 
            } 
            
            // Case 3: It's just a division operator (/)
            else {
                ungetc(next, src); // Put the non-comment char back
                // Keep ch as '/' and proceed
            }
        }
        /* ---------- Detect preprocessor directive ---------- */
        if (!st->in_string && !st->in_char && st->at_line_start) {
            if (ch == '#') {
                st->in_directive = 1;
                continue;
            }
            if (ch != ' ' && ch != '\t')
                st->at_line_start = 0;
        }

        /* ---------- Skip preprocessor lines ---------- */
        if (st->in_directive) {
            if (ch == '\n') {
                st->in_directive = 0;
                st->at_line_start = 1;
            }
            continue;
        }

        /* ---------- Whitespace normalization ---------- */
        if (!st->in_string && !st->in_char &&
            (ch == ' ' || ch == '\t')) {

            if (st->prev_was_space)
                continue;

            st->prev_was_space = 1;
            return ' ';
        }

        st->prev_was_space = 0;

        /* ---------- Newline handling ---------- */
        if (ch == '\n') {
            st->at_line_start = 1;
            return ch;
        }

        st->at_line_start = 0;
        return ch;
    }

    return EOF;
}