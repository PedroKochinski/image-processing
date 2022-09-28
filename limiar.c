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
    float limiar = 0.5;

    if (argc != 5 && argc != 7) {
        perror("Erro: Muitos Argumentos. Use -l* [limiar] -i [entrada] -o [saida]\n* Parametro opcional\n");
        exit(1);
    } else if (argc < 5) {
        perror("Erro: Poucos Argumentos. Use -l* [limiar] -i [entrada] -o [saida]\n* Parametro opcional\n");
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
        if (strcmp(argv[i], "-l") == 0) {
            int j = 0;
            while (argv[i + 1][j]) {
                if (isalpha(argv[i + 1][j])) {
                    perror("Erro: O parametro -m requer um numero real\nUse -l* [limiar] -i [entrada] -o [saida]\n* Parametro opcional\n");
                    exit(1);
                }
                j++;
            }
            limiar = atof(argv[i + 1]);
            if (limiar < 0 || limiar > 1) {
                perror("Erro: O valor do limiar deve estar entre 0 e 1");
                exit(1);
            }
        }
    }

    getImageProps(pgmImage, inputFile);
    pgmFilterLimiar(pgmImage, inputFile, outputFile, limiar);
    fclose(inputFile);
    fclose(outputFile);
    free(pgmImage);
    return 0;
}
