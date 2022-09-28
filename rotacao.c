#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgmfilters.h"

int main(int argc, char *argv[]) {
    struct pgm *pgmImage = (struct pgm *)malloc(sizeof(struct pgm));
    FILE *inputFile = NULL;
    FILE *outputFile = NULL;
    float angle = 90;

    if (argc != 5 && argc != 7) {
        perror("Erro: Quantidade de argumentos inválida. Use -a* [anglulo] -i [entrada] -o [saida]\n* Parametro opcional\n");
        exit(1);
    }
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            if (strcmp(argv[i + 1], "-o") == 0) {
                perror("Erro: Use -i para indicar o arquivo de entrada\n");
                exit(1);
            }
            inputFile = openImage(argv[i + 1]);
        }
        if (strcmp(argv[i], "-o") == 0) {
            if (strcmp(argv[i + 1], "-i") == 0) {
                perror("Erro: Use -o para indicar o arquivo de saida\n");
                exit(1);
            }
            outputFile = openOutputImage(argv[i + 1]);
        }
        if (strcmp(argv[i], "-a") == 0) {
            int j = 0;
            while (argv[i + 1][j]) {
                if (isalpha(argv[i + 1][j])) {
                    perror("Erro: O parametro -a requer um numero real\nUse -a* [anglulo] -i [entrada] -o [saida]\n* Parametro opcional\n");
                    exit(1);
                }
                j++;
            }
            angle = atof(argv[i + 1]);
        }
    }
    getImageProps(pgmImage, inputFile);
    pgmFilterRotation(pgmImage, inputFile, outputFile, angle);
    fclose(inputFile);
    fclose(outputFile);
    free(pgmImage);
    return 0;
}
