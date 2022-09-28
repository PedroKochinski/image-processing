#include <stdio.h>
#ifndef PGMIMAGE_H_
#define PGMIMAGE_H_
struct pgm {
    char type[BUFSIZ];
    unsigned char data[BUFSIZ][BUFSIZ];  // Menotti 8k * 8k = 16M - BUFSIZ = 8192
    unsigned int width;
    unsigned int height;
    unsigned int max;
    unsigned int min;
};

/*abre imagem de entrada*/
FILE* openImage(char* imageName);  // Menotti removi struct pgm* image

/*abre imagem de saida*/
FILE* openOutputImage(char* imageName);

#endif
