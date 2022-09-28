#include "pgmfilters.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgmimage.h"
#define LINESIZE 1024

/*funcao para ignorar comentarios*/
void ignoreComments(FILE *image) {
    int ch = fgetc(image);
    char line[LINESIZE];
    if (ch == '#') {
        /*se acha o char # entao, recursivamente remove cada linha*/
        fgets(line, sizeof(line), image);
        ignoreComments(image);
    } else {
        fseek(image, -1, SEEK_CUR);
    }
}

/*dados da imagem*/
void getImageProps(struct pgm *image, FILE *inputImage) {
    fgets(image->type, LINESIZE, inputImage);
    image->min = 0;
    unsigned char asciiValue;
    if (image->type[1] == '5') {
        ignoreComments(inputImage);
        fscanf(inputImage, "%d %d", &(image->width), &(image->height));
        ignoreComments(inputImage);
        fscanf(inputImage, "%d\n", &image->max);  // Menotti +\n
        for (int i = 0; i < image->height; i++) {
            for (int j = 0; j < image->width; j++) {
                if (!fread(&asciiValue, sizeof(unsigned char), 1, inputImage)) {
                    image->data[i][j] = 0;
                } else
                    image->data[i][j] = asciiValue;
                if (image->data[i][j] < image->min) image->min = image->data[i][j];
            }
        }
    } else {
        fscanf(inputImage, "%d %d", &(image->width), &(image->height));
        fscanf(inputImage, "%d\n", &image->max);  // Menotti +\n
        while (!feof(inputImage)) {
            for (int i = 0; i < image->height; i++) {
                for (int j = 0; j < image->width; j++) {
                    fscanf(inputImage, "%hhu", &(image->data[i][j]));
                }
            }
        }
    }
}

/*funcao de comparar usado para calcular a mediana */
int compar(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

/*calcula lpb*/
int neighboursLbp(struct pgm *image, FILE *inputImage, int x, int y) {
    int *values = (int *)calloc(9, sizeof(int));
    int mask[] = {1, 2, 4, 8, 0, 16, 32, 64, 128};
    int pixelLpb;
    int lpb = 0;
    int len = 0;
    int maskIndex = 0;
    int offset = 1;
    /*verifica se esta no dominio da imagem e constroi o filtro lpb*/
    for (int i = x - offset; i <= x + offset; i++) {
        for (int j = y - offset; j <= y + offset; j++) {
            if (i >= 0 && i < image->height && j >= 0 && j < image->width && !(i == x && j == y)) {
                if (image->data[i][j] >= image->data[x][y])
                    pixelLpb = 1;
                else
                    pixelLpb = 0;
                pixelLpb *= mask[maskIndex];
                values[len] = pixelLpb;
            }
            maskIndex++;
            len++;
        }
    }
    for (int i = 0; i < len; i++) {
        lpb += values[i];
    }
    free(values);
    return lpb;
}

/*filtro lpb*/
void pgmFilterLbp(struct pgm *image, FILE *inputImage, FILE *outputImage) {
    char line[LINESIZE + 1];
    int lpb;

    // Menotti - nova imagem
    struct pgm *imageOut = (struct pgm *)malloc(sizeof(struct pgm));
    strcpy(imageOut->type, image->type);
    imageOut->max = 255;  // Menotti image->max;
    imageOut->width = image->width;
    imageOut->height = image->height;

    fprintf(outputImage, "%s%d %d\n%d\n", image->type, image->width, image->height, imageOut->max);  // Menotti ->max
    fgets(line, LINESIZE, inputImage);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            lpb = neighboursLbp(image, inputImage, i, j);
            imageOut->data[i][j] = lpb;  // Out
            if (imageOut->type[1] == '5')
                fwrite(&imageOut->data[i][j], 1, 1, outputImage);
            else
                fprintf(outputImage, "%d ", imageOut->data[i][j]);
        }
    }
}

/*calcula mediana dos vizinhos*/
int neighboursMedian(struct pgm *image, FILE *inputImage, int x, int y, int mask) {
    int values[mask * mask];
    //    for(int i = 0; i < mask*mask; i++){
    //        values[i] = image->data[x][i];
    //    }
    int mediana;
    int len = 0;
    int offset = mask / 2;
    if (x < offset || x >= image->height - offset || y < offset || y >= image->width - offset) {
        return image->data[x][y];
    }
    for (int i = x - offset; i <= x + offset; i++) {
        for (int j = y - offset; j <= y + offset; j++) {
            if (i >= 0 && i < image->height && j >= 0 && j < image->width) {
                values[len] = image->data[i][j];
                len++;
            }
        }
    }
    qsort(values, len, sizeof(int), compar);
    mediana = values[len / 2];
    return mediana;
}

/*filtro mediana*/
void pgmFilterMedian(struct pgm *image, FILE *inputImage, FILE *outputImage, int mask) {
    char line[LINESIZE + 1];
    int mediana;

    // Menotti - nova imagem
    struct pgm *imageOut = malloc(sizeof(struct pgm));
    strcpy(imageOut->type, image->type);
    imageOut->max = image->max;
    imageOut->width = image->width;
    imageOut->height = image->height;

    fprintf(outputImage, "%s%d %d\n%d\n", imageOut->type, imageOut->width, imageOut->height, imageOut->max);
    fgets(line, LINESIZE, inputImage);
    for (int i = 0; i < imageOut->height; i++) {
        for (int j = 0; j < imageOut->width; j++) {
            mediana = neighboursMedian(image, inputImage, i, j, mask);
            imageOut->data[i][j] = mediana;
            if (imageOut->type[1] == '5')
                fwrite(&imageOut->data[i][j], 1, 1, outputImage);
            else
                fprintf(outputImage, "%u ", imageOut->data[i][j]);
        }
    }
}

/*filtro negativo*/
void pgmNegaFilter(struct pgm *image, FILE *inputImage, FILE *outputImage) {
    char line[LINESIZE + 1];
    fprintf(outputImage, "%s%d %d\n%d\n", image->type, image->width, image->height, image->max - image->min);
    fgets(line, LINESIZE, inputImage);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            image->data[i][j] = image->max - image->data[i][j];
            if (image->type[1] == '5') {
                fwrite(&(image->data[i][j]), 1, 1, outputImage);
            } else
                fprintf(outputImage, "%u ", image->data[i][j]);
        }
    }
}

/*rotation by any angle functions*/

void toRadians(double *angle) {
    *angle = (*angle * M_PI) / 180;
}

void rotationMatrix(int *x, int *y, double angle) {
    double newX = *x * cos(angle) - *y * sin(angle);
    double newY = *x * sin(angle) + *y * cos(angle);
    *x = round(newX);
    *y = round(newY);
}

void pgmFilterRotation(struct pgm *image, FILE *inputImage, FILE *outputImage, double angle) {
    unsigned int marginPixel = 0;
    char line[LINESIZE + 1];
    int x;
    int y;
    toRadians(&angle);

    struct pgm *imageOut = malloc(sizeof(struct pgm));
    strcpy(imageOut->type, image->type);
    imageOut->max = image->max;
    imageOut->width = round((abs(image->height * sin(angle))) + abs(image->width * cos(angle)));
    imageOut->height = round((abs((image->height * cos(angle)))) + abs(image->width * sin(angle)));

    fprintf(outputImage, "%s%d %d\n%d\n", imageOut->type, imageOut->width, imageOut->height, imageOut->max);
    fgets(line, LINESIZE, inputImage);

    /*calcula o centro da imagen original e rotacionada*/

    int originalXCenter = image->height / 2;
    int originalYCenter = image->width / 2;
    int newXCenter = imageOut->height / 2;
    int newYCenter = imageOut->width / 2;

    /*verifica se cada ponto da imagem rotacionada existe na imagem original e insere na matriz data*/

    for (int i = 0; i < imageOut->height; i++) {
        for (int j = 0; j < imageOut->width; j++) {
            x = imageOut->height - i - newXCenter;
            y = imageOut->width - j - newYCenter;
            rotationMatrix(&x, &y, angle);
            y = originalYCenter - y;
            x = originalXCenter - x;
            imageOut->data[i][j] = 255;
            if (x >= 0 && y >= 0 && x < image->height && y < image->width) {
                imageOut->data[i][j] = image->data[x][y];
            }
        }
    }

    for (int i = 0; i < imageOut->height; i++) {
        for (int j = 0; j < imageOut->width; j++) {
            if (i == 0 || i == imageOut->height - 1 || j == imageOut->width - 1 || j == 0) {
                if (imageOut->type[1] == '5')
                    fwrite(&marginPixel, 1, 1, outputImage);
                else
                    fprintf(outputImage, "%u ", marginPixel);
            } else if (imageOut->data[i][j] >= 0) {
                if (imageOut->type[1] == '5')
                    fwrite(&imageOut->data[i][j], 1, 1, outputImage);
                else
                    fprintf(outputImage, "%u ", imageOut->data[i][j]);
            }
        }
    }
}

/*filtro limiar*/
void pgmFilterLimiar(struct pgm *image, FILE *inputImage, FILE *outputImage, float limiar) {
    char line[LINESIZE + 1];
    float limiarPixel = (limiar * image->max);  // Menotti
    fprintf(outputImage, "%s%d %d\n%d\n", image->type, image->width, image->height, image->max);
    fgets(line, LINESIZE, inputImage);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            if (image->data[i][j] >= limiarPixel)
                image->data[i][j] = image->max;
            else
                image->data[i][j] = 0;

            if (image->type[1] == '5')
                fwrite(&(image->data[i][j]), 1, 1, outputImage);
            else
                fprintf(outputImage, "%u ", image->data[i][j]);
        }
    }
}

/*calcula media dos vizinhos*/
int neighboursMedia(struct pgm *image, FILE *inputImage, int x, int y) {
    int mask = 3;  // Menotti - 9 > 3
    int *values = (int *)calloc(mask * mask, sizeof(int));
    int media = 0;
    int len = 0;
    int offset = mask / 2;
    for (int i = x - offset; i <= x + offset; i++) {
        for (int j = y - offset; j <= y + offset; j++) {
            if (i >= 0 && i < image->height && j >= 0 && j < image->width) {  // Menotti
                values[len] = image->data[i][j];
                len++;  // Menotti - len++ apenas quando atribuir em values
            }
        }
    }
    for (int i = 0; i < len; i++) {
        media += values[i];
    }
    free(values);
    return media / len;
}

/*filtro media*/
void pgmFilterMedia(struct pgm *image, FILE *inputImage, FILE *outputImage) {
    char line[LINESIZE + 1];
    int media;

    // Menotti - nova imagem
    struct pgm *imageOut = malloc(sizeof(struct pgm));
    strcpy(imageOut->type, image->type);
    imageOut->max = image->max;
    imageOut->width = image->width;
    imageOut->height = image->height;

    fprintf(outputImage, "%s%d %d\n%d\n", imageOut->type, imageOut->width, imageOut->height, imageOut->max);
    fgets(line, LINESIZE, inputImage);

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            media = neighboursMedia(image, inputImage, i, j);
            imageOut->data[i][j] = (unsigned char)media;
            if (imageOut->type[1] == '5')
                fwrite(&imageOut->data[i][j], 1, 1, outputImage);
            else
                fprintf(outputImage, "%u ", imageOut->data[i][j]);
        }
    }

    free(imageOut);
}
