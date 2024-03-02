#include <stdio.h>
#include <stdlib.h>
#include "timelib.h"

#define DATE_BITS_LENGTH 15
#define UNSIGNED_INT_BITS_LENGTH 32
#define DAY_END 4
#define MONTH_END 8
#define YEAR_END 14
#define UNIX_YEAR_0 1970

struct date {
  unsigned int day, month, year;
};

int compare(const void *a, const void *b) {
  return (*(int *)a - *(int *)b);
}

unsigned int BitsToInt(unsigned long long int *numbers_bits, int *number_bits_index, int *start) {
  unsigned int result = 0;
  int cnt = 0;

  while (cnt < DATE_BITS_LENGTH) {
    if (*start >= UNSIGNED_INT_BITS_LENGTH) {
      (*number_bits_index)++;
      *start = 0;
    }

    result = result | (((numbers_bits[*number_bits_index] >> *start) & 1) << cnt);
    (*start)++;
    cnt++;
  }

  return result;
}

void BitsToDate(struct date *date, unsigned int *numbers, int number_index) {
  unsigned int result = 0;
  int start = 0, cnt = 0, p = 1;

  while (cnt < DATE_BITS_LENGTH) {
    result += p * ((numbers[number_index] >> start) & 1);
    if (cnt == DAY_END) {
      date[number_index].day = result;
      result = 0;
      p = 1;
    } else if (cnt == MONTH_END) {
      date[number_index].month = result;
      result = 0;
      p = 1;
    } else if (cnt == YEAR_END) {
      date[number_index].year = result;
    } else {
      p *= 2;
    }

    start++;
    cnt++;
  }
}

void displayDate(struct date date) {
  char *months[] = {
    "ianuarie", "februarie", "martie", "aprilie",
    "mai", "iunie", "iulie", "august", "septembrie",
    "octombrie", "noiembrie", "decembrie"
  };

  printf("%d %s %d\n", date.day, months[date.month - 1], UNIX_YEAR_0 + date.year);
}

int controlBit(unsigned long long int n) {
  int i = 0;
  unsigned int result = 0;

  for (i = 0; i < UNSIGNED_INT_BITS_LENGTH; i++) {
    result += ((n >> i) & 1);
  }

  return ((int)result % 2);
}

int main() {
  unsigned char task = '\0';
  scanf("%c", &task);

  if (task == '7') {
    // declaratii si initializari
    int i = 0, N = 0;

    // citesc numarul de UNSIGNED INTURI
    scanf("%d", &N);

    // citesc N UNSIGNED INTURI
    unsigned int *numbers = (unsigned int *)malloc(N * sizeof(unsigned int));
    struct date *date = (struct date *)malloc(N * sizeof(struct date));
    for (i = 0; i < N; i++) {
      scanf("%u", &numbers[i]);
    }

    // sortez numerele
    qsort(numbers, N, sizeof(unsigned int), compare);

    for (i = 0; i < N; i++) {
      BitsToDate(date, numbers, i);
      displayDate(date[i]);
    }

    free(numbers);
    free(date);
  } else if (task == '8') {
    // declaratii si initializari
    int i = 0, aux = 0, n = 0, N = 0, N_control = 0;
    int start = 0, number_bits_index = 0, number_index = 0, date_index = 0;
    int number_control_bit = 0, control_bit_index = 0, control_index = 0;

    // n este numarul de date
    scanf("%d", &n);

    // N este numarul de unsigned int uri care vor fi citite pentru date
    N = (n * DATE_BITS_LENGTH / UNSIGNED_INT_BITS_LENGTH);
    N += ((n * DATE_BITS_LENGTH) % UNSIGNED_INT_BITS_LENGTH == 0) ? 0 : 1;

    // N este numarul de unsigned int uri care vor fi citite pentru bitii de control
    N_control = N / UNSIGNED_INT_BITS_LENGTH;
    N_control += (N % UNSIGNED_INT_BITS_LENGTH == 0) ? 0 : 1;

    // aloc dinamic vectorul de unsigned inturi pe care il voi folosi in extragerea datelor
    unsigned int *numbers = (unsigned int *)malloc(0);

    // citesc N UNSIGNED INTURI
    unsigned long long int *numbers_bits = (unsigned long long int *)malloc(N * sizeof(unsigned long long int));
    for (i = 0; i < N; i++) {
      scanf("%llu", &numbers_bits[i]);
    }

    // citesc N_control UNSIGNED INTURI pt bitii de control
    unsigned int *control = (unsigned int *)malloc(N * sizeof(unsigned int));
    for (i = 0; i < N_control; i++) {
      scanf("%u", &control[i]);
    }

    while (date_index < n) {
      number_control_bit  = controlBit(numbers_bits[number_bits_index]);
      if (number_control_bit == ((control[control_index] >> control_bit_index) & 1)) {
        aux = number_bits_index;

        // transform sirul de biti cu informatia intr-un unsigned int
        number_index++;
        numbers = (unsigned int *)realloc(numbers, number_index * sizeof(unsigned int));
        numbers[number_index - 1] = BitsToInt(numbers_bits, &number_bits_index, &start);

        // verific daca unsigned int ul a fost corupt sau nu
        if (aux != number_bits_index) {
          // updatez indicii de control
          control_index = number_bits_index / UNSIGNED_INT_BITS_LENGTH;
          control_bit_index = number_bits_index % UNSIGNED_INT_BITS_LENGTH;

          number_control_bit = controlBit(numbers_bits[number_bits_index]);
          if (number_control_bit != ((control[control_index] >> control_bit_index) & 1)) {
            number_index--;
            numbers = (unsigned int *)realloc(numbers, number_index * sizeof(unsigned int));
          }
        }
      } else {
        if (start + DATE_BITS_LENGTH < UNSIGNED_INT_BITS_LENGTH) {
          start += DATE_BITS_LENGTH;
        } else {
          start = DATE_BITS_LENGTH - (UNSIGNED_INT_BITS_LENGTH - start);
          number_bits_index++;

          control_index = number_bits_index / UNSIGNED_INT_BITS_LENGTH;
          control_bit_index = number_bits_index % UNSIGNED_INT_BITS_LENGTH;
        }
      }

      // contorizez data prelucrata
      date_index++;
    }

    // sortez numerele
    qsort(numbers, number_index, sizeof(unsigned int), compare);

    // aloc dinamic vectorul de structuri care imi retine datele
    struct date *date = (struct date *)malloc(number_index * sizeof(struct date));

    for (i = 0; i < number_index; i++) {
      BitsToDate(date, numbers, i);
      displayDate(date[i]);
    }

    free(numbers);
    free(numbers_bits);
    free(control);
    free(date);
  }

  return 0;
}
