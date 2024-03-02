#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imageprocessing.h"
#include "bmp.h"

#define COMMAND_MAX 100
#define PATH_MAX 100

int ***load(int N, int M, char *path) {
  int ***image = aloc_image(N, M);
  read_from_bmp(image, N, M, path);
  return image;
}

int main() {
  // variabile comune
  int N = 0, M = 0, i = 0, j = 0, index = 0;
  // char command[3] = {'\0'}, path[MAX_COMMAND] = {'\0'};
  char *command = (char *)malloc(COMMAND_MAX * sizeof(char));
  char *path = (char *)malloc(PATH_MAX * sizeof(char));
  // variabile folosite pentru prelucrarea imaginilor
  int ****imagesArray = NULL, *images_N = NULL, *images_M = NULL, nr_images = 0;
  int index_img = 0, index_dst = 0, index_src = 0;
  // variabile folosite pentru prelucrarea filtrelor
  float ***filtersArray = NULL;
  int  *filters_size = NULL, nr_filters = 0, size = 0, index_filter = 0;
  // variabile folosite pentru implementarea diversor functii
  int x = 0, y = 0, h = 0, w = 0;
  int rows = 0, cols = 0, R = 0, G = 0, B = 0;

  do {
    scanf("%s", command);

    if (!strcmp(command, "l")) {
      scanf("%d%d", &N, &M);
      scanf("%s", path);

      // updatez dimensiunea vectorilor care imi retin dimensiunile imaginilor
      images_N = (int *)realloc(images_N, (nr_images + 1) * sizeof(int));
      images_N[nr_images] = N;
      images_M = (int *)realloc(images_M, (nr_images + 1) * sizeof(int));
      images_M[nr_images] = M;

      // updatez dimensiunea vectorului care imi retine imaginile
      imagesArray = (int ****)realloc(imagesArray, (nr_images + 1) * sizeof(int ***));
      imagesArray[nr_images] = load(N, M, path);

      // retin numarul nou de imagini
      nr_images++;
    } else if (!strcmp(command, "s")) {
      scanf("%d", &index);
      scanf("%s", path);
      write_to_bmp(imagesArray[index], images_N[index], images_M[index], path);
    } else if (!strcmp(command, "ah")) {
      scanf("%d", &index);
      imagesArray[index] = flip_horizontal(imagesArray[index], images_N[index], images_M[index]);
    } else if (!strcmp(command, "ar")) {
      scanf("%d", &index);
      imagesArray[index]  = rotate_left(imagesArray[index], images_N[index], images_M[index]);

      // updatez elementele de la poz. index al vectorilor care imi retin dimensiunile imaginilor
      int temp = images_N[index];
      images_N[index] = images_M[index];
      images_M[index] = temp;
    } else if (!strcmp(command, "ac")) {
      scanf("%d%d%d%d%d", &index, &x, &y, &w, &h);
      imagesArray[index]  = crop(imagesArray[index], images_N[index], images_M[index], x, y, h, w);
      images_N[index] = h;
      images_M[index] = w;
    } else if (!strcmp(command, "ae")) {
      scanf("%d%d%d%d%d%d", &index, &rows, &cols, &R, &G, &B);
      imagesArray[index]  = extend(imagesArray[index], images_N[index], images_M[index], rows, cols, R, G, B);
      images_N[index] += 2 * rows;
      images_M[index] += 2 * cols;
    } else if (!strcmp(command, "ap")) {
      scanf("%d%d%d%d", &index_dst, &index_src, &x, &y);
      int dstN = images_N[index_dst];
      int dstM = images_M[index_dst];
      int srcN = images_N[index_src];
      int srcM = images_M[index_src];
      imagesArray[index_dst]  = paste(imagesArray[index_dst], dstN, dstM, imagesArray[index_src], srcN, srcM, x, y);
    } else if (!strcmp(command, "cf")) {
      scanf("%d", &size);

      // updatez dimensiunea vectorului care imi retine dimensiunile filtrelor
      filters_size = (int *)realloc(filters_size, (nr_filters + 1) * sizeof(int));
      filters_size[nr_filters] = size;

      // updatez dimensiunea vectorului care imi retine filtrele
      filtersArray = (float ***)realloc(filtersArray, (nr_filters + 1) * sizeof(float **));

      filtersArray[nr_filters] = aloc_filter(size);
      for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
          scanf("%f", &filtersArray[nr_filters][i][j]);
        }
      }

      // retin numarul nou de filtre
      nr_filters++;
    } else if (!strcmp(command, "af")) {
      scanf("%d%d", &index_img, &index_filter);
      int dimN = images_N[index_img];
      int dimM = images_M[index_img];
      int dimF = filters_size[index_filter];
      imagesArray[index_img] = apply_filter(imagesArray[index_img], dimN, dimM, filtersArray[index_filter], dimF);
    } else if (!strcmp(command, "df")) {
      scanf("%d", &index_filter);

      dezaloc_filter(filtersArray[index_filter], filters_size[index_filter]);

      for (i = index_filter; i < nr_filters - 1; i++) {
        filtersArray[i] = filtersArray[i + 1];
        filters_size[i] = filters_size[i + 1];
      }

      // retin numarul nou de filtre
      nr_filters--;

      // updatez dimensiunea vectorului care imi retine filtrele
      filtersArray = (float ***)realloc(filtersArray, nr_filters * sizeof(float **));

      // updatez dimensiunea vectorului care imi retine dimensiunile filtrelor
      filters_size = (int *)realloc(filters_size, nr_filters * sizeof(int));
    } else if (!strcmp(command, "di")) {
      scanf("%d", &index_img);

      dezaloc_image(imagesArray[index_img], images_N[index_img], images_M[index_img]);

      for (i = index_img; i < nr_images - 1; i++) {
        imagesArray[i] = imagesArray[i + 1];
        images_N[i] = images_N[i + 1];
        images_M[i] = images_M[i + 1];
      }

      // retin numarul nou de imagini
      nr_images--;

      // updatez dimensiunea vectorului care imi retine imaginile
      imagesArray = (int ****)realloc(imagesArray, nr_images * sizeof(int ***));
      // updatez dimensiunea vectorului care imi retine dimensiunile filtrelor
      images_N = (int *)realloc(images_N, nr_images * sizeof(int));
      images_M = (int *)realloc(images_M, nr_images * sizeof(int));
    }
  } while (strcmp(command, "e"));

  // dezalocarea tuturor vectorilor alocati dinamic
  for (i = 0; i < nr_images; i++) {
    dezaloc_image(imagesArray[i], images_N[i], images_M[i]);
  }
  free(imagesArray);
  free(images_N);
  free(images_M);

  for (i = 0; i < nr_filters; i++) {
    dezaloc_filter(filtersArray[i], filters_size[i]);
  }
  free(filtersArray);
  free(filters_size);

  free(path);
  free(command);

  return 0;
}
