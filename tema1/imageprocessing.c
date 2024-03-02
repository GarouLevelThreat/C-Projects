#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"

#define DIM_COMPONENTA 3  // dimensiunea rgb
#define MIN_VAL 0
#define MAX_VAL 255

// functie de dezalocare a ununi filtru
void dezaloc_filter(float **filter, int filter_size) {
  int i = 0;

  for (i = 0; i < filter_size; i++) {
    free(filter[i]);
  }
  free(filter);
}

// functie de alocare a unui filtru
float **aloc_filter(int filter_size) {
  int i = 0, j = 0;

  float **newFilter = (float **)malloc(filter_size * sizeof(float *));
  if (newFilter == NULL) {
    return NULL;
  } else {
    for (i = 0; i < filter_size; i++) {
      newFilter[i] = (float *)malloc(filter_size * sizeof(float));
      if (newFilter[i] == NULL) {
        for (j = 0; j < i; j++) {
          free(newFilter[j]);
        }
        free(newFilter);
        return NULL;
      }
    }

    return newFilter;
  }
}

// functie de dezalocare a unei imagini
void dezaloc_image(int ***image, int N, int M) {
  int i = 0, j = 0;

  for (i = 0; i < N; i++) {
    for (j = 0; j < M; j++) {
      free(image[i][j]);
    }
    free(image[i]);
  }
  free(image);
}

// functie de alocare a unei imagini
int ***aloc_image(int N, int M) {
  int i = 0, i1 = 0, j = 0, j1 = 0;

  int ***newImage = (int ***)malloc(N * sizeof(int **));
  if (newImage == NULL) {
    return NULL;
  } else {
    for (i = 0; i < N; i++) {
      newImage[i] = (int **)malloc(M * sizeof(int *));
      if (newImage[i] == NULL) {
        for (i1 = 0; i1 < i; i1++) {
          for (j1 = 0; j1 < M; j++) {
            free(newImage[i1][j1]);
          }
          free(newImage[i1]);
        }
        free(newImage);
        return NULL;
      } else {
        for (j = 0; j < M; j++) {
          newImage[i][j] = (int *)malloc(DIM_COMPONENTA * sizeof(int));
          if (newImage[i][j] == NULL) {
            for (j1 = 0; j1 < j; j1++) {
              free(newImage[i][j1]);
            }
            free(newImage[i]);

            for (i1 = 0; i1 < i; i1++) {
              for (j1 = 0; j1 < M; j++) {
                free(newImage[i1][j1]);
              }
              free(newImage[i1]);
            }
            free(newImage);
            return NULL;
          }
        }
      }
    }
  }

  return newImage;
}

// TODO(me) Task 1
int ***flip_horizontal(int ***image, int N, int M) {
  int i = 0, j = 0, half = M / 2, *temp = NULL;

  for (i = 0; i < N; i++) {
    for (j = 0; j < half; j++) {
      temp = image[i][j];
      image[i][j] = image[i][M - j - 1];
      image[i][M - j - 1] = temp;
    }
  }

  return image;
}

// TODO(me) Task 2
int ***rotate_left(int ***image, int N, int M) {
  int i = 0, j = 0, k = 0, ***newImage = NULL;

  // aloc dinamic matricea care va rezulta in urma rotirii
  newImage = aloc_image(M, N);
  if (newImage == NULL) {
    dezaloc_image(image, N, M);
    return NULL;
  } else {
    // transfer componentele conform cerintei. Liniile inversate devin coloane
    for (i = 0; i < N; i++) {
      for (j = 0; j < M; j++) {
        for (k = 0; k < DIM_COMPONENTA; k++) {
          newImage[M - j - 1][i][k] = image[i][j][k];
        }
      }
    }

    // dezaloc imaginea primita ca parametru
    dezaloc_image(image, N, M);

    return newImage;
  }
}

// TODO(me) Task 3
int ***crop(int ***image, int N, int M, int x, int y, int h, int w) {
  int i = 0, j = 0, k = 0, ***newImage = NULL;

  // aloc dinamic matricea care va rezulta in urma functiei
  newImage = aloc_image(h, w);
  if (newImage == NULL) {
    dezaloc_image(image, N, M);
    return NULL;
  } else {
    // implementarea efectiva a functiei
    for (i = 0; i < h; i++) {
      for (j = 0; j < w; j++) {
        for (k = 0; k < DIM_COMPONENTA; k++) {
          newImage[i][j][k] = image[y + i][x + j][k];
        }
      }
    }

    // dezaloc imaginea primita ca parametru
    dezaloc_image(image, N, M);

    return newImage;
  }
}

// TODO(me) Task 4
int ***extend(int ***image, int N, int M, int rows, int cols, int new_R, int new_G, int new_B) {
  int i = 0, j = 0, k = 0, newN = 0, newM = 0, ***newImage = NULL;

  int *colors = (int *)malloc(DIM_COMPONENTA * (sizeof(int)));
  colors[0] = new_R, colors[1] = new_G, colors[2] = new_B;

  // aloc dinamic matricea care va rezulta in urma functiei
  newN = N + 2 * rows;
  newM = M + 2 * cols;
  newImage = aloc_image(newN, newM);
  if (newImage == NULL) {
    dezaloc_image(image, N, M);
    free(colors);
    return NULL;
  } else {
    // implementarea efectiva a functiei
    for (i = 0; i < newN; i++) {
      for (j = 0; j < newM; j++) {
        if (rows <= i && i < rows + N) {
          if (cols <= j && j < cols + M) {
            // daca pixelul se afla printre cei ai imaginii originale
            for (k = 0; k < DIM_COMPONENTA; k++) {
              newImage[i][j][k] = image[i - rows][j - cols][k];
            }
          } else {
            // daca pixelul tocmai a fost adaugat, ii atribui culoarea ceruta
            for (k = 0; k < DIM_COMPONENTA; k++) {
              newImage[i][j][k] = colors[k];
            }
          }
        } else {
          // daca pixelul tocmai a fost adaugat, ii atribui culoarea ceruta
          for (k = 0; k < DIM_COMPONENTA; k++) {
            newImage[i][j][k] = colors[k];
          }
        }
      }
    }

    // dezaloc colors
    free(colors);

    // dezaloc imaginea primita ca parametru
    dezaloc_image(image, N, M);

    return newImage;
  }
}

// TODO(me) Task 5
int ***paste(int ***image_dst, int N_dst, int M_dst, int *** image_src, int N_src, int M_src, int x, int y) {
  int i = 0, j = 0, k = 0;

  for (i = y; (i < N_dst) && (i - y < N_src); i++) {
    for (j = x; (j < M_dst) && (j - x < M_src); j++) {
      for (k = 0; k < DIM_COMPONENTA; k++) {
        image_dst[i][j][k] = image_src[i - y][j - x][k];
      }
    }
  }

  return image_dst;
}

// TODO(me) Task 6
int ***apply_filter(int ***image, int N, int M, float **filter, int filter_size) {
  int i = 0, j = 0, k1 = 0, k2 = 0, k = 0, ***newImage = NULL;
  int left_up_i = 0, left_down_i = 0, left_up_j = 0, right_up_j = 0;

  // aloc dinamic matricea care va rezulta in urma functiei
  newImage = aloc_image(N, M);
  if (newImage == NULL) {
    dezaloc_image(image, N, M);
    return NULL;
  } else {
    // aloc dinamic un vector cu valorile pixelilor dupa aplicarea efectului
    float *newColor = (float *)calloc(DIM_COMPONENTA, sizeof(float));
    // implementarea efectiva a functiei
    for (i = 0; i < N; i++) {
      for (j = 0; j < M; j++) {
        newColor[0] = newColor[1] = newColor[2] = 0;

        left_up_i = i - filter_size / 2, left_down_i = i + filter_size / 2;
        left_up_j = j - filter_size / 2, right_up_j = j + filter_size / 2;

        for (k1 = left_up_i; k1 <= left_down_i; k1++) {
          if (0 <= k1 && k1 < N) {
            for (k2 = left_up_j; k2 <= right_up_j; k2++) {
              if (0 <= k2 && k2 < M) {
                for (k = 0; k < DIM_COMPONENTA; k++) {
                  newColor[k] += ((float)image[k1][k2][k] * filter[k1 - left_up_i][k2 - left_up_j]);
                }
              }
            }
          }
        }

        for (k = 0; k < DIM_COMPONENTA; k++) {
          if (newColor[k] < MIN_VAL) {
            newImage[i][j][k] = MIN_VAL;
          } else if (newColor[k] > MAX_VAL) {
            newImage[i][j][k] = MAX_VAL;
          } else {
            newImage[i][j][k] = (int)newColor[k];
          }
        }
      }
    }

    // dezaloc newColor
    free(newColor);

    // dezaloc imaginea primita ca parametru
    dezaloc_image(image, N, M);

    return newImage;
  }
}
