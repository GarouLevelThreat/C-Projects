#ifndef TIMELIB_H_INCLUDED
#define TIMELIB_H_INCLUDED
#define MAX_NAME_LENGTH 5

#define DAYS_IN_NORMAL_YEAR 365
#define SECONDS_PER_MINUTE 60
#define SECONDS_PER_HOUR 3600
#define SECONDS_PER_DAY 86400
#define HOURS_PER_DAY 24
#define MONTHS_PER_YEAR 12
#define NUMBER_1 1
#define NUMBER_10 10
#define NUMBER_28 28
#define NUMBER_29 29
#define NUMBER_30 30
#define NUMBER_31 31
#define NUMBER_100 100
#define NUMBER_400 400
#define UNIX_YEAR_0 1970
#define UNIX_LEAP_YEAR_0 1972

typedef struct {
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
} TTime;

typedef struct {
    unsigned char day;
    unsigned char month;
    unsigned int year;
} TDate;

typedef struct {
    char name[MAX_NAME_LENGTH];
    signed char utc_hour_difference;
} TTimezone;

typedef struct {
    TDate date;
    TTime time;
    TTimezone *tz;
} TDateTimeTZ;

TTime convertUnixTimestampToTime(unsigned int timestamp);
TDate convertUnixTimestampToDateWithoutLeapYears(unsigned int timestamp);
TDate convertUnixTimestampToDate(unsigned int timestamp);
TDateTimeTZ convertUnixTimestampToDateTimeTZ(unsigned int timestamp, TTimezone *timezones, int timezone_index);
unsigned int convertDateTimeTZToUnixTimestamp(TDateTimeTZ);
void printDateTimeTZ(TDateTimeTZ datetimetz);

#endif  // TIMELIB_H_INCLUDED
