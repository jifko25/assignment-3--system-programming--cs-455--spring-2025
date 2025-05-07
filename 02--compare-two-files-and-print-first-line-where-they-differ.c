#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file1> <file2>\n", argv[0]);
        return 1;
    }

    FILE *fp1 = fopen(argv[1], "r");
    FILE *fp2 = fopen(argv[2], "r");

    if (!fp1 || !fp2) {
        perror("Error! Cannot open file");
        return 1;
    }

    char line1[MAX_LINE], line2[MAX_LINE];
    int lineno = 1;

    while (fgets(line1, MAX_LINE, fp1) && fgets(line2, MAX_LINE, fp2)) {
        if (strcmp(line1, line2) != 0) {
            printf("Files differ at line %d:\n", lineno);
            printf("%s: %s", argv[1], line1);
            printf("%s: %s", argv[2], line2);
            fclose(fp1);
            fclose(fp2);
            return 0;
        }
        lineno++;
    }
    
    if (fgets(line1, MAX_LINE, fp1)) {
        printf("Files differ at line %d:\n", lineno);
        printf("%s: %s", argv[1], line1);
        printf("%s: <no line>\n", argv[2]);
    } else if (fgets(line2, MAX_LINE, fp2)) {
        printf("Files differ at line %d:\n", lineno);
        printf("%s: <no line>\n", argv[1]);
        printf("%s: %s", argv[2], line2);
    } else {
        printf("Files are identical.\n");
    }

    fclose(fp1);
    fclose(fp2);
    return 0;
}