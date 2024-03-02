#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timelib.h"

#define MAX_TIMEZONE_NAME_LENGTH 4
#define MAX_PERSON_NAME_LENGTH 10

typedef struct {
  unsigned int start_time, end_time;
} freeTime;

typedef struct {
  char person_name[MAX_PERSON_NAME_LENGTH];
  char timezone_name[MAX_TIMEZONE_NAME_LENGTH];
  int timezone_index;
  int nr_intervals;
  freeTime *intervals;
} personData;

int compare(const void *a, const void *b) {
  return (int)(*(unsigned int*)a - *(unsigned int*)b);
}

int compareForStructs(const void *a, const void *b) {
  personData *dataA = (personData *)a;
  personData *dataB = (personData *)b;

  return strcmp(dataA->person_name, dataB->person_name);
}

int main() {
  // TODO(me) Task 9

  int i = 0, j = 0, k = 0, T = 0, P = 0, F = 0, event_length = 0;
  int index = 0, nr_intervals = 0;
  unsigned int start = 0, end = 0, ref_start = 0, timestamp = 0, length = 0;
  TTimezone standard = {"UTC", 0};
  TDateTimeTZ date_time_tz = {
    date: {0, 0, 0},
    time: {0, 0, 0},
    tz: &standard
  };

  // aloc dinamic vectorul in care voi pune start_time-urile diferite intre ele
  int nr_start_times = 0;
  unsigned int *startTimeArray = (unsigned int *)malloc(nr_start_times * sizeof(unsigned int));

  // citesc numarul de timezone-uri si timezone-urile
  scanf("%d", &T);
  TTimezone timezonesArray[T];
  for (i = 0; i < T; i++) {
    scanf("%s%hhd", timezonesArray[i].name, &timezonesArray[i].utc_hour_difference);
  }

  // citesc numarul de persoane si datele despre acestea
  scanf("%d", &P);
  personData personDataArray[P];
  for (i = 0; i < P; i++) {
    scanf("%s%s%d",
      personDataArray[i].person_name,
      personDataArray[i].timezone_name,
      &personDataArray[i].nr_intervals);

    // determinul indexul timezoneului persoanei respective
    for (index = 0; index < T; index++) {
      if (strcmp(timezonesArray[index].name, personDataArray[i].timezone_name) == 0) {
        personDataArray[i].timezone_index = index;
        break;
      }
    }

    // aloc dinamic vectorul de intervale
    nr_intervals = 0;
    personDataArray[i].intervals = (freeTime *)malloc(nr_intervals * sizeof(freeTime *));

    for (j = 0; j < personDataArray[i].nr_intervals; j++) {
      scanf("%u%hhu%hhu%u%u",
        &date_time_tz.date.year, &date_time_tz.date.month, &date_time_tz.date.day,
        &start, &end);

      // timestamp
      date_time_tz.time.hour = start;
      standard.utc_hour_difference = timezonesArray[personDataArray[i].timezone_index].utc_hour_difference;
      timestamp = convertDateTimeTZToUnixTimestamp(date_time_tz);

      // REZOLVARE BUG
      if (date_time_tz.date.month == 1) {
        timestamp += HOURS_PER_DAY * SECONDS_PER_HOUR;
      }

      // inceputul si sfarsitul intervalului
      start = timestamp;
      end = start + end * SECONDS_PER_HOUR;

      // unificarea intervalelor care se succed
      if (nr_intervals == 0 || personDataArray[i].intervals[nr_intervals - 1].end_time != start) {
        nr_intervals++;
        personDataArray[i].intervals = (freeTime *)realloc(
          personDataArray[i].intervals,
          nr_intervals * sizeof(freeTime *));
        personDataArray[i].intervals[nr_intervals - 1].start_time = start;
        personDataArray[i].intervals[nr_intervals - 1].end_time = end;

        // adaugarea start_time-ului in vectorul startTimeArray
        for (k = 0; k < nr_start_times; k++) {
          if (startTimeArray[k] == start) {
            break;
          }
        }

        if (k == nr_start_times) {
          nr_start_times++;
          startTimeArray = (unsigned int *)realloc(startTimeArray, nr_start_times * sizeof(unsigned int));
          startTimeArray[nr_start_times - 1] = start;
        }
      } else {
        personDataArray[i].intervals[nr_intervals - 1].end_time = end;
      }
    }
    personDataArray[i].nr_intervals = nr_intervals;
  }

  // citire date eveniment
  scanf("%d%d", &F, &event_length);

  // sortez crescator dupa nume vectorul personDataArray
  qsort(personDataArray, P, sizeof(personData), compareForStructs);

  // sortez crescator vectorul startTimeArray
  qsort(startTimeArray, nr_start_times, sizeof(unsigned int), compare);

  int missing = F;
  for (k = 0; k < nr_start_times; k++) {
    ref_start = startTimeArray[k];

    for (i = 0; i < P; i++) {
      nr_intervals = personDataArray[i].nr_intervals;
      for (j = 0; j < nr_intervals; j++) {
        start = personDataArray[i].intervals[j].start_time;
        end = personDataArray[i].intervals[j].end_time;
        length = (end - start) / SECONDS_PER_HOUR;

        if (
          (start <= ref_start && ref_start <= end) &&
          length >= event_length &&
          ((end - ref_start) / SECONDS_PER_HOUR >= event_length)) {
          missing--;
          break;
        }
      }

      if (missing == 0) {
        break;
      }
    }

    if (missing == 0) {
      break;
    } else {
      missing = F;
    }
  }

  if (k < nr_start_times) {
    for (i = 0; i < P; i++) {
      printf("%s: ", personDataArray[i].person_name);

      nr_intervals = personDataArray[i].nr_intervals;
      for (j = 0; j < nr_intervals; j++) {
        start = personDataArray[i].intervals[j].start_time;
        end = personDataArray[i].intervals[j].end_time;
        length = (end - start) / SECONDS_PER_HOUR;

        if (
          (start <= ref_start && ref_start <= end) &&
          length >= event_length &&
          ((end - ref_start) / SECONDS_PER_HOUR >= event_length)) {
            date_time_tz = convertUnixTimestampToDateTimeTZ(
              ref_start,
              timezonesArray,
              personDataArray[i].timezone_index);
            printDateTimeTZ(date_time_tz);
            break;
        }
      }

      if (j == nr_intervals) {
        printf("invalid\n");
      }
    }
  } else {
    printf("imposibil\n");
  }

  free(startTimeArray);
  for (i = 0; i < P; i++) {
    free(personDataArray[i].intervals);
  }

  return 0;
}
