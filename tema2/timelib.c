#include <stdio.h>
#include <stdlib.h>
#include "timelib.h"

// TODO(me) Task 1
TTime convertUnixTimestampToTime(unsigned int timestamp) {
  TTime result = {
    hour: 0,
    min: 0,
    sec: 0
  };

  result.hour = (timestamp / SECONDS_PER_HOUR) % HOURS_PER_DAY;
  timestamp %= SECONDS_PER_HOUR;
  result.min = timestamp / SECONDS_PER_MINUTE;
  timestamp %= SECONDS_PER_MINUTE;
  result.sec = timestamp;

  return result;
}

// TODO(me) Task 2
TDate convertUnixTimestampToDateWithoutLeapYears(unsigned int timestamp) {
  int i = 0;
  unsigned int seconds_per_month = 0;
  const int seconds_per_year = DAYS_IN_NORMAL_YEAR * SECONDS_PER_DAY;
  unsigned char daysInMonths[MONTHS_PER_YEAR] = {
    NUMBER_31, NUMBER_28, NUMBER_31, NUMBER_30, NUMBER_31, NUMBER_30,
    NUMBER_31, NUMBER_31, NUMBER_30, NUMBER_31, NUMBER_30, NUMBER_31
  };

  // initializez result cu prima data inregistrata
  TDate result = {
    day: NUMBER_1,
    month: NUMBER_1,
    year: UNIX_YEAR_0
  };

  // determin anul
  result.year += timestamp / seconds_per_year;
  timestamp = timestamp % seconds_per_year;

  // determin luna
  seconds_per_month = daysInMonths[i] * SECONDS_PER_DAY;
  while (timestamp >= seconds_per_month) {
    timestamp -= seconds_per_month;
    seconds_per_month = daysInMonths[++i] * SECONDS_PER_DAY;
  }
  result.month += i;

  // determin ziua
  result.day += timestamp / SECONDS_PER_DAY;

  return result;
}

unsigned char isBisect(unsigned int year) {
  if (year % NUMBER_400 == 0 || (year % 4 == 0 && year % NUMBER_100 != 0)) {
    return 1;
  }
  return 0;
}

// TODO(me) Task 3
TDate convertUnixTimestampToDate(unsigned int timestamp) {
  int i = 0;
  // determin data cand se ignora anii bisecti
  TDate result = convertUnixTimestampToDateWithoutLeapYears(timestamp);

  unsigned char daysInMonths[MONTHS_PER_YEAR] = {
    NUMBER_31, NUMBER_28, NUMBER_31, NUMBER_30, NUMBER_31, NUMBER_30,
    NUMBER_31, NUMBER_31, NUMBER_30, NUMBER_31, NUMBER_30, NUMBER_31
  };
  if (isBisect(result.year)) {
    daysInMonths[1] = NUMBER_29;
  } else {
    daysInMonths[1] = NUMBER_28;
  }

  // scad numarul de secunde echivalent unei zile si updatez data
  for (i = UNIX_LEAP_YEAR_0; i <= result.year; i += 4) {
    if (result.day == 1) {
      if (result.month == 1) {
        result.year--;
        if (isBisect(result.year)) {
          daysInMonths[1] = NUMBER_29;
        } else {
          daysInMonths[1] = NUMBER_28;
        }

        result.month = MONTHS_PER_YEAR;
      } else {
        result.month--;
      }

      result.day = daysInMonths[result.month - 1];
    } else {
      result.day--;
    }
  }

  return result;
}

// TODO(me) Task 4
TDateTimeTZ convertUnixTimestampToDateTimeTZ(unsigned int timestamp, TTimezone *timezones, int timezone_index) {
  TDateTimeTZ result = {
    date: {0, 0, 0},
    time: {0, 0, 0},
    tz: timezones + timezone_index
  };

  // realizez transformarea la UTC
  timestamp += result.tz->utc_hour_difference * SECONDS_PER_HOUR;

  result.date = convertUnixTimestampToDate(timestamp);
  result.time = convertUnixTimestampToTime(timestamp);

  return result;
}

// TODO(me) Task 5
unsigned int convertDateTimeTZToUnixTimestamp(TDateTimeTZ datetimetz) {
  unsigned int i = 0, timestamp = 0;

  // contorizez secundele din toti anii incheiati incepand cu 1970
  for (i = UNIX_YEAR_0; i < datetimetz.date.year; i++) {
    timestamp += SECONDS_PER_DAY * DAYS_IN_NORMAL_YEAR;
    if (isBisect(i)) {
      timestamp += SECONDS_PER_DAY;
    }
  }

  unsigned char daysInMonths[MONTHS_PER_YEAR] = {
    NUMBER_31, NUMBER_28, NUMBER_31, NUMBER_30, NUMBER_31, NUMBER_30,
    NUMBER_31, NUMBER_31, NUMBER_30, NUMBER_31, NUMBER_30, NUMBER_31
  };
  if (isBisect(datetimetz.date.year)) {
    daysInMonths[1] = NUMBER_29;
  } else {
    daysInMonths[1] = NUMBER_28;
  }

  // contorizez secundele din toate lunile incheiate ale ultimului an
  for (i = 0; i < datetimetz.date.month - 1; i++) {
    timestamp += daysInMonths[i] * SECONDS_PER_DAY;
  }
  // contorizez secundele din toate zilele incheiate ale ultimei luni
  timestamp += (datetimetz.date.day - 1) * SECONDS_PER_DAY;

  // contorizez secundele din ultima zi`
  timestamp += datetimetz.time.hour * SECONDS_PER_HOUR;
  timestamp += datetimetz.time.min * SECONDS_PER_MINUTE;
  timestamp += datetimetz.time.sec;

  // realizez transformarea la UTC
  timestamp -= datetimetz.tz->utc_hour_difference * SECONDS_PER_HOUR;

  return timestamp;
}

void printWithOrWithoutPrefix(unsigned char date) {
  if (date < NUMBER_10) {
    printf("0%d", date);
  } else {
    printf("%d", date);
  }
}

// TODO(me) Task 6
void printDateTimeTZ(TDateTimeTZ datetimetz) {
  // print date
  char *months[] = {
    "ianuarie", "februarie", "martie", "aprilie", "mai", "iunie",
    "iulie", "august", "septembrie", "octombrie", "noiembrie", "decembrie"
  };

  printWithOrWithoutPrefix(datetimetz.date.day);
  printf(" ");
  printf("%s ", months[datetimetz.date.month - 1]);
  printf("%d, ", datetimetz.date.year);

  // print time
  printWithOrWithoutPrefix(datetimetz.time.hour);
  printf(":");
  printWithOrWithoutPrefix(datetimetz.time.min);
  printf(":");
  printWithOrWithoutPrefix(datetimetz.time.sec);
  printf(" ");

  // print timezone name
  printf("%s ", datetimetz.tz->name);

  // print UTC[+/-][timezone utc hour diff]
  int absVal = datetimetz.tz->utc_hour_difference;
  if (absVal < 0) {
    absVal -= 2 * absVal;
  }

  printf("(UTC%c%d)\n", datetimetz.tz->utc_hour_difference < 0 ? '-':'+', absVal);
}
