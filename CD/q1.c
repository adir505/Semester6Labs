//Program that takes a file as input and replaces blank spaces and tabs by single space and writes the output to a file.
#include <stdio.h>
#include <stdlib.h>
void replace_spaces_and_tabs(const char *input_file, const char *output_file) {
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
    int prev_was_space = 0;

    while ((ch = fgetc(infile)) != EOF) {
        if (ch == ' ' || ch == '\t') {
            if (!prev_was_space) {
                fputc(' ', outfile);
                prev_was_space = 1;
            }
        } else {
            fputc(ch, outfile);
            prev_was_space = 0;
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

    replace_spaces_and_tabs(argv[1], argv[2]);

    return EXIT_SUCCESS;
}