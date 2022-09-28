#include <stdio.h>
#include "pgmimage.h"
#ifndef PGMFILTERS_H_
#define PGMFILTERS_H_

void ignoreComments(FILE *fp);

/*dados da imagem*/
void getImageProps(struct pgm *image, FILE *inputImage);

/*funcao de comparar usado para calcular a mediana */
int compar(const void *a, const void *b);

/*calcula lpb*/
int neighboursLbp(struct pgm *image, FILE *inputImage, int x, int y);

/*filtro lpb*/
void pgmFilterLbp(struct pgm *image, FILE *inputImage, FILE *outputImage);

/*calcula mediana dos vizinhos*/
int neighboursMedian(struct pgm *image, FILE *inputImage, int x, int y, int mask);

/*filtro mediana*/
void pgmFilterMedian(struct pgm *image, FILE *inputImage, FILE *outputImage, int mask);

/*filtro negativo*/
void pgmNegaFilter(struct pgm *image, FILE *inputImage, FILE *outputImage);

/*filtro rotacao AINDA PRECISA ARRUMAR*/
void toRadians(double *angle);

void rotationMatrix(int *x, int *y, double angle);

void pgmFilterRotation(struct pgm *image, FILE *inputImage, FILE *outputImage, double angle);

/*filtro limiar*/
void pgmFilterLimiar(struct pgm *image, FILE *inputImage, FILE *outputImage, float limiar);

/*calcula media dos vizinhos*/
int neighboursMedia(struct pgm *image, FILE *inputImage, int x, int y);

/*filtro media*/
void pgmFilterMedia(struct pgm *image, FILE *inputImage, FILE *outputImage);

#endif
