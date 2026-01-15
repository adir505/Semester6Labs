//Program to discard preprocessor directives from the given input ‘C’ file.
#include <stdio.h>
#include <stdlib.h>
void discard_preprocessor_directives(const char *input_file, const char *output_file) {
    FILE *infile = fopen(input_file, "r");
    if (infile == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    FILE *outfile = fopen(output_file, "w");
    if (outfile == NULL) {
        perror("Error opening output file");
        fclose(infile);
        exit(EXIT_FAILURE);
    }

    int ch;
    int in_directive = 0;

    while ((ch = fgetc(infile)) != EOF) {
        if (ch == '#') {
            in_directive = 1;
        }
        if (ch == '\n' && in_directive) {
            in_directive = 0;
            continue;
        }
        if (!in_directive) {
            fputc(ch, outfile);
        }
    }

    fclose(infile);
    fclose(outfile);
}
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    discard_preprocessor_directives(argv[1], argv[2]);
    return EXIT_SUCCESS;
}