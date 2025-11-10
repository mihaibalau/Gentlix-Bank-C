#include "domain.h"
#include <stddef.h>

Date createDate(short day, short month, short year){
    Date new_date;

    new_date.day = day;
    new_date.month = month;
    new_date.year = year;

    return new_date;
}

short getDay(Date* received_date){
    if (received_date == NULL) return 0;
    return received_date->day;
}

short getMonth(Date* received_date){
    if (received_date == NULL) return 0;
    return received_date->month;
}

short getYear(Date* received_date){
    if (received_date == NULL) return 0;
    return received_date->year;
}

void setDay(Date* received_date, short received_day){
    if (received_date == NULL) return;
    received_date->day = received_day;
}

void setMonth(Date* received_date, short received_month){
    if (received_date == NULL) return;
    received_date->month = received_month;
}

void setYear(Date* received_date, short received_year){
    if (received_date == NULL) return;
    received_date->year = received_year;
}
