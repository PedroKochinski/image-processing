#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgmfilters.h"
#include "pgmimage.h"

int main(int argc, char *argv[]) {
    struct pgm *pgmImage = (struct pgm *)malloc(sizeof(struct pgm));
    FILE *inputFile = NULL;
    FILE *outputFile = NULL;

    if (argc != 5) {
        perror("Erro: Quantidade de argumentos inválida. Use -i [entrada] -o [saida]\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && argv[i + 1]) {
            if (strcmp(argv[i + 1], "-o") == 0) {
                perror("Erro: Use -i para indicar o arquivo de entrada\n");
                exit(1);
            }
            inputFile = openImage(argv[i + 1]);
        }
        if (strcmp(argv[i], "-o") == 0 && argv[i + 1]) {
            if (strcmp(argv[i + 1], "-i") == 0) {
                perror("Erro: Use -o para indicar o arquivo de saida\n");
                exit(1);
            }
            outputFile = openOutputImage(argv[i + 1]);
        }
    }

    getImageProps(pgmImage, inputFile);
    pgmFilterLbp(pgmImage, inputFile, outputFile);
    fclose(inputFile);
    fclose(outputFile);
    free(pgmImage);
    return 0;
}
