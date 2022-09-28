#include "pgmimage.h"
#include <stdio.h>
#include <stdlib.h>

/*abre imagem de entrada*/
FILE* openImage(char* imageName) {  // Menotti - struct pgm* image?
    FILE* inputFile = fopen(imageName, "rb");
    if (!inputFile) {
        perror("Erro ao abrir arquivo de entrada. Verifique o nome do arquivo e do diretório");
        exit(1);
    }

    return inputFile;
}

/*abre imagem de saida*/
FILE* openOutputImage(char* imageName) {  // Menotti - struct pgm* image?
    FILE* outputImage = fopen(imageName, "w+b");

    if (!outputImage) {
        perror("Erro ao abrir ou criar arquivo de saida. Verifique o nome do arquivo e do diretório");
        exit(1);
    }

    return outputImage;
}
