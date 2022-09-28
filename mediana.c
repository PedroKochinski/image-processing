#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgmfilters.h"
#include "pgmimage.h"

int main(int argc, char *argv[]) {
    struct pgm *pgmImage = (struct pgm *)malloc(sizeof(struct pgm));
    FILE *inputFile = NULL;
    FILE *outputFile = NULL;
    int mask = 3;

    if (argc != 5 && argc != 7) {
        perror("Erro: Quantidade de argumentos inválida. Use -m* [mascara] -i [entrada] -o [saida]\n* Parametro opcional\n");
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
        if (strcmp(argv[i], "-m") == 0) {
            int j = 0;
            while (argv[i + 1][j]) {
                if (isalpha(argv[i + 1][j])) {
                    perror("Erro: O parametro -m requer um numero inteiro\nUse -m* [mascara] -i [entrada] -o [saida]\n* Parametro opcional\n");
                    exit(1);
                }
                j++;
            }
            mask = atoi(argv[i + 1]);
            if (mask < 0 || (mask % 2 == 0)) {
                perror("Erro: O valor da mascara deve ser positivo e ímpar");
                exit(1);
            }
        }
    }
    getImageProps(pgmImage, inputFile);
    pgmFilterMedian(pgmImage, inputFile, outputFile, mask);
    fclose(inputFile);
    fclose(outputFile);
    free(pgmImage);
    return 0;
}
